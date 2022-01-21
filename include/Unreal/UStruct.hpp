#ifndef RC_UNREAL_USTRUCT_HPP
#define RC_UNREAL_USTRUCT_HPP

#include <cstdint>
#include <stdexcept>
#include <functional>

#include <Constructs/Loop.hpp>

#include <Unreal/StaticOffsetFinder.hpp>
#include <Unreal/FField.hpp>
#include <Unreal/UField.hpp>

namespace RC::Unreal
{
    class RC_UE_API UStruct : public UField
    {
    private:
        static inline class UClass* m_static_obj{};

    public:
        // DO NOT USE... MEANT FOR INTERNAL USE ONLY
        auto static set_static_obj_ptr(class UClass* ptr)
        {
            m_static_obj = ptr;
        }

        auto static static_class() -> class UClass*
        {
            if (!m_static_obj) { throw std::runtime_error{"[UStructImpl::get_static_obj_ptr] m_static_obj is nullptr"}; }

            return m_static_obj;
        }

    public:
        auto get_super_struct() -> UStruct*;
        auto get_properties_size() -> int32_t;
        auto get_min_alignment() -> int32_t;
        auto is_child_of(UStruct* a_struct) -> bool;

        template<UObjectGlobals::UObjectDerivative UObjectDerivedType>
        auto is_child_of() -> bool
        {
            return is_child_of(UObjectDerivedType::static_class());
        }

        using ForEachSuperStructCallable = std::function<LoopAction(UStruct*)>;
        auto for_each_super_struct(const ForEachSuperStructCallable& callable) -> LoopAction;

        template<typename TypeParam1 = FField*>
        using ForEachChildInChainCallable = std::function<LoopAction(TypeParam1)>;

        template<typename ObjectType>
        auto for_each_child_in_chain(ForEachChildInChainCallable<ObjectType> callable) -> void
        {
            UStruct* uclass = this;

            while (uclass)
            {
                bool should_outer_loop_break{};

                uclass->for_each_property([&](ObjectType child) {
                    if (callable(child) == LoopAction::Break)
                    {
                        should_outer_loop_break = true;
                        return LoopAction::Break;
                    }
                    else
                    {
                        return LoopAction::Continue;
                    }
                });

                if (should_outer_loop_break) { break; }

                uclass->for_each_super_struct([&](UStruct* super_struct) {
                    if (!super_struct) { return LoopAction::Continue; }

                    super_struct->for_each_property([&](ObjectType child) {
                        auto loop_action = callable(child);
                        if (loop_action == LoopAction::Break)
                        {
                            should_outer_loop_break = true;
                            return LoopAction::Break;
                        }
                        else
                        {
                            return LoopAction::Continue;
                        }
                    });

                    if (should_outer_loop_break) { return LoopAction::Break; }

                    return LoopAction::Continue;
                });

                if (should_outer_loop_break) { break; }

                // TODO: Fix this! I think XField must be separated out into UField and FField
                UStruct* next_uclass = reinterpret_cast<UStruct*>(uclass->get_uclass());
                if (next_uclass != uclass)
                {
                    uclass = next_uclass;
                }
                else
                {
                    break;
                }
            }
        }

        auto for_each_function_in_chain(ForEachChildInChainCallable<UFunction*> callable) -> LoopAction;
        auto for_each_property_in_chain(ForEachChildInChainCallable<XProperty*> callable) -> LoopAction;
    };
}

#endif //RC_UNREAL_USTRUCT_HPP
