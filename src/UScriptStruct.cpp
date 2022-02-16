#include <Unreal/UClass.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/FProperty.hpp>

namespace RC::Unreal
{
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UScriptStruct);

    using MemberOffsets = ::RC::Unreal::StaticOffsetFinder::MemberOffsets;

    int32 UScriptStruct::GetSize()
    {
        // This is coincidentally the right offset
        return Helper::Casting::offset_deref<int32_t>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UStruct_PropertiesSize));
    }

    auto UScriptStruct::get_struct_flags() -> EStructFlags
    {
        return Helper::Casting::offset_deref<EStructFlags>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UScriptStruct_StructFlags));
    }

    auto UScriptStruct::get_super_script_struct() -> UScriptStruct*
    {
        return cast_object<UScriptStruct>(get_super_struct());
    }
}