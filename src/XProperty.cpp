#include <Unreal/XProperty.hpp>
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
    auto XPropertyType::as_string() -> std::wstring
    {
        FName type = m_property->get_type_fname();

        if (type == TypeChecker::m_structproperty_name)
        {
            return std::format(STR("F{}"), static_cast<XStructProperty*>(m_property)->get_script_struct()->get_name());
        }
        else if (type == TypeChecker::m_boolproperty_name)
        {
            XBoolProperty* typed_property = static_cast<XBoolProperty*>(m_property);
            if (typed_property->get_byte_mask() == 1 && typed_property->get_field_mask() == 255)
            {
                return STR("bool");
            }
            else
            {
                return STR("uint8");
            }
        }
        else if (type == TypeChecker::m_floatproperty_name)
        {
            return STR("float");
        }
        else if (type == TypeChecker::m_int8property_name)
        {
            return STR("int16");
        }
        else if (type == TypeChecker::m_int8property_name)
        {
            return STR("int16");
        }
        else if (type == TypeChecker::m_intproperty_name)
        {
            return STR("int32");
        }
        else if (type == TypeChecker::m_int64property_name)
        {
            return STR("int64");
        }
        else if (type == TypeChecker::m_byteproperty_name)
        {
            // TODO: Deal with enum
            return STR("uint8");
        }
        else if (type == TypeChecker::m_uint16property_name)
        {
            return STR("uint16");
        }
        else if (type == TypeChecker::m_uint32property_name)
        {
            return STR("uint32");
        }
        else if (type == TypeChecker::m_strproperty_name)
        {
            return STR("FString");
        }
        else if (type == TypeChecker::m_textproperty_name)
        {
            return STR("FText");
        }
        else if (type == TypeChecker::m_objectproperty_name)
        {
            UClass* property_class = static_cast<XObjectProperty*>(m_property)->get_property_class();
            return std::format(STR("{}{}*"), property_class->get_name_prefix(), property_class->get_fname().to_string());
        }
        else if (type == TypeChecker::m_classproperty_name)
        {
            UClass* meta_class = static_cast<XClassProperty*>(m_property)->get_meta_class();
            return std::format(STR("TSubclassOf<{}{}>"), meta_class->get_name_prefix(), meta_class->get_fname().to_string());
        }
        else if (type == TypeChecker::m_arrayproperty_name)
        {
            return std::format(STR("TArray<{}>"), XArrayProperty::get_inner(static_cast<XArrayProperty*>(m_property))->get_type().as_string());
        }
        else if (type == TypeChecker::m_nameproperty_name)
        {
            return STR("FName");
        }
        else if (type == TypeChecker::m_enumproperty_name)
        {
            return static_cast<XEnumProperty<uint8_t>*>(m_property)->get_unreal_enum()->get_name();
        }
        else if (type == TypeChecker::m_scriptdelegateproperty_name)
        {
            return static_cast<XDelegateProperty*>(m_property)->get_function_signature()->get_name();
        }
        else if (type == TypeChecker::m_multicastinlinescriptdelegateproperty_name)
        {
            return static_cast<XMulticastInlineDelegateProperty*>(m_property)->get_function_signature()->get_name();
        }
        else if (type == TypeChecker::m_multicastsparsescriptdelegateproperty_name)
        {
            return static_cast<XMulticastSparseDelegateProperty*>(m_property)->get_function_signature()->get_name();
        }
        else if (type == TypeChecker::m_weakobjectproperty_name)
        {
            return std::format(STR("TWeakObjectPtr<{}>"), static_cast<XWeakObjectProperty*>(m_property)->get_property_class()->get_name());
        }
        else if (type == TypeChecker::m_mapproperty_name)
        {
            XMapProperty* map_property = static_cast<XMapProperty*>(m_property);
            XProperty* key_property = map_property->get_key_property();
            XProperty* value_property = map_property->get_value_property();
            return std::format(STR("TMap<{}, {}>"), key_property->get_type().as_string(), value_property->get_type().as_string());
        }
        else
        {
            return type.to_string();
        }
    }

    auto XPropertyType::as_fname() -> FName
    {
        return m_property->get_ffieldclass()->get_fname();
    }

    auto XProperty::get_type() -> XPropertyType
    {
        return XPropertyType{this};
    }

    auto XProperty::get_offset_for_internal() -> int32_t
    {
        return Helper::Casting::offset_deref<int32_t>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::XProperty_Offset_Internal));
    }

    auto XProperty::get_rep_notify_func() -> FName
    {
        return Helper::Casting::offset_deref<FName>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::XProperty_Offset_Internal) + 4);
    }

    auto XProperty::get_size() -> int32_t
    {
        FName type = get_type().as_fname();

        // TODO: Add more types here
        if (type == TypeChecker::m_boolproperty_name)
            return 0x1;
        else if (type == TypeChecker::m_byteproperty_name)
            return 0x1;
        else if (type == TypeChecker::m_structproperty_name)
        {
            XStructProperty* inner_property = static_cast<XStructProperty*>(this);
            return inner_property->get_script_struct()->get_size();
        }
        else if (type == TypeChecker::m_intproperty_name)
            return 0x4;
        else if (type == TypeChecker::m_floatproperty_name)
            return 0x4;
        else if (type == TypeChecker::m_objectproperty_name)
            return 0x8;
        else if (type == TypeChecker::m_classproperty_name)
            return 0x8;
        else if (type == TypeChecker::m_strproperty_name)
            return 0x10;
        else if (type == TypeChecker::m_enumproperty_name)
            // TODO: An enum can be larger than 1 byte but for now this will work
            // Could maybe use UEnum::ECppForm
            return 0x1;
        else if (type == TypeChecker::m_arrayproperty_name)
            return 0x10;
        else if (type == TypeChecker::m_nameproperty_name)
            return 0x8;
        else if (type == TypeChecker::m_scriptdelegateproperty_name)
            return sizeof(TScriptDelegate);
        else if (type == TypeChecker::m_multicastinlinescriptdelegateproperty_name)
            return sizeof(TMulticastScriptDelegate);
        else if (type == TypeChecker::m_multicastsparsescriptdelegateproperty_name)
            return sizeof(FSparseDelegate);
        else if (type == TypeChecker::m_weakobjectproperty_name)
            return sizeof(FWeakObjectPtr);
        else
            return 0x4;
    }

    auto XProperty::get_array_dim() -> int32_t
    {
        return Helper::Casting::offset_deref<int32_t>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::Property_PropertyFlags) - 8);
    }

    auto XProperty::get_element_size() -> int32_t
    {
        return Helper::Casting::offset_deref<int32_t>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::Property_PropertyFlags) - 4);
    }

    auto XProperty::get_property_flags() -> EPropertyFlags
    {
        return Helper::Casting::offset_deref<EPropertyFlags>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::Property_PropertyFlags));
    }

    auto XProperty::has_any_property_flags(uint64_t flags_to_check) -> bool
    {
        return (get_property_flags() & flags_to_check) != 0 || flags_to_check == CPF_AllFlags;
    }

    auto XProperty::has_all_property_flags(uint64_t flags_to_check) -> bool
    {
        return ((get_property_flags() & flags_to_check) == flags_to_check);
    }

    auto XProperty::get_full_name(const void* stop_outer) -> std::wstring
    {
        std::wstring name;
        name.reserve(800);

        if (Version::is_below(4, 25) || !TypeChecker::is_ffield(this))
        {
            // We've verified that we're in <4.25 so this cast is safe but should be abstracted at some point
            UObjectBaseUtility::get_full_name(std::bit_cast<UObject*>(this), name, stop_outer);
            return name;
        }
        else
        {
            get_full_name(this, name, stop_outer);
            return name;
        }
    }

    auto XProperty::get_full_name(std::wstring& out_name, const void* stop_outer) -> void
    {
        if (Version::is_below(4, 25))
        {
            // We've verified that we're in <4.25 so this cast is safe but should be abstracted at some point
            UObjectBaseUtility::get_full_name(std::bit_cast<UObject*>(this), out_name, stop_outer);
        }
        else
        {
            get_full_name(this, out_name, stop_outer);
        }
    }

    auto XProperty::get_path_name_recursive(const XProperty* property, const void* stop_outer, std::wstring& out_path) -> void
    {
        size_t next_field{0};
        std::array<FName, 16> fields;

        // 4.25+ only!
        for (FFieldVariant owner_variant = property->get_owner_variant(); owner_variant.is_valid(); owner_variant = owner_variant.get_owner_variant())
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
        out_path.append(property->get_fname().to_string());
    }

    auto XProperty::get_path_name(const XProperty* property, const void* stop_outer, std::wstring& out_path) -> void
    {
        if (Version::is_below(4, 25))
        {
            // The reinterpret_cast should be safe here because in <4.25, the 'Property' type inherits from UObject
            UObjectBaseUtility::get_path_name(std::bit_cast<UObject*>(property), stop_outer, out_path);
        }
        else
        {
            std::wstring path;
            path.reserve(200);

            get_path_name_recursive(property, stop_outer, path);
            out_path.append(path);
        }
    }

    auto XProperty::get_full_name(const XProperty* property, std::wstring& out_name, const void* stop_outer) -> void
    {
        if (Version::is_below(4, 25))
        {
            // The reinterpret_cast should be safe here because in <4.25, the 'Property' type inherits from UObject
            UObjectBaseUtility::get_full_name(std::bit_cast<UObject*>(property), out_name, stop_outer);
        }
        else
        {
            out_name.append(property->get_ffieldclass()->get_fname().to_string() + L" ");
            get_path_name(property, stop_outer, out_name);
        }
    }

    auto XProperty::trivial_dump_to_string(void* p_this, std::wstring& out_line) -> void
    {
        XProperty* p_typed_this = static_cast<XProperty*>(p_this);

        out_line.append(std::format(L"[{:016X}] ", reinterpret_cast<uintptr_t>(p_this)));
        p_typed_this->get_full_name(out_line);
        out_line.append(std::format(L" [o: {:X}] ", p_typed_this->get_offset_for_internal()));

        if (Version::is_below(4, 25))
        {
            // We've verified that we're in <4.25 so this cast is safe but should be abstracted at some point
            UClass* property_class = static_cast<UObject*>(p_this)->get_uclass();
            out_line.append(std::format(L"[n: {:X}] [c: {:016X}]", p_typed_this->get_fname().get_comparison_index(), reinterpret_cast<uintptr_t>(property_class)));
        }
        else
        {
            out_line.append(std::format(L"[n: {:X}] [c: {:016X}] [owr: {:016X}]", p_typed_this->get_fname().get_comparison_index(), reinterpret_cast<uintptr_t>(p_typed_this->get_ffieldclass()), reinterpret_cast<uintptr_t>(p_typed_this->get_owner())));
        }
    }

    auto XProperty::to_string(void* p_this, std::wstring& out_line) -> void
    {
        trivial_dump_to_string(p_this, out_line);
    }
}
