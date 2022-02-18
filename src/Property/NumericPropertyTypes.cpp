#include <Unreal/StaticOffsetFinder.hpp>
#include "Unreal/Property/NumericPropertyTypes.hpp"

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FInt8Property);

    IMPLEMENT_FIELD_CLASS(FInt16Property);

    IMPLEMENT_FIELD_CLASS(FIntProperty);

    IMPLEMENT_FIELD_CLASS(FInt64Property);

    IMPLEMENT_FIELD_CLASS(FUInt16Property);

    IMPLEMENT_FIELD_CLASS(FUInt32Property);

    IMPLEMENT_FIELD_CLASS(FUInt64Property);

    IMPLEMENT_FIELD_CLASS(FFloatProperty);

    IMPLEMENT_FIELD_CLASS(FDoubleProperty);

    IMPLEMENT_FIELD_CLASS(FByteProperty);

    UEnum* FByteProperty::GetEnum()
    {
        return Helper::Casting::offset_deref<UEnum*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::ByteProperty_Enum));
    }
}
