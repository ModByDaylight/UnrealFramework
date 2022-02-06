#include "Unreal/Property/FNumericProperty.h"
#include "Unreal/UEnum.hpp"

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FNumericProperty);
    IMPLEMENT_VIRTUAL_FUNCTION(FNumericProperty, import_text);
    IMPLEMENT_VIRTUAL_FUNCTION(FNumericProperty, export_text_item);

    auto FNumericProperty::is_integer() -> bool
    {
        return CALL_VIRTUAL_FUNCTION(this, is_integer);
    }

    auto FNumericProperty::is_floating_point() -> bool
    {
        return CALL_VIRTUAL_FUNCTION(this, is_floating_point);
    }

    auto FNumericProperty::is_unsigned_integer() -> bool
    {
        return CALL_VIRTUAL_FUNCTION(this, is_unsigned_integer);
    }

    auto FNumericProperty::get_int_property_enum() -> UEnum*
    {
        return CALL_VIRTUAL_FUNCTION(this, get_int_property_enum);
    }

    auto FNumericProperty::set_unsigned_int_property_value(void* data, uint64_t value) -> void
    {
        CALL_VIRTUAL_FUNCTION(this, set_unsigned_int_property_value, data, value);
    }

    auto FNumericProperty::set_signed_int_property_value(void* data, int64_t value) -> void
    {
        CALL_VIRTUAL_FUNCTION(this, set_signed_int_property_value, data, value);
    }

    auto FNumericProperty::set_floating_point_property_value(void* data, double value) -> void
    {
        CALL_VIRTUAL_FUNCTION(this, set_floating_point_property_value, data, value);
    }

    auto FNumericProperty::set_numeric_property_value_from_string(void* data, const wchar_t* value) -> void
    {
        CALL_VIRTUAL_FUNCTION(this, set_numeric_property_value_from_string, data, value);
    }

    auto FNumericProperty::get_signed_int_property_value(const void *data) -> int64_t
    {
        return CALL_VIRTUAL_FUNCTION(this, get_signed_int_property_value, data);
    }

    auto FNumericProperty::get_unsigned_int_property_value(const void *data) -> uint64_t
    {
        return CALL_VIRTUAL_FUNCTION(this, get_unsigned_int_property_value, data);
    }

    auto FNumericProperty::get_floating_point_property_value(const void *data) -> double
    {
        return CALL_VIRTUAL_FUNCTION(this, get_floating_point_property_value, data);
    }

    auto FNumericProperty::get_numeric_property_value_to_string(const void *data) -> std::wstring
    {
        return CALL_VIRTUAL_FUNCTION(this, get_numeric_property_value_to_string, data);
    }

    auto FNumericProperty::export_text_item_impl(std::wstring& value_str, const void* property_value, const void* default_value, UObject* parent, int32_t port_flags, UObject* export_root_scope) -> void
    {
        value_str += get_numeric_property_value_to_string(property_value);
    }

    auto FNumericProperty::import_text_impl(const wchar_t* buffer, void* data, int32_t port_flags, UObject* owner_object) -> const wchar_t*
    {
        if (buffer != nullptr)
        {
            const wchar_t* start = buffer;

            if (is_integer())
            {
                if (isalpha(*buffer))
                {
                    UEnum* property_enum = get_int_property_enum();
                    if (property_enum == nullptr)
                    {
                        return nullptr;
                    }

                    int64_t enum_value = property_enum->get_value_by_name(buffer);
                    if (enum_value != INDEX_NONE)
                    {
                        set_signed_int_property_value(data, enum_value);
                    }
                    else
                    {
                        return nullptr;
                    }
                }
                else
                {
                    if (!_wcsnicmp(start, STR("0x"), 2))
                    {
                        buffer += 2;
                        while (buffer && (hex_digit(*buffer) != 0 || *buffer == '0'))
                        {
                            buffer++;
                        }
                    }
                    else
                    {
                        while (buffer && (*buffer == '-' || *buffer == '+'))
                        {
                            buffer++;
                        }
                        while (buffer && iswdigit(*buffer))
                        {
                            buffer++;
                        }
                    }
                    if (start == buffer)
                    {
                        return nullptr;
                    }
                }
            }
            else
            {
                while (*buffer == '+' || *buffer == '-' || *buffer == '.' || (*buffer >= '0' && *buffer <= '9'))
                {
                    buffer++;
                }
                if (*buffer == 'f' || *buffer == 'F')
                {
                    buffer++;
                }
            }

            set_numeric_property_value_from_string(data, start);
        }
        return buffer;
    }
}