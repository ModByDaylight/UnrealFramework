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

namespace RC::Unreal
{
    FFieldClassVariant FProperty::m_static_obj{(FFieldClass*) nullptr};

    auto FProperty::static_class() -> FFieldClassVariant {
        if (!m_static_obj.is_valid())
        {
            throw std::runtime_error{"[FProperty::static_class] m_static_obj_variant is not valid"};
        }
        if (m_static_obj.is_uobject())
        {
            return m_static_obj.to_uobject();
        }
        else
        {
            return m_static_obj.to_field();
        }
    }

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

    auto FProperty::get_path_name(const void* stop_outer) -> std::wstring
    {
        std::wstring out_path;
        size_t next_field{0};
        std::array<FName, 16> fields;

        for (FFieldVariant owner_variant = get_owner_variant(); owner_variant.is_valid(); owner_variant = owner_variant.get_owner_variant())
        {
            if (!owner_variant.is_uobject())
            {
                FField* owner = owner_variant.to_field();
                fields[next_field] = owner->get_fname();
            }
            else
            {
                UObject* owner = owner_variant.to_uobject();
                UObjectBaseUtility::get_path_name_recursive(owner, stop_outer, out_path);
                out_path.append(STR(":"));
                break;
            }
        }

        for (size_t field_index{0}; field_index < next_field; ++field_index)
        {
            out_path.append(fields[field_index].to_string() + L".");
        }
        out_path.append(get_fname().to_string());
        return out_path;
    }
}
