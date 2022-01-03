#ifndef RC_UNREAL_PROPERTYREADERS_HPP
#define RC_UNREAL_PROPERTYREADERS_HPP

#include <cstdint>

#define REGISTER_BASE_PROPERTY_READER(func_name, return_type)\
template<>                                                   \
auto func_name<return_type>(const wchar_t* property_string, CustomProperty* custom_property)\
{

#define REGISTER_UOBJECT_PROPERTY_READER(reader_func) \
    return PropertyReaders::reader_func(get_property_internal(property_string, custom_property));\
}

#define REGISTER_STRUCT_PROPERTY_READER(reader_func) \
    return PropertyReaders::reader_func(get_internal_complex_value(property_string, custom_property));\
}\

#define REGISTER_ALL_PROPERTY_READERS(func_name, macro_one) \
REGISTER_BASE_PROPERTY_READER(func_name, UObject)                         \
macro_one(read_object_property_data)\
REGISTER_BASE_PROPERTY_READER(func_name, FWeakObjectPtr)                         \
macro_one(read_weak_object_property_data)\
REGISTER_BASE_PROPERTY_READER(func_name, UClass)                         \
macro_one(read_class_property_data)\
REGISTER_BASE_PROPERTY_READER(func_name, int8_t)                         \
macro_one(read_int8_property_data)\
REGISTER_BASE_PROPERTY_READER(func_name, int16_t)                         \
macro_one(read_int16_property_data)\
REGISTER_BASE_PROPERTY_READER(func_name, int32_t)                         \
macro_one(read_int_property_data)\
REGISTER_BASE_PROPERTY_READER(func_name, int64_t)                         \
macro_one(read_int64_property_data)\
REGISTER_BASE_PROPERTY_READER(func_name, uint8_t)                         \
macro_one(read_byte_property_data)\
REGISTER_BASE_PROPERTY_READER(func_name, float)                         \
macro_one(read_float_property_data)                         \
REGISTER_BASE_PROPERTY_READER(func_name, bool)                         \
macro_one(read_bool_property_data)\
REGISTER_BASE_PROPERTY_READER(func_name, FName)                         \
macro_one(read_name_property_data)

namespace RC::Unreal
{
    struct PropertyDataVC;

    class UObject;
    class UClass;
    class XStructData;
    struct FName;
}

namespace RC::Unreal
{
    struct FWeakObjectPtr;
}

namespace RC::Unreal::PropertyReaders
{
    // Trivial -> START
    auto read_object_property_data(const PropertyDataVC&) -> UObject**;
    auto read_weak_object_property_data(const PropertyDataVC&) -> Unreal::FWeakObjectPtr*;
    auto read_class_property_data(const PropertyDataVC&) -> UClass**;

    auto read_int8_property_data(const PropertyDataVC&) -> int8_t*;
    auto read_int16_property_data(const PropertyDataVC&) -> int16_t*;
    auto read_int_property_data(const PropertyDataVC&) -> int32_t*;
    auto read_int64_property_data(const PropertyDataVC&) -> int64_t*;
    auto read_byte_property_data(const PropertyDataVC&) -> uint8_t*;

    auto read_float_property_data(const PropertyDataVC&) -> float*;
    auto read_bool_property_data(const PropertyDataVC&) -> bool;
    auto read_name_property_data(const PropertyDataVC&) -> FName&;
    // Trivial -> END
}


#endif //RC_UNREAL_PROPERTYREADERS_HPP
