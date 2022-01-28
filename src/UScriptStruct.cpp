#include <Unreal/UClass.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/FProperty.hpp>

namespace RC::Unreal
{
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UScriptStruct);

    using MemberOffsets = ::RC::Unreal::StaticOffsetFinder::MemberOffsets;

    auto UScriptStruct::get_struct_flags() -> EStructFlags
    {
        return Helper::Casting::offset_deref<EStructFlags>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UScriptStruct_StructFlags));
    }

    auto UScriptStruct::get_super_script_struct() -> UScriptStruct*
    {
        return cast_object<UScriptStruct>(get_super_struct());
    }
}