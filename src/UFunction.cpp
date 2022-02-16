#include <Unreal/UFunction.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/FProperty.hpp>

namespace RC::Unreal
{
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UFunction);

    using MemberOffsets = ::RC::Unreal::StaticOffsetFinder::MemberOffsets;

    auto UFunction::get_function_flags() -> EFunctionFlags
    {
        // Works from 4.18 to 4.27 (confirmed)
        // Before 4.18, there was 'uint16 RepOffset;' between 'NumParms' and 'FunctionFlags'
        // For reference, 'NumParms' is 8 bits, and 'FunctionFlags' is 32 bits
        return Helper::Casting::offset_deref<EFunctionFlags>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UFunction_NumParms) - 4);
    }

    auto UFunction::get_func_ptr() -> UnrealScriptFunction
    {
        return Helper::Casting::offset_deref<UnrealScriptFunction>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UFunction_Func));
    }

    auto UFunction::set_func_ptr(UnrealScriptFunction new_func_ptr) -> void
    {
        UnrealScriptFunction* func = static_cast<UnrealScriptFunction*>(static_cast<void*>(this + StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UFunction_Func)));
        *func = new_func_ptr;
    }

    auto UFunction::get_num_parms() -> uint8_t
    {
        uint8_t num_parameters = 0;

        this->for_each_property([&](FProperty* param) {
            if (param->HasAnyPropertyFlags(Unreal::CPF_Parm))
            {
                num_parameters++;
            }
            return LoopAction::Continue;
        });
        return num_parameters;
    }

    auto UFunction::get_parms_size() -> int32_t
    {
        int32_t params_size = 0;

        this->for_each_property([&](FProperty* param) {
            if (param->HasAnyPropertyFlags(Unreal::CPF_Parm))
            {
                params_size = param->GetOffset_ForInternal() + param->GetSize();
            }
            return LoopAction::Continue;
        });
        return params_size;
    }

    auto UFunction::get_return_property() -> FProperty*
    {
        FProperty* return_property = nullptr;

        this->for_each_property([&](FProperty* param) {
            if (param->HasAnyPropertyFlags(Unreal::CPF_ReturnParm))
            {
                return_property = param;
                return LoopAction::Break;
            }
            return LoopAction::Continue;
        });
        return return_property;
    }

    auto UFunction::get_function_hook_data() -> UnrealScriptFunctionData&
    {
        Internal::HookedUFunctionMap& hooked_functions = Internal::get_hooked_functions_map();
        auto iterator = hooked_functions.find(this);

        if (iterator == hooked_functions.end())
        {
            iterator = hooked_functions.insert({this, UnrealScriptFunctionData(get_func_ptr())}).first;
            set_func_ptr(&Internal::unreal_script_function_hook);
        }
        return iterator->second;
    }

    auto UFunction::register_pre_hook(const UnrealScriptFunctionCallable& pre_callback) -> CallbackId
    {
        UnrealScriptFunctionData& function_data = get_function_hook_data();
        return function_data.add_pre_callback(pre_callback);
    }

    auto UFunction::register_post_hook(const UnrealScriptFunctionCallable& pre_callback) -> CallbackId
    {
        UnrealScriptFunctionData& function_data = get_function_hook_data();
        return function_data.add_pre_callback(pre_callback);
    }

    auto UFunction::unregister_hook(CallbackId callback_id) -> bool
    {
        UnrealScriptFunctionData& function_data = get_function_hook_data();
        return function_data.remove_callback(callback_id);
    }

    auto UFunction::unregister_all_hooks() -> void
    {
        Internal::HookedUFunctionMap& hooked_functions = Internal::get_hooked_functions_map();
        auto iterator = hooked_functions.find(this);

        if (iterator != hooked_functions.end())
        {
            iterator->second.remove_all_callbacks();
            set_func_ptr(iterator->second.get_original_func_ptr());
            hooked_functions.erase(this);
        }
    }
}
