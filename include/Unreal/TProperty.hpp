#ifndef RC_UNREAL_TPROPERTY_H
#define RC_UNREAL_TPROPERTY_H

#include <Unreal/FProperty.hpp>

namespace RC::Unreal
{
    /** Implements common operations for the provided template type */
    template<typename InTCppType>
    class TPropertyTypeFundamentals
    {
    public:
        typedef InTCppType TCppType;
        enum
        {
            CPPSize = sizeof(TCppType),
            CPPAlignment = alignof(TCppType)
        };

        /** Convert the address of a value of the property to the proper type */
        FORCEINLINE static auto get_property_value_ptr(const void* value) -> const TCppType*
        {
            return std::bit_cast<const TCppType*>(value);
        }

        /** Convert the address of a value of the property to the proper type */
        FORCEINLINE static auto get_property_value_ptr(void* value) -> TCppType*
        {
            return std::bit_cast<TCppType*>(value);
        }

        /** Get the value of the property from an address */
        FORCEINLINE static auto get_property_value(const void* value) -> const TCppType&
        {
            return *get_property_value_ptr(value);
        }

        /** Get the default value of the cpp type, just the default constructor, which works even for things like int32 */
        FORCEINLINE static auto get_default_property_value() -> TCppType
        {
            return TCppType();
        }

        /** Get the value of the property from an address, unless it is NULL, then return the default value */
        FORCEINLINE static auto get_optional_property_value(const void* value) -> TCppType
        {
            return value ? get_property_value(value) : get_default_property_value();
        }

        /** Set the value of a property at an address */
        FORCEINLINE static auto set_property_value(void* property_value, const TCppType& value) -> void
        {
            *get_property_value_ptr(property_value) = value;
        }

        /** Initialize the value of a property at an address, this assumes over uninitialized memory */
        FORCEINLINE static auto initialize_property_value(void* value) -> TCppType*
        {
            return new (value) TCppType();
        }

        /** Destroy the value of a property at an address */
        FORCEINLINE static auto destroy_property_value(void* value) -> void
        {
            get_property_value_ptr(value)->~TCppType();
        }
    };

    /** Generic property implementation for standard C++ types */
    template<typename InTCppType, class TInPropertyBaseClass>
    class TProperty : public TInPropertyBaseClass, public TPropertyTypeFundamentals<InTCppType>
    {
    public:
        typedef InTCppType TCppType;
        typedef TInPropertyBaseClass Super;
        typedef TPropertyTypeFundamentals<InTCppType> TTypeFundamentals;

        /** Convert the address of a container to the address of the property value, in the proper type */
        FORCEINLINE auto get_property_value_ptr_in_container(const void* A, int32_t array_index = 0) -> const TCppType*
        {
            assert_type_equality();
            return TTypeFundamentals::get_property_value_ptr(Super::template container_ptr_to_value_ptr<void>(A, array_index));
        }

        /** Convert the address of a container to the address of the property value, in the proper type */
        FORCEINLINE auto get_property_value_ptr_in_container(void* A, int32_t array_index = 0) -> TCppType*
        {
            assert_type_equality();
            return TTypeFundamentals::get_property_value_ptr(Super::template container_ptr_to_value_ptr<void>(A, array_index));
        }

        /** Get the value of the property from a container address */
        FORCEINLINE auto get_property_value_in_container(void const* A, int32_t array_index = 0) -> TCppType const&
        {
            return *get_property_value_ptr_in_container(A, array_index);
        }

        /** Get the value of the property from a container address, unless it is NULL, then return the default value */
        FORCEINLINE auto get_optional_property_value_in_container(const void* A, int32_t array_index = 0) -> TCppType
        {
            assert_type_equality();
            return A ? get_property_value_in_container(A, array_index) : TTypeFundamentals::GetDefaultPropertyValue();
        }

        /** Set the value of a property in a container */
        FORCEINLINE auto set_property_value_in_container(void* A, const TCppType& value, int32_t array_index = 0) -> void
        {
            *get_property_value_ptr_in_container(A, array_index) = value;
        }
    protected:
        auto initialize_value_impl(void* dest) -> void
        {
            assert_type_equality();
            for (int32_t i = 0; i < this->get_array_dim(); i++)
            {
                TTypeFundamentals::initialize_property_value((uint8_t*)dest + i * this->get_element_size());
            }
        }

        auto destroy_value_impl(void* dest) -> void
        {
            assert_type_equality();
            for (int32_t i = 0; i < this->get_array_dim(); i++)
            {
                TTypeFundamentals::destroy_property_value((uint8_t*) dest + i * this->get_element_size());
            }
        }

        auto clear_value_impl(void* data) -> void
        {
            assert_type_equality();
            TTypeFundamentals::set_property_value(data, TTypeFundamentals::get_default_property_value());
        }

        auto copy_values_internal_impl(void* dest, void const* src, int32_t count) -> void
        {
            assert_type_equality();
            for (int32_t i = 0; i < count; i++)
            {
                TTypeFundamentals::get_property_value_ptr(dest)[i] = TTypeFundamentals::get_property_value_ptr(src)[i];
            }
        }

        FORCEINLINE auto assert_type_equality()
        {
            if (this->get_element_size() != TTypeFundamentals::CPPSize)
            {
                throw std::runtime_error("Type equality assertion failed: get_element_size() != TTypeFundamentals::CPPSize");
            }
        }
    };

    template<typename InTCppType, class TInPropertyBaseClass>
    class TProperty_WithEqualityAndSerializer : public TProperty<InTCppType, TInPropertyBaseClass>
    {
    public:
        typedef TProperty<InTCppType, TInPropertyBaseClass> Super;
        typedef InTCppType TCppType;
        typedef typename Super::TTypeFundamentals TTypeFundamentals;
    protected:
        auto identical_impl(const void* A, const void* B, uint32_t port_flags) -> bool
        {
            // RHS is the same as TTypeFundamentals::GetOptionalPropertyValue(B) but avoids an unnecessary copy of B
            return TTypeFundamentals::get_property_value(A) == (B ? TTypeFundamentals::get_property_value(B) : TTypeFundamentals::get_default_property_value());
        }

        //virtual void SerializeItem(FStructuredArchive::FSlot Slot, void *Value, void const *Defaults) const override {
        //    Slot << *TTypeFundamentals::GetPropertyValuePtr(Value);
        //}
    };
}

#endif //RC_UNREAL_TPROPERTY_H
