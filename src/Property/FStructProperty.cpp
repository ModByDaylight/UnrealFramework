#include <Unreal/Property/FStructProperty.hpp>
#include <Helpers/Casting.hpp>

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FStructProperty);

    UScriptStruct* FStructProperty::GetStruct()
    {
        return Helper::Casting::offset_deref<UScriptStruct*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::StructProperty_Struct));
    }
}
