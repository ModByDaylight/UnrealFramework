#include <Unreal/Hooks.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include <Unreal/UnrealInitializer.hpp>
#include <Unreal/FFrame.hpp>
#include <Unreal/UObjectGlobals.hpp>
#include <Unreal/UnrealVersion.hpp>
#include <Unreal/UClass.hpp>
// TODO: Uncomment when Lua & Unreal have been decoupled
//#include <LuaType/LuaUObject.hpp>
// TODO: Uncomment when UE4SS & Unreal have been decoupled
//#include <UE4SSProgram.hpp>

//#include <polyhook2/CapstoneDisassembler.hpp>
#include <polyhook2/ZydisDisassembler.hpp>

namespace RC::Unreal
{
    uint64_t hook_trampoline_process_event = NULL;
    uint64_t hook_trampoline_static_construct_object = NULL;

    static HookedUFunctionMap g_hooked_script_functions{};

    auto Hook::add_required_object(const std::wstring& object_full_typeless_name) -> void
    {
        if (!UObjectGlobals::static_find_object(nullptr, nullptr, object_full_typeless_name))
        {
            Output::send(STR("Need to construct: {}\n"), object_full_typeless_name);
            StaticStorage::required_objects_for_init.emplace_back(Hook::StaticStorage::RequiredObject{object_full_typeless_name});
            StaticStorage::all_required_objects_constructed = false;
        }
        else
        {
            Output::send(STR("Already constructed: {}\n"), object_full_typeless_name);
            if (StaticStorage::required_objects_for_init.size() == 0)
            {
                StaticStorage::all_required_objects_constructed = true;
            }
        }
    }

    auto Hook::register_process_event_callback(ProcessEventCallback callback, HookType hook_type) -> void
    {
        switch (hook_type)
        {
            case HookType::Pre:
                StaticStorage::process_event_pre_callbacks.emplace_back(callback);
                break;
            case HookType::Post:
                StaticStorage::process_event_post_callbacks.emplace_back(callback);
                break;
        }
    }

    auto get_hooked_functions_map() -> HookedUFunctionMap&
    {
        return g_hooked_script_functions;
    }

    auto unreal_script_function_hook(UObject* context, FFrame& the_stack, void* result_decl) -> void
    {
        try
        {
            // Try fetching the data corresponding to this UFunction from a global unordered_map
            const auto& func = get_hooked_functions_map().at(the_stack.CurrentNativeFunction);

            // The 'result_decl' value can be changed by callbacks
            // Keep in mind that it's only the last callback that can control the result_decl value
            for (const auto& callback : func.pre_callbacks)
            {
                callback.callable({.context = context, .the_stack = the_stack, .result_decl = result_decl}, callback.custom_data);
            }

            // After the callback function has been executed you should call the original function
            // This will execute any internal UE4 scripting functions & native functions depending on the type of UFunction
            func.original_func(context, the_stack, result_decl);

            for (const auto& return_value_callback : func.post_callbacks)
            {
                return_value_callback.callable({.context = context, .the_stack = the_stack, .result_decl = result_decl}, return_value_callback.custom_data);
            }
        }
        catch (std::out_of_range&)
        {
            Output::send(STR("A UFunction was hooked and the original Func pointer was not stored\n"));
        }
        catch (std::runtime_error& e)
        {
            Output::send(STR("{}\n"), fmt(L"%S", e.what()));
        }
        catch (...)
        {
            Output::send(STR("Unknown exception thrown\n"));
        }
    }

    auto hooked_static_construct_object_body(UObject* constructed_object) -> void
    {
        if (!Hook::StaticStorage::all_required_objects_constructed)
        {
            for (auto& required_object : Hook::StaticStorage::required_objects_for_init)
            {
                if (Hook::StaticStorage::num_required_objects_constructed >= Hook::StaticStorage::required_objects_for_init.size())
                {
                    Hook::StaticStorage::all_required_objects_constructed = true;
                    break;
                }

                if (required_object.object_constructed) { continue; }

                UObject* required_object_ptr = UObjectGlobals::static_find_object(nullptr, nullptr, required_object.object_name);
                if (required_object_ptr)
                {
                    required_object.object_constructed = true;
                    ++Hook::StaticStorage::num_required_objects_constructed;
                    Output::send(STR("Constructed [{} / {}]: {}\n"), Hook::StaticStorage::num_required_objects_constructed, Hook::StaticStorage::required_objects_for_init.size(), required_object.object_name);
                }
            }
        }

        // TODO: Uncomment when Lua & Unreal have been decoupled
        /*
        if (UnrealInitializer::StaticStorage::is_initialized)
        {
            UStruct* object_class = constructed_object->get_uclass();
            while (object_class)
            {
                for (const auto& callback_data : Hook::StaticStorage::static_construct_object_lua_callbacks)
                {
                    if (callback_data.instance_of_class == object_class)
                    {
                        try
                        {
                            callback_data.lua.registry().get_function_ref(callback_data.registry_index);
                            LuaType::auto_construct_object(callback_data.lua, constructed_object);
                            callback_data.lua.call_function(1, 0);
                        }
                        catch (std::runtime_error& e)
                        {
                            Output::send(STR("{}\n"), to_wstring(e.what()));
                        }
                    }
                }

                object_class = object_class->get_super_struct();
            }
        }
        //*/
    }

    auto hooked_static_construct_object_deprecated(StaticConstructObject_Internal_Params_Deprecated) -> UObject*
    {
        UObject* constructed_object = PLH::FnCast(hook_trampoline_static_construct_object, UObjectGlobals::GlobalState::static_construct_object_internal_deprecated.get_function_pointer())(
                InClass_,
                InOuter_,
                InName_,
                InFlags_,
                InternalSetFlags_,
                InTemplate_,
                bCopyTransientsFromClassDefaults_,
                InInstanceGraph_,
                bAssumeTemplateIsArchetype_,
                ExternalPackage_
        );

        hooked_static_construct_object_body(constructed_object);

        return constructed_object;
    }

    auto hooked_static_construct_object(const FStaticConstructObjectParameters& params) -> UObject*
    {
        UObject* constructed_object = PLH::FnCast(hook_trampoline_static_construct_object, UObjectGlobals::GlobalState::static_construct_object_internal.get_function_pointer())(
                params
        );

        hooked_static_construct_object_body(constructed_object);

        return constructed_object;
    }

    auto hooked_process_event(UObject* context, UFunction* function, void* parms) -> void
    {
        for (const auto& callback : Hook::StaticStorage::process_event_pre_callbacks)
        {
            callback(context, function, parms);
        }

        PLH::FnCast(hook_trampoline_process_event, UObject::process_event_internal.get_function_pointer())(context, function, parms);

        for (const auto& callback : Hook::StaticStorage::process_event_post_callbacks)
        {
            callback(context, function, parms);
        }

        // TODO: Uncomment when UE4SS & Unreal have been decoupled
        /*
        if (UE4SSProgram::m_should_load_all_assets && !UE4SSProgram::m_assets_are_loading)
        {
            UE4SSProgram::load_all_assets_thread();

            // Loading assets is an asynchronous operation
            // It's very important to not reset this bool until after the assets have been loaded
            // This is what keeps the load-requester waiting until all the assets have been loaded
            UE4SSProgram::m_should_load_all_assets = false;
        }
        //*/
    }

    auto hook_static_construct_object() -> void
    {
        PLH::ZydisDisassembler dis(PLH::Mode::x64);

        if (Version::is_atmost(4, 25))
        {
            Hook::StaticStorage::static_construct_object_detour = std::make_unique<PLH::x64Detour>(
                    static_cast<char*>(UObjectGlobals::GlobalState::static_construct_object_internal_deprecated.get_function_address()),
                    std::bit_cast<char*>(&hooked_static_construct_object_deprecated),
                    &hook_trampoline_static_construct_object,
                    dis);
        }
        else
        {
            Hook::StaticStorage::static_construct_object_detour = std::make_unique<PLH::x64Detour>(
                    static_cast<char*>(UObjectGlobals::GlobalState::static_construct_object_internal.get_function_address()),
                    std::bit_cast<char*>(&hooked_static_construct_object),
                    &hook_trampoline_static_construct_object,
                    dis);
        }

        Hook::StaticStorage::static_construct_object_detour->hook();
    }

    auto hook_process_event() -> void
    {
        PLH::ZydisDisassembler dis(PLH::Mode::x64);
        Hook::StaticStorage::process_event_detour = std::make_unique<PLH::x64Detour>(
                static_cast<char*>(UObject::process_event_internal.get_function_address()),
                std::bit_cast<char*>(&hooked_process_event),
                &hook_trampoline_process_event,
                dis);

        Hook::StaticStorage::process_event_detour->hook();
    }
}
