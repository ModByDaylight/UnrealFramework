#ifndef RC_UNREAL_XPROPERTY_HPP
#define RC_UNREAL_XPROPERTY_HPP

#include <Unreal/FField.hpp>
#include <Unreal/PropertyMacros.h>

namespace RC::Unreal
{
    class RC_UE_API FProperty : public FField
    {
        DECLARE_PROPERTY_CLASS(FProperty);
    public:
        /**
         * Returns the array dimensions of the property
         * This is only relevant for statically sized array properties defined in C++
         * for other properties and non-native properties that will always be 1
         */
        auto get_array_dim() -> int32_t;

        /**
         * Retrieves the size of the single array element of this property
         * Equals to the property size for the properties that do not represent static arrays
         */
        auto get_element_size() -> int32_t;

        /**
         * Retrieves the flags defined on this property objects
         * Use methods below to test whenever the particular flags are set
         */
        auto get_property_flags() -> EPropertyFlags;

        /**
         * Retrieves the offset of this property inside of it's parent struct
         * That would mean the offset of the property inside of the UObject or UStruct
         */
        auto get_offset_for_internal() -> int32_t;

        /**
         * Retrieves the name of the RepNotify func associated with this property,
         * the function will be called once the property value is replicated across the network
         * Can totally return NAME_None if property is not replicated or does not have a RepNotify set
         */
        auto get_rep_notify_func() -> FName;

        /**
         * Returns the total size of the property, correctly handling the statically sized array properties
         * Equivalent to element size for other properties
         */
        inline auto get_size() -> int32_t
        {
            return get_element_size() * get_array_dim();
        }

        inline auto has_any_property_flags(EPropertyFlags flags_to_check) -> bool
        {
            return (get_property_flags() & flags_to_check) != 0 || flags_to_check == CPF_AllFlags;
        }

        inline auto has_all_property_flags(EPropertyFlags flags_to_check) -> bool
        {
            return ((get_property_flags() & flags_to_check) == flags_to_check);
        }

        template<typename T>
        auto container_ptr_to_value_ptr(void* container, int32_t array_index = 0) -> T*
        {
            return reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(container) + get_offset_for_internal() + get_element_size() * array_index);
        }

        template<typename T>
        auto container_ptr_to_value_ptr(const void* container, int32_t array_index = 0) -> const T*
        {
            return reinterpret_cast<const T*>(reinterpret_cast<uintptr_t>(container) + get_offset_for_internal() + get_element_size() * array_index);
        }

        /**
         * Initializes the memory with the default value of the property of this type
         * The pointed memory should be large enough to hold a complete property value of that type
         * @param dest pointer to the memory to store property's value at
         */
        auto initialize_value(void* dest) -> void;

        /**
         * Initializes property value inside of it's container
         * @param dest pointer to the property's container (UObject, struct or other property's value)
         */
        inline auto initialize_value_in_container(void* dest) -> void
        {
            initialize_value(container_ptr_to_value_ptr<void>(dest));
        }

        /**
         * Destroys the value of the property located at the provided memory pointer
         * The memory at the location should be initialized and contain a valid value of matching type
         */
        auto destroy_value(void* dest) -> void;

        /**
         * Destroys property value in the property's container object
         * @param dest pointer to the property's container object instance
         */
        inline auto destroy_value_in_container(void* dest) -> void
        {
            destroy_value(container_ptr_to_value_ptr<void>(dest));
        }

        /**
         * Resets value of the property inside of the container
         * This function will only clear ONE value of the statically sized array properties
         * @property dest pointer to the property's container object instance
         */
        inline auto clear_single_value_in_container(void* dest, int32_t array_index = 0) -> void
        {
            clear_single_value(container_ptr_to_value_ptr<void>(dest, array_index));
        }

        /**
        * Resets value of the property inside of the container
        * This function will clear the value of all elements for the statically sized array properties
        * @property dest pointer to the property's container object instance
        */
        inline auto clear_complete_value_in_container(void* dest) -> void
        {
            for (int32_t i = 0; i < get_array_dim(); i++)
            {
                clear_single_value_in_container(dest, i);
            }
        }

        /**
         * Resets the provided value of the property to the default one
         *
         * This function will only clear ONE value of the statically sized array properties
         * @param value pointer to the property's value
         */
        auto clear_single_value(void* value) -> void;

        /**
         * Clears the property value for all elements of the statically sized array properties
         * @param value pointer to the property's value
         */
        auto clear_complete_value(void* value) -> void
        {
            const int32_t element_size = get_element_size();
            for (int32_t i = 0; i < get_array_dim(); i++)
            {
                clear_single_value((uint8_t*) value + i * element_size);
            }
        }

        /**
         * Copies a single property value from one memory location to another
         *
         * This function will only copy ONE value of the statically sized array properties
         * @param dest the location to copy value into
         * @param src the location to copy value from
         */
        auto copy_single_value(void* dest, const void* src) -> void;

        /**
         * Copies a complete property value from one memory location to another
         * That would copy all of the array elements of the statically sized array properties
         * @param dest the location to copy value into
         * @param src the location to copy value from
         */
        inline auto copy_complete_value(void* dest, const void* src) -> void
        {
            const int32_t element_size = get_element_size();
            for (int32_t i = 0; i < get_array_dim(); i++)
            {
                copy_single_value((uint8_t*) dest + i * element_size, (uint8_t*) src + i * element_size);
            }
        }

        /**
         * Copies a single property value from one container instance to another
         * When dealing with statically sized arrays that would only copy the element at the provided index
         * @param dest destination container to copy values into
         * @param src source container to copy values from
         */
        inline auto copy_single_value_in_container(void* dest, void* src, int32_t array_index = 0) -> void
        {
            copy_single_value(container_ptr_to_value_ptr<void>(dest, array_index), container_ptr_to_value_ptr<void>(dest, array_index));
        }

        /**
         * Copies a complete property value from one container instance to another, including all elements
         * of the statically sized array properties
         * @param dest destination container to copy values into
         * @param src source container to copy values from
         */
        inline auto copy_complete_value_in_container(void* dest, void* src) -> void
        {
            for (int32_t i = 0; i < get_array_dim(); i++)
            {
                copy_single_value_in_container(dest, src, i);
            }
        }

        /**
         * Copies a single property value from the container into the KismetVM controlled external
         * memory location, such as return value space or context object pointer
         * This function will only copy a single property value for statically sized array properties
         *
         * Used primarily because almost all UObject-based properties are represented as raw UObject* values
         * and not like their internal property representation
         * For example, that would copy the UObject* into the VM for FWeakObjectProperty, even though
         * property's internal representation in memory would be FWeakObjectPtr
         *
         * @param dest the VM controlled memory location to copy value into
         * @param src the normal property value to make a copy of
         */
        auto copy_single_value_into_script_vm(void* dest, const void* src) -> void;

        /**
         * Copies a single property value from the KismetVM controlled external
         * memory location, such as return value space or context object pointer, into the normal property container
         * This function will only copy a single property value for statically sized array properties
         *
         * Used primarily because almost all UObject-based properties are represented as raw UObject* values
         * and not like their internal property representation
         * For example, that would set FWeakObjectPtr value for FWeakObjectProperty, even though
         * the internal object representation in the VM is just a raw UObject pointer
         *
         * @param dest the normal property value dest location to copy into
         * @param src the VM controlled memory location to copy value from
         */
        auto copy_single_value_from_script_vm(void* dest, const void* src) -> void;


        auto identical(const void* A, const void* B, uint32_t port_flags) -> bool;
        auto export_text(std::wstring& value_string, const void* property_value, const void* default_value, UObject* parent, int32_t port_flags) -> void;
        auto import_text(const wchar_t* buffer, void* data, int32_t port_flags, UObject* owner_object) -> const wchar_t*;
    protected:
        auto get_dispatch_table() -> const Internal::PropertyDispatchTable&;

        //INTERNAL IMPLEMENTATION FUNCTIONS
        //DECLARE FUNCTIONS WITH THE MATCHING SIGNATURE IN CHILD CLASSES TO OVERRIDE
        auto initialize_value_implementation(void* dest) -> void;
        auto destroy_value_implementation(void* value) -> void;
        auto copy_value_implementation(void* dest, const void* src) -> void;
        auto clear_value_implementation(void* value) -> void;
        auto identical_implementation(const void* A, const void* B, uint32_t port_flags) -> bool;
        auto export_text_implementation(std::wstring& value_string, const void* property_value, const void* default_value, UObject* parent, int32_t port_flags) -> void;
        auto import_text_implementation(const wchar_t* buffer, void* data, int32_t port_flags, UObject* owner_object) -> const wchar_t*;

        //FUNCTIONS SPECIFIC TO FNumericProperty
        auto get_property_value_int64_implementation(void* value) -> int64_t;
        auto get_property_value_double_implementation(void* value) -> double;
        auto is_floating_point_implementation() -> bool;

        //FUNCTIONS SPECIFIC TO FObjectPropertyBase
        auto get_property_value_object_implementation(void* value) -> UObject*;
    };
}


#endif //RC_UNREAL_XPROPERTY_HPP
