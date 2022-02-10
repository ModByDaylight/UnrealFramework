#ifndef UE4SS_FBOOLPROPERTY_H
#define UE4SS_FBOOLPROPERTY_H

#include "File/File.hpp"
#include "Unreal/FProperty.hpp"

namespace RC::Unreal
{
    class RC_UE_API FBoolProperty : public FProperty
    {
        DECLARE_FIELD_CLASS(FBoolProperty);
        DECLARE_VIRTUAL_TYPE(FBoolProperty);
    public:
        //Emulate the CPP type API, see TPropertyTypeFundamentals
        //this is incomplete as some operations make no sense for bitfields, for example they don't have a usable address
        typedef bool TCppType;

        /** Offset from the member variable to the byte of the property (0-7). */
        auto get_byte_offset() -> uint8_t;

        /** Mask of the byte with the property value. */
        auto get_byte_mask() -> uint8_t;

        /** Mask of the field with the property value. Either equal to ByteMask or 255 in case of 'bool' type. */
        auto get_field_mask() -> uint8_t;

        /** Returns the value of the boolean property */
        FORCEINLINE auto get_property_value(const void* A) -> bool
        {
            uint8_t* byte_value = (uint8_t*)A + get_byte_offset();
            return !!(*byte_value & get_field_mask());
        }

        /** Returns the value of the boolean property inside of it's container */
        FORCEINLINE auto get_property_value_in_container(const void* A, int32_t array_index = 0) -> bool
        {
            return get_property_value(container_ptr_to_value_ptr<void>(A, array_index));
        }

        /** Returns the default value of the boolean property */
        FORCEINLINE static auto get_default_property_value() -> bool
        {
            return false;
        }

        /** Returns the property value from the given address, or default value if it represents the nullptr */
        FORCEINLINE auto get_optional_property_value(const void* B) -> bool
        {
            return B ? get_property_value(B) : get_default_property_value();
        }

        /** Returns the property value from the given container address, or default value if it represents the nullptr */
        FORCEINLINE auto get_optional_property_value_in_container(const void* B, int32_t array_index = 0) -> bool
        {
            return B ? get_property_value_in_container(B, array_index) : get_default_property_value();
        }

        /** Updates property value to the provided one */
        FORCEINLINE auto set_property_value(void* A, bool value) -> void
        {
            uint8_t* byte_value = (uint8_t*)A + get_byte_offset();
            *byte_value = ((*byte_value) & ~get_field_mask()) | (value ? get_byte_mask() : 0);
        }

        /** Updates property value inside of the container */
        FORCEINLINE auto set_property_value_in_container(void* A, bool value, int32_t array_index = 0)
        {
            set_property_value(container_ptr_to_value_ptr<void>(A, array_index), value);
        }

        /** If the return value is true this FBoolProperty represents C++ bool type */
        FORCEINLINE auto is_native_bool() -> bool
        {
            return get_field_mask() == 0xff;
        }
    protected:
        uint32_t get_value_type_hash_impl(const void* src);
    };
}

#endif //UE4SS_FBOOLPROPERTY_H
