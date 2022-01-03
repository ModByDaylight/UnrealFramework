#include <Unreal/FField.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UFunction.hpp>
#include <Unreal/XProperty.hpp>

namespace RC::Unreal
{
    auto FField::get_ffieldclass() const -> FFieldClass*
    {
        return Helper::Casting::offset_deref<FFieldClass*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::FField_Class));
    }

    auto FField::get_fname() -> FName
    {
        return Helper::Casting::offset_deref<FName>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::FField_NamePrivate));
    }

    auto FField::get_fname() const -> FName
    {
        return Helper::Casting::offset_deref<FName>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::FField_NamePrivate));
    }

    auto FField::get_name() -> File::StringType
    {
        return get_fname().to_string();
    }

    auto FField::get_name() const -> File::StringType
    {
        return get_fname().to_string();
    }

    auto FField::get_type_fname() -> FName
    {
        return Unreal::Container::m_unreal_vc_base->Field_get_type_fname(this);
    }

    auto FField::get_type_name() -> std::wstring
    {
        return get_type_fname().to_string();
    }

    auto FField::is_child_of(FFieldClassVariant field_class_variant) -> bool
    {
        if (field_class_variant.is_uobject())
        {
            UObject* property_as_object = std::bit_cast<UObject*>(this);
            return property_as_object->get_uclass()->is_child_of(static_cast<UStruct*>(field_class_variant.to_uobject()));
        }
        else
        {
            FFieldClass* field_class_to_compare = field_class_variant.to_field();
            FFieldClass* field_class = get_ffieldclass();
            while (field_class)
            {
                if (field_class == field_class_to_compare)
                {
                    return true;
                }
                field_class = field_class->super_class;
            }

            return false;
        }
    }

    auto FField::get_owner() -> UStruct*
    {
        return static_cast<UStruct*>(Container::m_unreal_vc_base->Field_get_ffield_owner(this));
    }

    auto FField::get_owner() const -> UStruct*
    {
        return static_cast<UStruct*>(Container::m_unreal_vc_base->Field_get_ffield_owner(this));
    }

    auto FField::get_owner_variant() -> FFieldVariant
    {
        return Helper::Casting::offset_deref<FFieldVariant>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::FField_Owner));
    }

    auto FField::get_owner_variant() const -> FFieldVariant
    {
        return Helper::Casting::offset_deref<FFieldVariant>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::FField_Owner));
    }

    auto FField::as_object() const -> const UField*
    {
        return std::bit_cast<const UField*>(this);
    }

    auto FField::as_object() -> UField*
    {
        return std::bit_cast<UField*>(this);
    }

    auto FFieldClass::get_fname() -> FName
    {
        return Container::m_unreal_vc_base->FFieldClass_get_fname(this);
    }
}
