#include <Unreal/StaticOffsetFinder.hpp>
#include "Unreal/Property/NumericPropertyTypes.h"

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FInt8Property);
    IMPLEMENT_TPROPERTY_NUMERIC_VIRTUAL_FUNCTIONS(FInt8Property);

    IMPLEMENT_FIELD_CLASS(FInt16Property);
    IMPLEMENT_TPROPERTY_NUMERIC_VIRTUAL_FUNCTIONS(FInt16Property);

    IMPLEMENT_FIELD_CLASS(FIntProperty);
    IMPLEMENT_TPROPERTY_NUMERIC_VIRTUAL_FUNCTIONS(FIntProperty);

    IMPLEMENT_FIELD_CLASS(FInt64Property);
    IMPLEMENT_TPROPERTY_NUMERIC_VIRTUAL_FUNCTIONS(FInt64Property);

    IMPLEMENT_FIELD_CLASS(FUInt16Property);
    IMPLEMENT_TPROPERTY_NUMERIC_VIRTUAL_FUNCTIONS(FUInt16Property);

    IMPLEMENT_FIELD_CLASS(FUInt32Property);
    IMPLEMENT_TPROPERTY_NUMERIC_VIRTUAL_FUNCTIONS(FUInt32Property);

    IMPLEMENT_FIELD_CLASS(FUInt64Property);
    IMPLEMENT_TPROPERTY_NUMERIC_VIRTUAL_FUNCTIONS(FUInt64Property);

    IMPLEMENT_FIELD_CLASS(FFloatProperty);
    IMPLEMENT_TPROPERTY_NUMERIC_VIRTUAL_FUNCTIONS(FFloatProperty);

    IMPLEMENT_FIELD_CLASS(FDoubleProperty);
    IMPLEMENT_TPROPERTY_NUMERIC_VIRTUAL_FUNCTIONS(FDoubleProperty);

    IMPLEMENT_FIELD_CLASS(FByteProperty);
    IMPLEMENT_TPROPERTY_NUMERIC_VIRTUAL_FUNCTIONS(FByteProperty);

    auto FByteProperty::get_int_property_enum_impl() -> UEnum*
    {
        return get_enum();
    }

    auto FByteProperty::get_enum() -> UEnum*
    {
        return Helper::Casting::offset_deref<UEnum*>(this, StaticOffsetFinder::retrieve_static_offset(StaticOffsetFinder::MemberOffsets::ByteProperty_Enum));
    }
}
