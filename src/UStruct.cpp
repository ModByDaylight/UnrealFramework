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
                if (FProperty* property = cast_field<FProperty>(current_field_as_ffield)) {
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
                if (FProperty* property = cast_field<FProperty>(current_field)) {
                    LoopAction loop_action = callable(property);

                    if (loop_action == LoopAction::Break) {
                        break;
                    }
                }
                current_field = current_field->get_next_ffield_unsafe();
            }
        }
    }
}