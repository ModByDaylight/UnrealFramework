#include <Unreal/UStruct.hpp>
#include <Unreal/FField.hpp>
#include <Unreal/UClass.hpp>

namespace RC::Unreal
{
    auto UStruct::get_super_struct() -> UStruct*
    {
        return Helper::Casting::offset_deref<UStruct*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UStruct_SuperStruct));
    }

    auto UStruct::get_properties_size() -> int32_t
    {
        return Helper::Casting::offset_deref<int32_t>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UStruct_PropertiesSize));
    }

    auto UStruct::get_min_alignment() -> int32_t
    {
        return Helper::Casting::offset_deref<int32_t>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UStruct_MinAlignment));
    }

    auto UStruct::is_child_of(UStruct* a_struct) -> bool
    {
        UStruct* current_struct = this;
        do {
            if (current_struct == a_struct)
            {
                return true;
            }
            current_struct = current_struct->get_super_struct();

        }
        while (current_struct);
        return false;
    }

    auto UStruct::for_each_super_struct(const ForEachSuperStructCallable& callable) -> LoopAction
    {
        UStruct* super_struct = get_super_struct();
        LoopAction loop_action{};

        while (super_struct)
        {
            loop_action = callable(super_struct);
            if (loop_action == LoopAction::Break) { break; }

            super_struct = super_struct->get_super_struct();
        }

        return loop_action;
    }

    template<typename ForEachType, typename ForEachCallable, typename Callable>
    auto static for_each_x_in_chain_internal(UStruct* ustruct, ForEachCallable for_each_x, Callable callable) -> LoopAction
    {
        LoopAction loop_action{};

        loop_action = (ustruct->*for_each_x)([&](ForEachType* obj) {
            if (callable(obj) == LoopAction::Break)
            {
                loop_action = LoopAction::Break;
                return loop_action;
            }
            else
            {
                return LoopAction::Continue;
            }
        });
        if (loop_action == LoopAction::Break) { return loop_action; }

        loop_action = ustruct->for_each_super_struct([&](UStruct* super_struct) {
            if (!super_struct) { return LoopAction::Continue; }

            loop_action = (super_struct->*for_each_x)([&](ForEachType* obj) {
                if (callable(obj) == LoopAction::Break)
                {
                    loop_action = LoopAction::Break;
                    return loop_action;
                }
                else
                {
                    return LoopAction::Continue;
                }
            });

            if (loop_action == LoopAction::Break)
            {
                return LoopAction::Break;
            }
            else
            {
                return LoopAction::Continue;
            }
        });

        return loop_action;
    }

    auto UStruct::for_each_function_in_chain(ForEachChildInChainCallable<UFunction*> callable) -> LoopAction
    {
        return for_each_x_in_chain_internal<UFunction>(this, &UStruct::for_each_function, callable);
    }

    auto UStruct::for_each_property_in_chain(ForEachChildInChainCallable<XProperty*> callable) -> LoopAction
    {
        return for_each_x_in_chain_internal<XProperty>(this, &UStruct::for_each_property, callable);
    }
}