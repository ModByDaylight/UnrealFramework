#ifndef UE4SS_FNUMERICPROPERTY_H
#define UE4SS_FNUMERICPROPERTY_H

#include <Unreal/Property/TProperty.h>
#include <File/File.hpp>

namespace RC::Unreal
{
    class UEnum;

    class FNumericProperty : public FProperty
    {
        DECLARE_FIELD_CLASS(FNumericProperty);
        DECLARE_VIRTUAL_TYPE(FNumericProperty);
    public:
        /** Return true if this property is for a floating point number **/
        auto is_floating_point() -> bool;

        /** Return true if this property is for a integral or enum type **/
        auto is_integer() -> bool;

        /** Returns true if this property is for a unsigned integral type */
        auto is_unsigned_integer();

        /** Return the UEnum if this property is a FByteProperty with a non-null Enum **/
       auto get_int_property_enum() -> UEnum*;

        /** Return true if this property is a FByteProperty with a non-null Enum **/
        inline auto is_enum() -> bool
        {
            return !!get_int_property_enum();
        }

        /**
         * Set the value of an unsigned integral property type
         * @param data - pointer to property data to set
         * @param value - Value to set data to
        **/
        auto set_int_property_value(void* data, uint64_t value) -> void;

        /**
         * Set the value of a signed integral property type
         * @param data - pointer to property data to set
         * @param value - Value to set data to
        **/
        auto set_int_property_value(void* data, int64_t value) -> void;

        /**
         * Set the value of a floating point property type
         * @param data - pointer to property data to set
         * @param value - Value to set data to
        **/
        auto set_floating_point_property_value(void* data, double value) -> void;

        /**
         * Set the value of any numeric type from a string point
         * @param data - pointer to property data to set
         * @param value - Value (as a string) to set
         * CAUTION: This routine does not do enum name conversion
        **/
        auto set_numeric_property_value_from_string(void* data, const wchar_t* value) -> void;

        /**
         * Gets the value of a signed integral property type
         * @param data - pointer to property data to get
         * @return Data as a signed int
        **/
        auto get_signed_int_property_value(const void* data) -> int64_t;

        /**
         * Gets the value of an unsigned integral property type
         * @param data - pointer to property data to get
         * @return Data as an unsigned int
        **/
        auto get_unsigned_int_property_value(const void* data) -> uint64_t;

        /**
         * Gets the value of an floating point property type
         * @param data - pointer to property data to get
         * @return Data as a double
        **/
        auto get_floating_point_property_value(const void* data) -> double;

        /**
         * Get the value of any numeric type and return it as a string
         * @param data - pointer to property data to get
         * @return Data as a string
         * CAUTION: This routine does not do enum name conversion
        **/
        auto get_numeric_property_value_to_string(const void* data) -> std::wstring;
    protected:
        auto export_text_item_impl(std::wstring& value_str, const void* property_value, const void* default_value, UObject* parent, int32_t port_flags, UObject* export_root_scope) -> void;
        auto import_text_impl(const wchar_t* buffer, void* data, int32_t port_flags, UObject* owner_object) -> wchar_t*;
    };

    template<typename InTCppType>
    class TProperty_Numeric : public TProperty_WithEqualityAndSerializer<InTCppType, FNumericProperty>
    {
    public:
        typedef TProperty_WithEqualityAndSerializer<InTCppType, FNumericProperty> Super;
        typedef InTCppType TCppType;
        typedef typename Super::TTypeFundamentals TTypeFundamentals;
    protected:
        auto is_floating_point_impl() -> bool
        {
            return std::is_floating_point_v<TCppType>;
        }

        auto is_integer_impl() -> bool
        {
            return std::is_integral_v<TCppType>;
        }

        auto is_unsigned_integer_impl() -> bool
        {
            return std::is_unsigned_v<TCppType>;
        }

        auto set_int_property_value_impl(void* data, uint64_t value) -> void
        {
            if (!std::is_integral_v<TCppType>)
            {
                throw std::runtime_error("Cannot set floating point property value to integral value");
            }
            TTypeFundamentals::set_property_value(data, (TCppType) value);
        }

        auto set_int_property_value_impl(void* data, int64_t value) -> void
        {
            if (!std::is_integral_v<TCppType>)
            {
                throw std::runtime_error("Cannot set floating point property value to integral value");
            }
            TTypeFundamentals::set_property_value(data, (TCppType) value);
        }

        auto set_floating_point_property_value_impl(void* data, double value) -> void
        {
            if (!std::is_floating_point_v<TCppType>)
            {
                throw std::runtime_error("Cannot set integer property value to floating point value");
            }
            TTypeFundamentals::set_property_value(data, (TCppType) value);
        }

        auto get_signed_int_property_value_impl(const void* data) -> int64_t
        {
            if (!std::is_integral_v<TCppType>)
            {
                throw std::runtime_error("Cannot get floating point property value as integer");
            }
            return (int64_t) TTypeFundamentals::get_property_value(data);
        }

        auto get_unsigned_int_property_value_impl(const void* data) -> uint64_t
        {
            if (!std::is_integral_v<TCppType>)
            {
                throw std::runtime_error("Cannot get floating point property value as integer");
            }
            return (uint64_t) TTypeFundamentals::get_property_value(data);
        }

        auto get_floating_point_property_value_impl(const void* data) -> double
        {
            if (!std::is_floating_point_v<TCppType>)
            {
                throw std::runtime_error("Cannot get integral property value as floating point");
            }
            return (double) TTypeFundamentals::get_property_value(data);
        }

        virtual void SetNumericPropertyValueFromString(void* Data, TCHAR const* Value) const override
        {
            LexFromString(*TTypeFundamentals::GetPropertyValuePtr(Data), Value);
        }

        virtual FString GetNumericPropertyValueToString(void const* Data) const override
        {
            return LexToString(TTypeFundamentals::GetPropertyValue(Data));
        }
    };
}

#endif //UE4SS_FNUMERICPROPERTY_H
