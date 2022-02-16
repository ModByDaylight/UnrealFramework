#include <Unreal/Property/FStructProperty.hpp>

namespace RC::Unreal
{
    UScriptStruct* FStructProperty::GetStruct()
    {
        return Helper::Casting::offset_deref<UScriptStruct*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::StructProperty_Struct));
    }
}
