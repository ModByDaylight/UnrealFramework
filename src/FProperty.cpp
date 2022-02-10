#include <Unreal/FProperty.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UFunction.hpp>
#include <Unreal/FMemory.hpp>
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

    auto FProperty::identical(const void* A, const void* B, uint32_t port_flags) -> bool
    {
        return CALL_VIRTUAL_FUNCTION(this, identical, A, B, port_flags);
    }

    auto FProperty::import_text(const wchar_t* buffer, void* data, int32_t port_flags, UObject* owner_object) -> const wchar_t* {
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

        return CALL_VIRTUAL_FUNCTION(this, import_text, buffer, data, port_flags, owner_object);
    }

    auto FProperty::export_text_item(std::wstring& value_str, const void* property_value, const void* default_value, UObject* parent, int32_t port_flags, UObject* export_root_scope) -> void
    {
        return CALL_VIRTUAL_FUNCTION(this, export_text_item, value_str, property_value, default_value, parent, port_flags, export_root_scope);
    }

    auto FProperty::get_value_type_hash(const void* src) -> uint32_t
    {
        if ((get_property_flags() & CPF_HasGetValueTypeHash) == 0)
        {
            throw std::runtime_error("This property does not have the GetValueTypeHash implementation");
        }
        return CALL_VIRTUAL_FUNCTION(this, get_value_type_hash, src);
    }

    auto FProperty::copy_values_internal(void* dest, const void* src, int32_t count) -> void
    {
        if(dest != src)
        {
            if (get_property_flags() & CPF_IsPlainOldData)
            {
                FMemory::memcpy(dest, src, get_element_size() * count);
            }
            else
            {
                CALL_VIRTUAL_FUNCTION(this, copy_values_internal, dest, src, count);
            }
        }
    }

    auto FProperty::clear_value(void* data) -> void
    {
        if (has_all_property_flags(CPF_NoDestructor | CPF_ZeroConstructor))
        {
            FMemory::memzero(data, get_element_size());
        }
        else
        {
            CALL_VIRTUAL_FUNCTION(this, clear_value, data);
        }
    }

    auto FProperty::destroy_value(void* dest) -> void
    {
        if (!(get_property_flags() & CPF_NoDestructor))
        {
            CALL_VIRTUAL_FUNCTION(this, destroy_value, dest);
        }
    }

    auto FProperty::initialize_value(void* dest) -> void
    {
        if (get_property_flags() & CPF_ZeroConstructor)
        {
            FMemory::memzero(dest, get_element_size() * get_array_dim());
        }
        else
        {
            CALL_VIRTUAL_FUNCTION(this, initialize_value, dest);
        }
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
