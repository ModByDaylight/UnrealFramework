#include <Unreal/Hooks.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include <Unreal/UnrealInitializer.hpp>
#include <Unreal/FFrame.hpp>
#include <Unreal/UObjectGlobals.hpp>
#include <Unreal/UnrealVersion.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UAssetRegistry.hpp>
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

    auto Hook::register_static_construct_object_pre_callback(StaticConstructObjectPreCallback callback) -> void
    {
        StaticStorage::static_construct_object_pre_callbacks.emplace_back(callback);
    }

    auto Hook::register_static_construct_object_post_callback(StaticConstructObjectPostCallback callback) -> void
    {
        StaticStorage::static_construct_object_post_callbacks.emplace_back(callback);
    }

    auto Hook::register_process_event_pre_callback(ProcessEventCallback callback) -> void
    {
        StaticStorage::process_event_pre_callbacks.emplace_back(callback);
    }

    auto Hook::register_process_event_post_callback(ProcessEventCallback callback) -> void
    {
        StaticStorage::process_event_post_callbacks.emplace_back(callback);
    }

    auto hooked_static_construct_object_pre(const FStaticConstructObjectParameters& params) -> UObject*
    {
        UObject* altered_return_value{};
        for (const auto& callback : Hook::StaticStorage::static_construct_object_pre_callbacks)
        {
            altered_return_value = callback(params);
        }
        return altered_return_value;
    }

    auto hooked_static_construct_object_post(const FStaticConstructObjectParameters& params, UObject* constructed_object) -> UObject*
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

        UObject* altered_return_value{};
        for (const auto& callback : Hook::StaticStorage::static_construct_object_post_callbacks)
        {
            altered_return_value = callback(params, constructed_object);
        }
        return altered_return_value;
    }

    auto hooked_static_construct_object_deprecated(StaticConstructObject_Internal_Params_Deprecated) -> UObject*
    {
        const FStaticConstructObjectParameters params{
            .Class = InClass_,
            .Outer = InOuter_,
            .Name = InName_,
            .SetFlags = InFlags_,
            .InternalSetFlags = InternalSetFlags_,
            .bCopyTransientsFromClassDefaults = bCopyTransientsFromClassDefaults_,
            .bAssumeTemplateIsArchetype = bAssumeTemplateIsArchetype_,
            .Template = InTemplate_,
            .InstanceGraph = InInstanceGraph_,
            .ExternalPackage = static_cast<class UPackage*>(ExternalPackage_),
            .SubobjectOverrides = nullptr
        };

        UObject* altered_return_value = hooked_static_construct_object_pre(params);
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
        altered_return_value = hooked_static_construct_object_post(params, constructed_object);

        return altered_return_value ? altered_return_value : constructed_object;
    }

    auto hooked_static_construct_object(const FStaticConstructObjectParameters& params) -> UObject*
    {
        UObject* altered_return_value = hooked_static_construct_object_pre(params);
        UObject* constructed_object = PLH::FnCast(hook_trampoline_static_construct_object, UObjectGlobals::GlobalState::static_construct_object_internal.get_function_pointer())(
                params
        );
        altered_return_value = hooked_static_construct_object_post(params, constructed_object);

        return altered_return_value ? altered_return_value : constructed_object;
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

        if (UAssetRegistry::should_load_all_assets() && !UAssetRegistry::assets_are_loading())
        {
            UAssetRegistry::load_all_assets_thread();

            // Loading assets is an asynchronous operation
            // It's very important to not reset this bool until after the assets have been loaded
            // This is what keeps the load-requester waiting until all the assets have been loaded
            UAssetRegistry::set_should_load_all_assets(false);
        }
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
