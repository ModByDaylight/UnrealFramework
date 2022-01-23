#include <Unreal/Property/XNumericProperty.hpp>
#include <Unreal/Property/XInt8Property.hpp>
#include <Unreal/Property/XInt16Property.hpp>
#include <Unreal/Property/XIntProperty.hpp>
#include <Unreal/Property/XInt64Property.hpp>
#include <Unreal/Property/XByteProperty.hpp>
#include <Unreal/Property/XUInt16Property.hpp>
#include <Unreal/Property/XUInt32Property.hpp>
#include <Unreal/Property/XUInt64Property.hpp>
#include <Unreal/Property/XFloatProperty.hpp>
#include <Unreal/Property/XDoubleProperty.hpp>

namespace RC::Unreal
{
    auto XNumericProperty::is_floating_point() -> bool
    {
        FName property_type = get_ffieldclass()->get_fname();

        if (property_type == TypeChecker::m_floatproperty_name ||
            property_type == TypeChecker::m_doubleproperty_name)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    auto XNumericProperty::read_as_int64(UObject* object, FProperty* property) -> int64_t
    {
        if (is_child_of<XInt8Property>())
        {
            return *property->container_ptr_to_value_ptr<int8_t>(object);
        }
        else if (is_child_of<XInt16Property>())
        {
            return *property->container_ptr_to_value_ptr<int16_t>(object);
        }
        else if (is_child_of<XIntProperty>())
        {
            return *property->container_ptr_to_value_ptr<int>(object);
        }
        else if (is_child_of<XInt64Property>())
        {
            return *property->container_ptr_to_value_ptr<int64_t>(object);
        }
        else if (is_child_of<XByteProperty>())
        {
            return *property->container_ptr_to_value_ptr<uint8_t>(object);
        }
        else if (is_child_of<XUInt16Property>())
        {
            return *property->container_ptr_to_value_ptr<uint16_t>(object);
        }
        else if (is_child_of<XUInt32Property>())
        {
            return *property->container_ptr_to_value_ptr<uint32_t>(object);
        }
        else if (is_child_of<XUInt64Property>())
        {
            return *property->container_ptr_to_value_ptr<uint64_t>(object);
        }
        else if (is_child_of<XFloatProperty>())
        {
            return *std::bit_cast<int64_t*>(property->container_ptr_to_value_ptr<float>(object));
        }
        else if (is_child_of<XDoubleProperty>())
        {
            return *std::bit_cast<int64_t*>(property->container_ptr_to_value_ptr<double>(object));
        }
        else
        {
            throw std::runtime_error{"Tried retrieving 'NumericProperty' value as 'int64' but the underlying type is not supported"};
        }
    }

    auto XNumericProperty::read_as_int64(UObject* object) -> int64_t
    {
        return read_as_int64(object, this);
    }

    auto XNumericProperty::read_as_double(UObject* object) -> double
    {
        if (is_child_of<XFloatProperty>())
        {
            return *container_ptr_to_value_ptr<float>(object);
        }
        else if (is_child_of<XDoubleProperty>())
        {
            return *container_ptr_to_value_ptr<double>(object);
        }
        else
        {
            throw std::runtime_error{"Tried retrieving 'NumericProperty' value as 'double' but the underlying type is not supported"};
        }
    }
}
