#ifndef RC_UNREAL_CUSTOM_TYPE_HPP
#define RC_UNREAL_CUSTOM_TYPE_HPP

#include <Unreal/XProperty.hpp>

namespace RC::Unreal
{
    class UScriptStruct;

    // Special struct to be used for creating our own U/FProperty objects with our own custom data
    // Used when creating custom properties
    class RC_UE_API CustomProperty : public XProperty
    {
    protected:
        using MemberOffsets = ::RC::Unreal::StaticOffsetFinder::MemberOffsets;

        // Since the property classes are normally just supposed to be mapped directly to game memory,
        // we don't have an implicit vtable pointer, so we're faking one here just for alignment purposes
        //void* vtable;

    public:
        // Untyped data
        // The proper data will be copied inside at the proper offsets using StaticOffsetFinder
        unsigned char m_data[0x128]{0};

    public:
        CustomProperty(int32_t offset_internal);

        auto static construct(int32_t offset_internal, UClass* belongs_to_class, UClass* inner_class) -> std::unique_ptr<CustomProperty>;
    };

    class CustomArrayProperty : public CustomProperty
    {
    public:
        CustomArrayProperty(int32_t offset_internal);

        // Used for the C++ API
        auto static construct(int32_t offset_internal, XProperty* array_inner) -> std::unique_ptr<CustomProperty>;

        // Used for the Lua API
        auto static construct(int32_t offset_internal, UClass* belongs_to_class, UClass* inner_class, XProperty* array_inner) -> std::unique_ptr<CustomProperty>;
    };

    class CustomStructProperty : public CustomProperty
    {
    public:
        CustomStructProperty(int32_t offset_internal);

        auto static construct(int32_t offset_internal, UScriptStruct* script_struct) -> std::unique_ptr<CustomProperty>;
    };
}


#endif //RC_UNREAL_CUSTOM_TYPE_HPP
