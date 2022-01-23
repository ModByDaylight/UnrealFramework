#include <Unreal/UFunction.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/FProperty.hpp>

namespace RC::Unreal
{
    auto UFunction::get_function_flags() -> EFunctionFlags
    {
        // Works from 4.18 to 4.27 (confirmed)
        // Before 4.18, there was 'uint16 RepOffset;' between 'NumParms' and 'FunctionFlags'
        // For reference, 'NumParms' is 8 bits, and 'FunctionFlags' is 32 bits
        return Helper::Casting::offset_deref<EFunctionFlags>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UFunction_NumParms) - 4);
    }

    auto UFunction::get_num_parms() -> uint8_t
    {
        return Helper::Casting::offset_deref<uint8_t>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UFunction_NumParms));
    }

    auto UFunction::get_return_value_offset() -> uint16_t
    {
        return Helper::Casting::offset_deref<uint16_t>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UFunction_ReturnValueOffset));
    }

    auto UFunction::get_func_ptr() -> void*
    {
        return Helper::Casting::offset_deref<void*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UFunction_Func));
    }

    auto UFunction::set_func_ptr(UnrealScriptFunction new_func_ptr) -> void
    {
        UnrealScriptFunction* func = static_cast<UnrealScriptFunction*>(static_cast<void*>(this + StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UFunction_Func)));

        *func = new_func_ptr;
    }

    auto UFunction::get_return_property() -> FProperty*
    {
        FProperty* return_property{nullptr};

        this->for_each_property([&](FProperty* param) {
            if (param->has_any_property_flags(Unreal::CPF_ReturnParm))
            {
                return_property = param;
                return LoopAction::Break;
            }
            else
            {
                return LoopAction::Continue;
            }
        });

        return return_property;
    }

    auto UFunction::unhook_all() -> void
    {
        HookedUFunctionMap& hooked_functions = get_hooked_functions_map();
        if (hooked_functions.contains(this))
        {
            set_func_ptr(hooked_functions[this].original_func);

            erase_if(hooked_functions, [&](const auto& map_item) -> bool {
                auto const& [script_function, script_struct] = map_item;

                return script_function == this;
            });
        }
    }
}
