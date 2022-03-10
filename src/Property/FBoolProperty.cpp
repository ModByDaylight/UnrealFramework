#include <Unreal/StaticOffsetFinder.hpp>
#include <Unreal/Property/FBoolProperty.hpp>
#include <Unreal/UnrealHashing.hpp>

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FBoolProperty);

    auto FBoolProperty::get_byte_offset() -> uint8_t
    {
        return Helper::Casting::offset_deref<uint8_t>(this, StaticOffsetFinder::retrieve_static_offset(StaticOffsetFinder::MemberOffsets::ByteOffset_BoolProperty));
    }

    auto FBoolProperty::get_byte_mask() -> uint8_t
    {
        return Helper::Casting::offset_deref<uint8_t>(this, StaticOffsetFinder::retrieve_static_offset(StaticOffsetFinder::MemberOffsets::ByteMask_BoolProperty));
    }

    auto FBoolProperty::get_field_mask() -> uint8_t
    {
        return Helper::Casting::offset_deref<uint8_t>(this, StaticOffsetFinder::retrieve_static_offset(StaticOffsetFinder::MemberOffsets::FieldMask_BoolProperty));
    }
}

