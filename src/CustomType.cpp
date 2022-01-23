#include <Unreal/CustomType.hpp>
#include <Unreal/StaticOffsetFinder.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/UnrealVersion.hpp>

namespace RC::Unreal
{
    template<typename ValueType>
    auto static set_member_variable(const StaticOffsetFinder::MemberOffsets member, CustomProperty* property, ValueType new_value) -> void
    {
        ValueType* data_offset_internal = reinterpret_cast<ValueType*>(&property->m_data[StaticOffsetFinder::retrieve_static_offset(member)]);
        *data_offset_internal = new_value;
    }

    CustomProperty::CustomProperty(int32_t offset_internal)
    {
        set_member_variable(MemberOffsets::XProperty_Offset_Internal, this, offset_internal);
    }

    auto CustomProperty::construct(int32_t offset_internal, UClass* belongs_to_class, UClass* inner_class) -> std::unique_ptr<CustomProperty>
    {
        std::unique_ptr<CustomProperty> custom_property = std::make_unique<CustomProperty>(offset_internal);

        set_member_variable(MemberOffsets::FField_Class, custom_property.get(), inner_class);

        if (Version::is_atleast(4, 25))
        {
            set_member_variable(MemberOffsets::FField_Owner, custom_property.get(), belongs_to_class);
        }
        else
        {
            set_member_variable(MemberOffsets::UObject_OuterPrivate, custom_property.get(), belongs_to_class);
        }

        return custom_property;
    }

    CustomArrayProperty::CustomArrayProperty(int32_t offset_internal) : CustomProperty(offset_internal) {}

    auto CustomArrayProperty::construct(int32_t offset_internal, FProperty* array_inner) -> std::unique_ptr<CustomProperty>
    {
        std::unique_ptr<CustomArrayProperty> custom_array_property = std::make_unique<CustomArrayProperty>(offset_internal);

        set_member_variable(MemberOffsets::ArrayProperty_Inner, custom_array_property.get(), array_inner);

        return custom_array_property;
    }

    auto CustomArrayProperty::construct(int32_t offset_internal, UClass* belongs_to_class, UClass* inner_class, FProperty* array_inner) -> std::unique_ptr<CustomProperty>
    {
        // Create dummy property to act as the 'Inner' member variable for the ArrayProperty
        // Then we set the inner type for the array property as the 'FFieldClass' of this dummy property
        CustomProperty* array_inner_property = new CustomProperty{0x0};
        set_member_variable(MemberOffsets::FField_Class, array_inner_property, array_inner);

        std::unique_ptr<CustomProperty> custom_array_property = CustomProperty::construct(offset_internal, belongs_to_class, inner_class);
        set_member_variable(MemberOffsets::ArrayProperty_Inner, custom_array_property.get(), array_inner_property);

        return custom_array_property;
    }

    CustomStructProperty::CustomStructProperty(int32_t offset_internal) : CustomProperty(offset_internal) {}

    auto CustomStructProperty::construct(int32_t offset_internal, UScriptStruct* script_struct) -> std::unique_ptr<CustomProperty>
    {
        std::unique_ptr<CustomStructProperty> custom_struct_property = std::make_unique<CustomStructProperty>(offset_internal);

        set_member_variable(MemberOffsets::StructProperty_Struct, custom_struct_property.get(), script_struct);

        return custom_struct_property;
    }
}