#include <Unreal/UStruct.hpp>
#include <Unreal/FField.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UFunction.hpp>
#include <Unreal/FProperty.hpp>

namespace RC::Unreal
{
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UStruct);

    using MemberOffsets = ::RC::Unreal::StaticOffsetFinder::MemberOffsets;

    auto UStruct::get_children() -> UField*
    {
        return Helper::Casting::offset_deref<UField*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::XField_Children));
    }

    auto UStruct::get_child_properties() -> FField*
    {
        if (Version::is_below(4, 25))
        {
            throw std::runtime_error("UStruct::ChildProperties is not available before UE 4.25");
        }
        return Helper::Casting::offset_deref<FField*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::XField_ChildProperties));
    }

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

    auto UStruct::for_each_function(const std::function<LoopAction(UFunction*)>& callable) -> void
    {
        UField* current_field = get_children();

        while (current_field != nullptr) {
            //Only trigger the callable on UFunction objects
            if (UFunction* function = cast_object<UFunction>(current_field)) {
                LoopAction loop_action = callable(function);

                if (loop_action == LoopAction::Break) {
                    break;
                }
            }
            current_field = current_field->get_next_ufield();
        }
    }

    auto UStruct::for_each_property(const std::function<LoopAction(FProperty* property)>& callable) -> void
    {
        if (Version::is_below(4, 25))
        {
            UField* current_field = get_children();

            while (current_field != nullptr) {
                FField* current_field_as_ffield = current_field->as_ffield_unsafe();

                //Only trigger the callable on UProperty objects
                if (FProperty* property = CastField<FProperty>(current_field_as_ffield)) {
                    LoopAction loop_action = callable(property);

                    if (loop_action == LoopAction::Break) {
                        break;
                    }
                }
                current_field = current_field->get_next_ufield();
            }
        }
        else
        {
            FField* current_field = get_child_properties();

            while (current_field != nullptr) {
                //Only trigger the callable on FProperty objects
                if (FProperty* property = CastField<FProperty>(current_field)) {
                    LoopAction loop_action = callable(property);

                    if (loop_action == LoopAction::Break) {
                        break;
                    }
                }
                current_field = current_field->GetNextFFieldUnsafe();
            }
        }
    }

    auto UStruct::for_each_super_struct(const ForEachSuperStructCallable& callable) -> void
    {
        UStruct* super_struct = get_super_struct();
        LoopAction loop_action{};

        while (super_struct)
        {
            loop_action = callable(super_struct);
            if (loop_action == LoopAction::Break) { break; }

            super_struct = super_struct->get_super_struct();
        }
    }

    auto UStruct::ForEachPropertyInChain(const ForEachPropertyInChainCallable& callable) -> void
    {
        UStruct* uclass = this;

        while (uclass)
        {
            bool should_outer_loop_break{};

            uclass->for_each_property([&](FProperty* child) {
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

                super_struct->for_each_property([&](FProperty* child) {
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

            UStruct* next_uclass = uclass->get_uclass();
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

    FProperty* UStruct::FindProperty(FName PropertyName)
    {
        FProperty* PropertyFound = nullptr;

        for_each_property([&](FProperty* Property) {
            if (Property->GetFName() == PropertyName)
            {
                PropertyFound = Property;
                return LoopAction::Break;
            }
            else
            {
                return LoopAction::Continue;
            }
        });

        return PropertyFound;
    }

    bool UStruct::HasChildren()
    {
        return get_child_properties() || get_children();
    }
}