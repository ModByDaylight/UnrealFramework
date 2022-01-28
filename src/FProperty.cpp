#include <Unreal/FProperty.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/Property/XArrayProperty.hpp>
#include <Unreal/Property/XObjectProperty.hpp>
#include <Unreal/Property/XClassProperty.hpp>
#include <Unreal/Property/XEnumProperty.hpp>
#include <Unreal/Property/XBoolProperty.hpp>
#include <Unreal/Property/XStructProperty.hpp>
#include <Unreal/Property/XMulticastInlineDelegateProperty.hpp>
#include <Unreal/Property/XMulticastSparseDelegateProperty.hpp>
#include <Unreal/Property/XWeakObjectProperty.hpp>
#include <Unreal/Property/XMapProperty.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/UEnum.hpp>
#include <Unreal/UFunction.hpp>
#include <Unreal/UnrealVersion.hpp>
#include <Helpers/Format.hpp>
#include "Unreal/Memory/FMemory.hpp"

namespace RC::Unreal
{
    IMPLEMENT_PROPERTY_CLASS(FProperty);

    using MemberOffsets = ::RC::Unreal::StaticOffsetFinder::MemberOffsets;

    auto FProperty::get_offset_for_internal() -> int32_t
    {
        return Helper::Casting::offset_deref<int32_t>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::XProperty_Offset_Internal));
    }

    auto FProperty::get_rep_notify_func() -> FName
    {
        return Helper::Casting::offset_deref<FName>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::XProperty_Offset_Internal) + 4);
    }

    auto FProperty::get_array_dim() -> int32_t
    {
        return Helper::Casting::offset_deref<int32_t>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::Property_PropertyFlags) - 8);
    }

    auto FProperty::get_element_size() -> int32_t
    {
        return Helper::Casting::offset_deref<int32_t>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::Property_PropertyFlags) - 4);
    }

    auto FProperty::get_property_flags() -> EPropertyFlags
    {
        return Helper::Casting::offset_deref<EPropertyFlags>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::Property_PropertyFlags));
    }

    auto FProperty::initialize_value_implementation(void* dest) -> void { throw std::runtime_error("initialize_value not implemented, and the property is not CPF_ZeroConstructor"); }
    auto FProperty::destroy_value_implementation(void *value) -> void { throw std::runtime_error("destroy_value not implemented"); }
    auto FProperty::copy_value_implementation(void *dest, const void *src) -> void { throw std::runtime_error("copy_value not implemented"); }
    auto FProperty::clear_value_implementation(void *value) -> void { throw std::runtime_error("clear_value not implemented"); }
    auto FProperty::identical_implementation(const void *A, const void *B, uint32_t port_flags) -> bool { throw std::runtime_error("identical not implemented"); }
    auto FProperty::export_text_implementation(std::wstring &value_string, const void *property_value, const void *default_value, UObject *parent, int32_t port_flags) -> void { throw std::runtime_error("export_text not implemented"); }
    auto FProperty::import_text_implementation(const wchar_t *buffer, void *data, int32_t port_flags, UObject *owner_object) -> const wchar_t * { throw std::runtime_error("import_text not implemented"); }

    auto FProperty::get_property_value_int64_implementation(void *value) -> int64_t { throw std::runtime_error("is_floating_point can only be used on FNumericProperty"); }
    auto FProperty::get_property_value_double_implementation(void *value) -> double { throw std::runtime_error("is_floating_point can only be used on FNumericProperty"); }
    auto FProperty::is_floating_point_implementation() -> bool { throw std::runtime_error("is_floating_point can only be used on FNumericProperty"); }
    auto FProperty::get_property_value_object_implementation(void *value) -> UObject * { throw std::runtime_error("is_floating_point can only be used on FObjectPropertyBase"); }

    auto FProperty::initialize_value(void* dest) -> void
    {
        if (get_property_flags() & CPF_ZeroConstructor)
        {
            FMemory::memzero(dest, get_size());
        }
        else
        {
            get_dispatch_table().initialize_value(this, dest);
        }
    }

    auto FProperty::destroy_value(void* value) -> void
    {
        if (!(get_property_flags() & CPF_NoDestructor))
        {
            get_dispatch_table().destroy_value(this, value);
        }
    }

    auto FProperty::clear_value(void* value) -> void
    {
        if (has_all_property_flags(CPF_ZeroConstructor | CPF_NoDestructor))
        {
            FMemory::memzero(value, get_element_size());
        }
        else
        {
            get_dispatch_table().clear_value(this, value);
        }
    }
}
