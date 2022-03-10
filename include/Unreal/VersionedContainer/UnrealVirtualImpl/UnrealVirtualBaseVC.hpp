#ifndef RC_UNREAL_UNREAL_VIRTUAL_BASE_VC_HPP
#define RC_UNREAL_UNREAL_VIRTUAL_BASE_VC_HPP

#define PARAMS(...) __VA_ARGS__
#define ARGS(...)  __VA_ARGS__

#define IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(class_name, function_name, return_type) \
if (VTableOffsets::function_name == 0)                                                         \
{                                                                                              \
    throw std::runtime_error{"Virtual " #class_name "::" #function_name " is unavailable, possibly unsupported in engine version"}; \
}                                                                                           \
std::byte* vtable = std::bit_cast<std::byte*>(*std::bit_cast<std::byte**>(this)); \
RC::Function<return_type(const class_name*)> func = *std::bit_cast<void**>(vtable + VTableOffsets::function_name); \
if (!func.is_ready()) \
{ \
throw std::runtime_error{"Function '" #function_name "' not available"}; \
}                                                                          \
return func(this);

#define IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(class_name, function_name, return_type, params, args) \
if (VTableOffsets::function_name == 0)                                                         \
{                                                                                              \
    throw std::runtime_error{"Virtual " #class_name "::" #function_name " is unavailable, possibly unsupported in engine version"}; \
}\
std::byte* vtable = std::bit_cast<std::byte*>(*std::bit_cast<std::byte**>(this)); \
RC::Function<return_type(const class_name*, params)> func = *std::bit_cast<void**>(vtable + VTableOffsets::function_name); \
if (!func.is_ready()) \
{ \
throw std::runtime_error{"Function '" #function_name "' not available"}; \
}                                                                          \
return func(this, args);

#define GET_ADDRESS_OF_UNREAL_VIRTUAL(class_name, function_name, instance) \
[&instance]() -> void* {                                                           \
    std::byte* vtable = std::bit_cast<std::byte*>(*std::bit_cast<std::byte**>(instance)); \
    return *std::bit_cast<void**>(vtable + class_name::VTableOffsets::function_name);\
}()

namespace RC::Unreal
{
    class UnrealVirtualBaseVC
    {
    public:
        virtual ~UnrealVirtualBaseVC() = default;

    public:
        virtual auto set_virtual_offsets() -> void = 0;
    };
}

#endif //RC_UNREAL_UNREAL_VIRTUAL_BASE_VC_HPP