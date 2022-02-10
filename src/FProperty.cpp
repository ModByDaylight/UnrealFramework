#include <Unreal/FProperty.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UFunction.hpp>
#include <Unreal/FMemory.hpp>
#include <Unreal/FString.hpp>
#include <Unreal/VersionedContainer/Container.hpp>
#include <Unreal/UnrealStringConversions.h>
#include <Unreal/UnrealParsing.h>

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FProperty);
    IMPLEMENT_VIRTUAL_FUNCTION(FProperty, copy_values_to_script_vm_internal);
    IMPLEMENT_VIRTUAL_FUNCTION(FProperty, copy_values_from_script_vm_internal);

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

    auto FProperty::serialize(FArchive& ar) -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty, Serialize, void, PARAMS(FArchive&), ARGS(ar))
    }

    auto FProperty::post_duplicate(const FField& in_field) -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty, PostDuplicate, void, PARAMS(const FField&), ARGS(in_field))
    }

    auto FProperty::get_cpp_macro_type(FString& extended_type_text) const -> FString
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty, GetCPPMacroType, FString, PARAMS(FString&), ARGS(extended_type_text))
    }

    auto FProperty::pass_cpp_args_by_ref() const -> bool
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FProperty, PassCPPArgsByRef, bool)
    }

    auto FProperty::get_cpp_type(FString* extended_type_text, uint32 cpp_export_flags) const -> FString
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty, GetCPPType, FString, PARAMS(FString*, uint32), ARGS(extended_type_text, cpp_export_flags))
    }

    auto FProperty::identical(const void* A, const void* B, uint32_t port_flags) -> bool
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty, Identical, bool, PARAMS(const void*, const void*, uint32_t), ARGS(A, B, port_flags))
    }

    auto FProperty::get_cpp_type_forward_declaration() const -> FString
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FProperty, GetCPPTypeForwardDeclaration, FString)
    }

    auto FProperty::convert_from_type(const FPropertyTag& tag, FStructuredArchive::FSlot slot, uint8* data, UStruct* defaults_struct) -> EConvertFromTypeResult
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty,
                                         ConvertFromType,
                                         EConvertFromTypeResult,
                                         PARAMS(const FPropertyTag&, FStructuredArchive::FSlot, uint8*, UStruct*),
                                         ARGS(tag, slot, data, defaults_struct))
    }

    auto FProperty::serialize_item(FStructuredArchive::FSlot slot, void* value, void const* defaults) const -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty,
                                         SerializeItem,
                                         void,
                                         PARAMS(FStructuredArchive::FSlot, void*, void const*),
                                         ARGS(slot, value, defaults))
    }

    auto FProperty::net_serialize_item(FArchive& ar, UPackageMap* map, void* data, TArray<uint8>* meta_data = nullptr) const -> bool
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty,
                                         NetSerializeItem,
                                         bool,
                                         PARAMS(FArchive&, UPackageMap*, void*, TArray<uint8>*),
                                         ARGS(ar, map, data, meta_data))
    }

    auto FProperty::supports_met_shared_serialization() const -> bool
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FProperty, SupportsNetSharedSerialization, bool)
    }

    auto FProperty::link_internal(FArchive& ar) -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty, LinkInternal, void, PARAMS(FArchive&), ARGS(ar))
    }

    auto FProperty::import_text(const TCHAR* buffer, void* data, int32_t port_flags, UObject* owner_object, FOutputDevice* error_text) -> const TCHAR* {
        //Do not allow property import on Config properties when it's restricted
        if ((port_flags & EPropertyPortFlags::PPF_RestrictImportTypes) && (get_property_flags() & CPF_Config))
        {
            return nullptr;
        }
        if (buffer == nullptr)
        {
            return nullptr;
        }

        //Imports should always process deprecated properties
        port_flags |= EPropertyPortFlags::PPF_UseDeprecatedProperties;

        return import_text_internal(buffer, data, port_flags, owner_object, error_text);
    }

    auto FProperty::import_text_internal(const TCHAR* buffer, void* data, int32 port_flags, UObject* owner_object, FOutputDevice* error_text) -> const TCHAR*
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty,
                                         ImportText_Internal,
                                         const TCHAR*,
                                         PARAMS(const TCHAR*, void*, int32, UObject*, FOutputDevice*),
                                         ARGS(buffer, data, port_flags, owner_object, error_text))
    }

    auto FProperty::export_text_item(FString& value_str, const void* property_value, const void* default_value, UObject* parent, int32_t port_flags, UObject* export_root_scope) -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty,
                                         ExportTextItem,
                                         void,
                                         PARAMS(FString&, const void*, const void*, UObject*, int32_t, UObject*),
                                         ARGS(value_str, property_value, default_value, parent, port_flags, export_root_scope))
    }

    auto FProperty::get_value_type_hash(const void* src) -> uint32_t
    {
        if ((get_property_flags() & CPF_HasGetValueTypeHash) == 0)
        {
            throw std::runtime_error("This property does not have the GetValueTypeHash implementation");
        }
        return get_value_type_hash_internal(src);
    }

    auto FProperty::get_value_type_hash_internal(const void* src) const -> uint32
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty, GetValueTypeHashInternal, uint32, PARAMS(const void*), ARGS(src))
    }

    auto FProperty::copy_single_value_to_script_vm(void* dest, void const* src) const -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty, CopySingleValueToScriptVM, void, PARAMS(void*, void const*), ARGS(dest, src))
    }

    auto FProperty::copy_complete_value_to_script_vm(void* dest, void const* src) const -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty, CopyCompleteValueToScriptVM, void, PARAMS(void*, void const*), ARGS(dest, src))
    }

    auto FProperty::copy_single_value_from_script_vm(void* dest, void const* src) const -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty, CopySingleValueFromScriptVM, void, PARAMS(void*, void const*), ARGS(dest, src))
    }

    auto FProperty::copy_complete_value_from_script_vm(void* dest, void const* src) const -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty, CopyCompleteValueFromScriptVM, void, PARAMS(void*, void const*), ARGS(dest, src))
    }

    auto FProperty::copy_values_internal(void* dest, const void* src, int32_t count) -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty, CopyValuesInternal, void, PARAMS(void*, const void*, int32_t), ARGS(dest, src, count))
    }

    auto FProperty::clear_value(void* data) -> void
    {
        if (has_all_property_flags(CPF_NoDestructor | CPF_ZeroConstructor))
        {
            FMemory::memzero(data, get_element_size());
        }
        else
        {
            clear_value_internal(data);
        }
    }

    auto FProperty::clear_value_internal(void* data) const -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty, ClearValueInternal, void, PARAMS(void*), ARGS(data))
    }

    auto FProperty::destroy_value(void* dest) -> void
    {
        if (!(get_property_flags() & CPF_NoDestructor))
        {
            destroy_value_internal(dest);
        }
    }

    auto FProperty::destroy_value_internal(void* dest) const -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty, DestroyValueInternal, void, PARAMS(void*), ARGS(dest))
    }

    auto FProperty::initialize_value(void* dest) -> void
    {
        if (get_property_flags() & CPF_ZeroConstructor)
        {
            FMemory::memzero(dest, get_element_size() * get_array_dim());
        }
        else
        {
            initialize_value_internal(dest);
        }
    }

    auto FProperty::initialize_value_internal(void* dest) const -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty, InitializeValueInternal, void, PARAMS(void*), ARGS(dest))
    }

    auto FProperty::get_id() const -> FName
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FProperty, GetID, FName)
    }

    auto FProperty::instance_subobjects(void* data, void const* default_data, UObject* owner, FObjectInstancingGraph* instance_graph) -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty,
                                         InstanceSubobjects,
                                         void,
                                         PARAMS(void*, void const*, UObject*, FObjectInstancingGraph*),
                                         ARGS(data, default_data, owner, instance_graph))
    }

    auto FProperty::get_min_alignment() const -> int32
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FProperty, GetMinAlignment, int32)
    }

    auto FProperty::contains_object_reference(TArray<const class FStructProperty*>& encountered_struct_props, EPropertyObjectReferenceType in_reference_type = EPropertyObjectReferenceType::Strong) const -> bool
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty,
                                         ContainsObjectReference,
                                         bool,
                                         PARAMS(TArray<const class FStructProperty*> &, EPropertyObjectReferenceType),
                                         ARGS(encountered_struct_props, in_reference_type))
    }

    auto FProperty::emit_reference_info(UClass& owner_class, int32 base_offset, TArray<const FStructProperty*>& encountered_struct_props) -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty,
                                         EmitReferenceInfo,
                                         void,
                                         PARAMS(UClass&, int32, TArray<const FStructProperty*>&),
                                         ARGS(owner_class, base_offset, encountered_struct_props))
    }

    auto FProperty::same_type(const FProperty* other) const -> bool
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FProperty, SameType, bool, PARAMS(const FProperty*), ARGS(other))
    }

    auto FProperty::copy_values_to_script_vm_internal(void *dest, const void *src, int32_t count) -> void
    {
        return CALL_VIRTUAL_FUNCTION(this, copy_values_to_script_vm_internal, dest, src, count);
    }

    auto FProperty::copy_values_from_script_vm_internal(void *dest, const void *src, int32_t count) -> void
    {
        return CALL_VIRTUAL_FUNCTION(this, copy_values_from_script_vm_internal, dest, src, count);
    }

    auto FProperty::copy_values_to_script_vm_internal_impl(void *dest, const void *src, int32_t count) -> void
    {
        copy_values_internal(dest, src, count);
    }

    auto FProperty::copy_values_from_script_vm_internal_impl(void *dest, const void *src, int32_t count) -> void
    {
        copy_values_internal(dest, src, count);
    }

    /*-----------------------------------------------------------------------------
	Helpers.
-----------------------------------------------------------------------------*/

    static std::wstring alpha_numeric_chars = STR("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

    FORCEINLINE bool is_valid_token_start(wchar_t first_char, bool dotted_names)
    {
        return alpha_numeric_chars.find(first_char) != std::wstring::npos || (dotted_names && first_char == '/') || first_char > 255;
    }

    FORCEINLINE std::wstring parse_property_token(const wchar_t* str, bool dotted_names)
    {
        std::wstring result_string;
        while (*str)
        {
            const wchar_t character = *str++;
            if (alpha_numeric_chars.find(character) != std::wstring::npos ||
                    (character == '_' || character == '-' || character == '+') ||
                    dotted_names && (character == '.' || character == '/' || character == SUBOBJECT_DELIMITER_CHAR)) {
                result_string.push_back(character);
                continue;
            }
            break;
        }
        return result_string;
    }

    const TCHAR* FPropertyHelpers::read_token(const TCHAR* buffer, std::wstring& string, bool dotted_names)
    {
        if(*buffer == wchar_t('"'))
        {
            int32_t num_chars_read = 0;
            if (!FParse::quoted_string(buffer, string, &num_chars_read))
            {
                return nullptr;
            }
            buffer += num_chars_read;
        }
        else if (is_valid_token_start(*buffer, dotted_names))
        {
            std::wstring token = parse_property_token(buffer, dotted_names);
            string += token;
            buffer += token.size();
        }
        else
        {
            // Get just one.
            string += *buffer;
        }
        return buffer;
    }
}
