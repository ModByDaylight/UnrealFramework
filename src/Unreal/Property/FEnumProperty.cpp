#include <Unreal/Property/FEnumProperty.hpp>
#include <Helpers/Casting.hpp>

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FEnumProperty);

    auto FEnumProperty::GetUnderlyingProperty() -> FNumericProperty*
    {
        return Helper::Casting::offset_deref<FNumericProperty*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::EnumProperty_UnderlyingType));
    }

    auto FEnumProperty::GetEnum() -> UEnum*
    {
        return Helper::Casting::offset_deref<UEnum*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::EnumProperty_Enum));
    }
}
