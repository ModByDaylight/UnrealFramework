#include <Unreal/UClass.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/FProperty.hpp>
#include <Helpers/Casting.hpp>

namespace RC::Unreal
{
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UScriptStruct);

    using MemberOffsets = ::RC::Unreal::StaticOffsetFinder::MemberOffsets;

    int32 UScriptStruct::GetSize()
    {
        // This is coincidentally the right offset
        return Helper::Casting::offset_deref<int32_t>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UStruct_PropertiesSize));
    }

    auto UScriptStruct::GetStructFlags() -> EStructFlags
    {
        return Helper::Casting::offset_deref<EStructFlags>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UScriptStruct_StructFlags));
    }

    auto UScriptStruct::GetSuperScriptStruct() -> UScriptStruct*
    {
        return Cast<UScriptStruct>(GetSuperStruct());
    }
}