#ifndef UE4SS_FNUMERICPROPERTY_H
#define UE4SS_FNUMERICPROPERTY_H

#include <Unreal/Property/TProperty.h>
#include <File/File.hpp>
#include "Unreal/UnrealStringConversions.h"

#define IMPLEMENT_TPROPERTY_NUMERIC_VIRTUAL_FUNCTIONS(ClassName) \
    IMPLEMENT_TPROPERTY_WITH_EQUALITY_AND_SERIALIZER_VIRTUAL_FUNCTIONS(ClassName); \
    IMPLEMENT_VIRTUAL_FUNCTION(ClassName, is_unsigned_integer); \
    IMPLEMENT_VIRTUAL_FUNCTION(ClassName, set_unsigned_int_property_value); \
    IMPLEMENT_VIRTUAL_FUNCTION(ClassName, set_signed_int_property_value);

namespace RC::Unreal
{
    class UEnum;

    class RC_UE_API FNumericProperty : public FProperty
    {
        DECLARE_FIELD_CLASS(FNumericProperty);
        DECLARE_VIRTUAL_TYPE(FNumericProperty);

    public:
#include <VTableOffsets_FNumericProperty.hpp>

    public:
        /** Return true if this property is for a floating point number **/
        auto IsFloatingPoint() const -> bool;

        /** Return true if this property is for a integral or enum type **/
        auto IsInteger() const -> bool;

        /** Return the UEnum if this property is a FByteProperty with a non-null Enum **/
        auto GetIntPropertyEnum() const -> UEnum*;

        // TODO: The vtable offset generator isn't currently capable of handling multiple functions with the same name
        //       As a result, these two functions map to the same offset
        auto SetIntPropertyValue(void* Data, uint64 Value) const -> void;
        auto SetIntPropertyValue(void* Data, int64 Value) const -> void;

        /**
         * Set the value of a floating point property type
         * @param data - pointer to property data to set
         * @param value - Value to set data to
        **/
        auto SetFloatingPointPropertyValue(void* Data, double Value) -> void;

        /**
         * Set the value of any numeric type from a string point
         * @param data - pointer to property data to set
         * @param value - Value (as a string) to set
         * CAUTION: This routine does not do enum name conversion
        **/
        auto SetNumericPropertyValueFromString(void* Data, TCHAR const* Value) -> void;

        /**
         * Set the value of a signed integral property type
         * @param data - pointer to property data to set
         * @param value - Value to set data to
        **/
        /**
         * Gets the value of a signed integral property type
         * @param data - pointer to property data to get
         * @return Data as a signed int
        **/
        auto GetSignedIntPropertyValue(void* Data, int64 Value) -> void;

        /**
         * Gets the value of an unsigned integral property type
         * @param data - pointer to property data to get
         * @return Data as an unsigned int
        **/
        auto GetUnsignedIntPropertyValue(void const* Data) -> uint64;

        /**
         * Gets the value of an floating point property type
         * @param data - pointer to property data to get
         * @return Data as a double
        **/
        auto GetFloatingPointPropertyValue(void const* Data) -> double;

        /**
         * Get the value of any numeric type and return it as a string
         * @param data - pointer to property data to get
         * @return Data as a string
         * CAUTION: This routine does not do enum name conversion
        **/
        auto GetNumericPropertyValueToString(void const* Data) -> FString;

        auto CanHoldDoubleValueInternal(double Value) const -> bool;

        auto CanHoldSignedValueInternal(int64 Value) const -> bool;

        auto CanHoldUnsignedValueInternal(uint64 Value) const -> bool;

        /** Returns true if this property is for a unsigned integral type */
        auto is_unsigned_integer() -> bool;

        /** Return true if this property is a FByteProperty with a non-null Enum **/
        inline auto is_enum() -> bool
        {
            return !!GetIntPropertyEnum();
        }

        /**
         * Set the value of an unsigned integral property type
         * @param data - pointer to property data to set
         * @param value - Value to set data to
        **/
        auto set_unsigned_int_property_value(void* data, uint64_t value) -> void;
    };

    template<typename InTCppType>
    class RC_UE_API TProperty_Numeric : public TProperty_WithEqualityAndSerializer<InTCppType, FNumericProperty>
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

        auto get_int_property_enum_impl() -> UEnum*
        {
            return nullptr;
        }

        auto set_unsigned_int_property_value_impl(void* data, uint64_t value) -> void
        {
            if (!std::is_integral_v<TCppType>)
            {
                throw std::runtime_error("Cannot set floating point property value to integral value");
            }
            TTypeFundamentals::set_property_value(data, (TCppType) value);
        }

        auto set_signed_int_property_value_impl(void* data, int64_t value) -> void
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

        auto set_numeric_property_value_from_string_impl(void* data, const wchar_t* value) -> void
        {
            lex_from_string(*TTypeFundamentals::get_property_value_ptr(data), value);
        }

        auto get_numeric_property_value_to_string_impl(const void* data) -> std::wstring
        {
            return lex_to_string(TTypeFundamentals::get_property_value(data));
        }
    };
}

#endif //UE4SS_FNUMERICPROPERTY_H
