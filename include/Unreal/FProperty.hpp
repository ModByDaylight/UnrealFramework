#ifndef RC_UNREAL_XPROPERTY_HPP
#define RC_UNREAL_XPROPERTY_HPP

#include <Unreal/FField.hpp>
#include <Unreal/PrimitiveTypes.hpp>

namespace RC::Unreal
{
    class FString;

    enum class EPropertyObjectReferenceType
    {
        None = 0,
        Strong = 1 << 0,
        Weak = 1 << 1,
    }

    class RC_UE_API FProperty : public FField
    {
        DECLARE_FIELD_CLASS(FProperty);
        DECLARE_VIRTUAL_TYPE(FProperty);

    public:
#include <VTableOffsets_FProperty.hpp>

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

        /** See if the offset of this property is below the supplied container size */
        inline bool is_in_container(UStruct* container_class)
        {
            return get_offset_for_internal() + get_size() <= (container_class ? container_class->get_properties_size() : INT32_MAX);
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

        using FArchive = void*; // Remove if/when we have an FArchive implementation, for now, probably a bad idea to call
        auto serialize(FArchive& ar) -> void;

        auto post_duplicate(const FField& in_field) -> void;

        auto get_cpp_macro_type(FString& extended_type_text) const -> FString;

        auto get_cpp_type(FString* extended_type_text = nullptr, uint32 cpp_export_flags = 0) const -> FString;

        auto pass_cpp_args_by_ref() const -> bool;

        auto get_cpp_type_forward_declaration() const -> FString;

        auto link_internal(FArchive& ar) -> void;

        using EConvertFromTypeResult = int32; // Remove if/when we have an EConvertFromTypeResult implementation, safe to use if you know the underlying enum numbers
        using FPropertyTag = void*; // Remove if/when we have a FPropertyTag implementation, for now, probably a bad idea to call
        struct FStructuredArchive { enum class FSlot {}; }; // Remove if/when we have a FStructuredArchive implementation, for now, probably a bad idea to call
        auto convert_from_type(const FPropertyTag& tag, FStructuredArchive::FSlot slot, uint8* data, UStruct* defaults_struct) -> EConvertFromTypeResult;

        /**
         * Determines whether the property values are identical.
         *
         * @param A	property data to be compared, already offset
         * @param B	property data to be compared, already offset
         * @param port_flags allows caller more control over how the property values are compared
         * @return true if the property values are identical
         */
        auto identical(const void* A, const void* B, uint32_t port_flags = 0) -> bool;

        auto serialize_item(FStructuredArchive::FSlot slot, void* value, void const* defaults = nullptr) const -> void;

        using UPackageMap = void*; // Remove if/when we have a UPackageMap implementation, for now, probably a bad idea to call
        auto net_serialize_item(FArchive& ar, UPackageMap* map, void* data, TArray<uint8>* meta_data = nullptr) const -> bool;

        auto supports_met_shared_serialization() const -> bool;

        /**
         * Determines whether the property values are identical.
         *
         * @param A property container of data to be compared, NOT offset
         * @param B property container of data to be compared, NOT offset
         * @param port_flags allows caller more control over how the property values are compared
         * @return true if the property values are identical
         */
        inline auto identical_in_container(const void* A, const void* B, int32_t array_index = 0, uint32_t port_flags = 0) -> bool
        {
            return identical(container_ptr_to_value_ptr<void>(A, array_index), B ? container_ptr_to_value_ptr<void>(B, array_index) : nullptr, port_flags);
        }

        /**
         * Exports the property value into the text string provided
         *
         * @param value_str string to serialize the property value into
         * @param property_value property value to serialize
         * @param default_value the default value of the property, or nullptr to serialize everything
         * @param parent parent object on which the property is set
         * @param port_flags the additional flags for property serialization
         * @param export_root_scope the scope to create relative paths from, if the PPF_ExportsNotFullyQualified flag is passed in. If nullptr, the package containing the object will be used instead.
         */
        auto export_text_item(FString& value_str, const void* property_value, const void* default_value, UObject* parent, int32_t port_flags, UObject* export_root_scope = nullptr) -> void;

        /**
         * Exports the property value to the string if it differs from the delta value specified
         *
         * @param value_str string to append the property value to
         * @param data property value to serialize
         * @param delta default property value, serialization is skipped if it is identical to the data
         * @param parent parent object on which property is being serialized
         * @param port_flags the additional flags for property serialization
         * @param export_root_scope the scope to create relative paths from, if the PPF_ExportsNotFullyQualified flag is passed in
         * @return true if the value was different from delta and was actually serialized to string
         */
        inline auto export_text_direct(std::wstring& value_str, const void* data, const void* delta, UObject* parent, int32_t port_flags, UObject* export_root_scope = nullptr) -> bool
        {
            if (data == delta || !identical(data, delta, port_flags))
            {
                export_text_item(value_str, data, delta, parent, port_flags, export_root_scope);
                return true;
            }
            return false;
        }

        /**
         * Exports the property value to the string if it is different from the default value specified, in the container
         *
         * @param index index of the value if the property represents the statically sized array
         * @param value_str string to serialize the value into
         * @param data the pointer to the container with the property value
         * @param delta the pointer to the container with the default property value
         * @param parent the pointer to the UObject which property is being exported
         * @param port_flags the additional flags for property serialization
         * @param export_root_scope the scope to create relative paths from, if the PPF_ExportsNotFullyQualified flag is passed in
         * @return true if the value was different from delta and was actually serialized to string
         */
        inline auto export_text_in_container(int32_t index, std::wstring& value_str, const void* data, const void* delta, UObject* parent, int32_t port_flags, UObject* export_root_scope = NULL) -> bool
        {
            return export_text_direct(value_str, container_ptr_to_value_ptr<void>(data, index),
                                      container_ptr_to_value_ptr<void>(delta, index),
                                      parent, port_flags, export_root_scope);
        }

        /**
         * Imports the property value from the provided text string
         *
         * @param buffer the text to read the value from
         * @param data the property value to serialize the data into
         * @param port_flags the extra flags for parsing the value
         * @param owner_object the object owning this property's value
         * @return the remaining string after the value has been read, or nullptr if the value has not been read
         */
        auto import_text(const TCHAR* buffer, void* data, int32_t port_flags, UObject* owner_object, class FOutputDevice* error_text) -> const TCHAR*;
        auto import_text_internal(const TCHAR* buffer, void* data, int32 port_flags, UObject* owner_object, class FOutputDevice* error_text) -> const TCHAR*;

    public:

        /**
         * Copy the value for a single element of this property.
         * Behaves exactly the same as copy_complete_value for non-static sized array properties
         *
         * @param dest the address where the value should be copied to
         * @param src the address of the value to copy from
         */
        inline auto copy_single_value(void* dest, void const* src) -> void
        {
            copy_values_internal(dest, src, 1);
        }

        /**
         * Copy the value for all elements of this property.
         *
         * @param dest the address where the value should be copied to
         * @param src the address of the value to copy from
         */
        inline auto copy_complete_value(void* dest, const void* src) -> void
        {
            copy_values_internal(dest, src, get_array_dim());
        }

        inline auto copy_complete_value_in_container(void* dest, const void* src) -> void
        {
            return copy_complete_value(container_ptr_to_value_ptr<void>(dest), container_ptr_to_value_ptr<void>(src));
        }

        /**
         * Returns the hash value for an element of this property.
         * Will throw the exception if the property value is not hashable.
         */
        uint32_t get_value_type_hash(const void* src);
        auto get_value_type_hash_internal(const void* src) const -> uint32;

        /**
         * Copy the value for a single element of this property. To the script VM.
         *
         * @param dest the address where the value should be copied to
         * @param src the address of the value to copy from. should be evaluated the same way as Dest
         */
        auto copy_single_value_to_script_vm(void* dest, void const* src) const -> void;

        /**
         * Copy the value for all elements of this property. To the script VM.
         *
         * @param dest the address where the value should be copied to
         * @param src the address of the value to copy from. should be evaluated the same way as Dest
         */
        auto copy_complete_value_to_script_vm(void* dest, void const* src) const -> void;

        /**
         * Copy the value for a single element of this property. From the script VM.
         *
         * @param dest the address where the value should be copied to
         * @param src the address of the value to copy from. should be evaluated the same way as Dest
         */
        auto copy_single_value_from_script_vm(void* dest, void const* src) const -> void;

        /**
         * Copy the value for all elements of this property. From the script VM.
         *
         * @param dest the address where the value should be copied to
         * @param src the address of the value to copy from. should be evaluated the same way as Dest
         */
        auto copy_complete_value_from_script_vm(void* dest, void const* src) const -> void;

        /**
         * Zeros the value for this property. The existing data is assumed valid (so for example this calls FString::Empty)
         * This only does one item and not the entire fixed size array.
         *
         * @param data the address of the value for this property that should be cleared.
         */
        auto clear_value(void* data) -> void;
        auto clear_value_internal(void* data) const -> void;

        /**
         * Zeros the value for this property. The existing data is assumed valid (so for example this calls FString::Empty)
         * This only does one item and not the entire fixed size array.
         *
         * @param Data the address of the container of the value for this property that should be cleared.
         */
        inline auto clear_value_in_container(void* data, int32_t array_index = 0) -> void
        {
            clear_value(container_ptr_to_value_ptr<void>(data, array_index));
        }

        /**
         * Destroys the value for this property. The existing data is assumed valid (so for example this calls FString::Empty)
         * This does the entire fixed size array.
         *
         * @param dest the address of the value for this property that should be destroyed.
         */
        auto destroy_value(void* dest) -> void;
        auto destroy_value_internal(void* dest) const -> void;

        /**
         * Destroys the value for this property. The existing data is assumed valid (so for example this calls FString::Empty)
         * This does the entire fixed size array.
         *
         * @param dest the address of the container containing the value that should be destroyed.
         */
        inline auto destroy_value_in_container(void* dest) -> void
        {
            destroy_value(container_ptr_to_value_ptr<void>(dest));
        }

        /**
         * Zeros, copies from the default, or calls the constructor for on the value for this property.
         * The existing data is assumed invalid (so for example this might indirectly call FString::FString,
         * This will do the entire fixed size array.
         *
         * @param dest the address of the value for this property that should be cleared.
         */
        auto initialize_value(void* dest) -> void;
        auto initialize_value_internal(void* dest) const -> void;

        auto get_id() const -> FName;

        auto instance_subobjects(void* data, void const* default_data, UObject* owner, struct FObjectInstancingGraph* instance_graph) -> void;

        auto get_min_alignment() const -> int32;

        auto contains_object_reference(TArray<const class FStructProperty*>& encountered_struct_props, EPropertyObjectReferenceType in_reference_type = EPropertyObjectReferenceType::Strong) const -> bool;

        auto emit_reference_info(UClass& owner_class, int32 base_offset, TArray<const FStructProperty*>& encountered_struct_props) -> void;

        auto same_type(const FProperty* other) const -> bool;

        /**
         * Zeros, copies from the default, or calls the constructor for on the value for this property.
         * The existing data is assumed invalid (so for example this might indirectly call FString::FString,
         * This will do the entire fixed size array.
         *
         * @param dest the address of the container of value for this property that should be cleared.
         */
        inline auto initialize_value_in_container(void* dest) -> void
        {
            initialize_value(container_ptr_to_value_ptr<void>(dest));
        }
    //protected:
        /**
         * Copies specified number of property values from src address to dest
         * Internal function, do not call directly!
         */
        auto copy_values_internal(void* dest, const void* src, int32_t count) -> void;

        /**
         * Copies specified number of property values into the kismet VM
         * Internal function, do not call directly!
         */
        auto copy_values_to_script_vm_internal(void* dest, const void* src, int32_t count) -> void;

        /**
         * Copies specified number of property values from the kismet VM
         * Internal function, do not call directly!
         */
        auto copy_values_from_script_vm_internal(void* dest, const void* src, int32_t count) -> void;
    protected:
        auto copy_values_from_script_vm_internal_impl(void* dest, const void* src, int32_t count) -> void;
        auto copy_values_to_script_vm_internal_impl(void* dest, const void* src, int32_t count) -> void;

        //auto identical_impl(const void* A, const void* B, uint32_t port_flags) -> void;
        //auto export_text_item_impl(std::wstring& value_str, const void* property_value, const void* default_value, UObject* parent, int32_t port_flags, UObject* export_root_scope) -> void;
        //auto import_text_impl(const wchar_t* buffer, void* data, int32_t port_flags, UObject* owner_object) -> wchar_t*;
        //uint32_t get_value_type_hash_impl(const void* src);

        //auto clear_value_impl(void* data) -> void;
        //auto destroy_value_impl(void* dest) -> void;
        //auto initialize_value_impl(void* dest) -> void;
        //auto copy_values_internal_impl(void* dest, void const* src, int32_t count);
    };

    class RC_UE_API FPropertyHelpers
    {
    public:
        static const wchar_t* read_token(const wchar_t* buffer, std::wstring& out, bool dotted_names = false);
    };
}


#endif //RC_UNREAL_XPROPERTY_HPP
