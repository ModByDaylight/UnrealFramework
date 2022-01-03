#include <Unreal/PropertyReaders.hpp>
#include <Unreal/TArray.hpp>
#include <Unreal/Property/XObjectProperty.hpp>
#include <Unreal/Property/XWeakObjectProperty.hpp>
#include <Unreal/Property/XClassProperty.hpp>
#include <Unreal/Property/XInt8Property.hpp>
#include <Unreal/Property/XInt16Property.hpp>
#include <Unreal/Property/XIntProperty.hpp>
#include <Unreal/Property/XInt64Property.hpp>
#include <Unreal/Property/XByteProperty.hpp>
#include <Unreal/Property/XFloatProperty.hpp>
#include <Unreal/Property/XBoolProperty.hpp>
#include <Unreal/Property/XNameProperty.hpp>
#include <Unreal/XStructData.hpp>

namespace RC::Unreal::PropertyReaders
{
    auto read_object_property_data(const PropertyDataVC& data) -> Unreal::UObject**
    {
        return XObjectProperty::read_data(data);
    }
    auto read_weak_object_property_data(const PropertyDataVC& data) -> Unreal::FWeakObjectPtr*
    {
        return XWeakObjectProperty::read_data(data);
    }
    auto read_class_property_data(const PropertyDataVC& data) -> Unreal::UClass**
    {
        return XClassProperty::read_data(data);
    }

    auto read_int8_property_data(const PropertyDataVC& data) -> int8_t*
    {
        return XInt8Property::read_data(data);
    }
    auto read_int16_property_data(const PropertyDataVC& data) -> int16_t*
    {
        return XInt16Property::read_data(data);
    }
    auto read_int_property_data(const PropertyDataVC& data) -> int32_t*
    {
        return XIntProperty::read_data(data);
    }
    auto read_int64_property_data(const PropertyDataVC& data) -> int64_t*
    {
        return XInt64Property::read_data(data);
    }
    auto read_byte_property_data(const PropertyDataVC& data) -> uint8_t*
    {
        return XByteProperty::read_data(data);
    }

    auto read_float_property_data(const PropertyDataVC& data) -> float*
    {
        return XFloatProperty::read_data(data);
    }
    auto read_bool_property_data(const PropertyDataVC& data) -> bool
    {
        return XBoolProperty::read_data(data);
    }
    auto read_name_property_data(const PropertyDataVC& data) -> FName&
    {
        return XNameProperty::read_data(data);
    }
}
