#ifndef RC_UNREAL_XSTRUCTDATA_HPP
#define RC_UNREAL_XSTRUCTDATA_HPP

#include <Helpers/Format.hpp>

#include <Unreal/FField.hpp>
#include <Unreal/XProperty.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/PropertyReaders.hpp>
#include <Unreal/Property/XStructProperty.hpp>
#include <Unreal/Property/XIntProperty.hpp>
#include <Unreal/Property/XFloatProperty.hpp>
#include <Unreal/CustomType.hpp>

namespace RC::Unreal
{
    class RC_API XStructData
    {
    private:
        void* m_uobject{};
        void* m_data{};
        XStructProperty* m_prop{};

    public:
        XStructData(void* uobject, void* data, void* prop) : m_uobject(uobject), m_data(data), m_prop(static_cast<XStructProperty*>(prop)) {}

    public:
        auto get_data_ptr() -> void*
        {
            return m_data;
        }

        auto get_uobject_ptr() -> void*
        {
            return m_uobject;
        }

        auto get_property_ptr() -> XStructProperty*
        {
            return m_prop;
        }

        auto set_data_ptr(void* new_data) -> void
        {
            m_data = new_data;
        }

        auto get_struct_child(const wchar_t* struct_prop) -> XProperty*;
        auto get_struct_child(FName struct_prop_name) -> XProperty*;

        template<typename InnerStructPropType>
        // I'd like to use operator[] but there are problems with making it work with a template
        //auto operator[](const wchar_t* struct_prop) -> InnerStructPropType*
        auto get_internal_complex(const wchar_t* struct_prop) -> typename InnerStructPropType::InnerType*
        {
            // Exception thrown inside the call if property was not found
            XProperty* selected_property = get_struct_child(struct_prop);

            void* data_ptr = static_cast<void*>(static_cast<char*>(m_data) + selected_property->get_offset_for_internal());
            if (!data_ptr)
            {
                throw std::runtime_error{fmt("[XStructDataImpl::get_internal_complex] data_ptr is nullptr (selected_property: %p)", selected_property)};
            }

            PropertyDataVC data{.property_ptr = m_prop, .uobject_ptr = m_uobject, .data_ptr = data_ptr};
            return InnerStructPropType::read_data(data);
        }

        auto get_internal_complex_value(const wchar_t* struct_prop, CustomProperty* custom_property = nullptr) -> PropertyDataVC;

        template<typename InnerStructPropInnerType>
        auto get_internal_trivial(const wchar_t* struct_prop) -> InnerStructPropInnerType*
        {
            // Exception thrown inside the call if property was not found
            XProperty* selected_property = get_struct_child(struct_prop);

            auto data_ptr = static_cast<InnerStructPropInnerType*>(static_cast<void*>(static_cast<char*>(m_data) + selected_property->get_offset_for_internal()));
            if (!data_ptr)
            {
                throw std::runtime_error{fmt("[XStructDataImpl::get_internal_trivial] data_ptr is nullptr (selected_property: %p)", selected_property)};
            }

            return data_ptr;
        }

        // Used for trivial types that return Type*
        // This will do the required map to get the data without involving the 'read_data()' function in the property class
        // The good thing with this one is that you can supply the actual type (e.g. int32_t) instead of the property type (e.g. XIntProperty)
        template<typename InnerStructPropType>
        auto get_property(const wchar_t* struct_prop) -> InnerStructPropType*
        {
            return get_internal_trivial<InnerStructPropType>(struct_prop);
        }

        // Specializations, this might not be required (unused for now)
        // This will invoke the 'read_data()' function for the respective property
        // This was initially gonna be for non-ptr types (e.g. arrays & maps) but it didn't work very well
        // See get_value<PropertyType>()
        template<>
        auto get_property<float>(const wchar_t* struct_prop) -> float*
        {
            return get_internal_complex<XFloatProperty>(struct_prop);
        }

        // Used for types that can't return a simple Type*
        template<typename PropertyType>
        auto get_value(const wchar_t* struct_prop, class CustomProperty* custom_property = nullptr) -> auto
        {
            if constexpr (std::is_same_v<PropertyType, class XStructProperty>)
            {
                return PropertyType::read_data(get_internal_complex_value(struct_prop, custom_property));
            }
            else if constexpr (std::is_enum_v<PropertyType>)
            {
                return TUEnum<PropertyType>::read_data(get_internal_complex_value(struct_prop, custom_property));
            }
            else if constexpr (PropertyType::internal_type == InternalType::Array)
            {
                return PropertyType::read_data(get_internal_complex_value(struct_prop, custom_property));
            }
            else
            {
                static_assert(false, "never hit");
            }
        }

        REGISTER_ALL_PROPERTY_READERS(get_value, REGISTER_STRUCT_PROPERTY_READER)
    };
}

#endif //RC_UNREAL_XSTRUCTDATA_HPP
