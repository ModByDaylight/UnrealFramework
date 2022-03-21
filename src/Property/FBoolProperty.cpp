#include <Unreal/StaticOffsetFinder.hpp>
#include <Unreal/Property/FBoolProperty.hpp>
#include <Unreal/UnrealHashing.hpp>

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FBoolProperty);

    auto FBoolProperty::GetByteOffset() -> uint8_t
    {
        return Helper::Casting::offset_deref<uint8_t>(this, StaticOffsetFinder::retrieve_static_offset(StaticOffsetFinder::MemberOffsets::ByteOffset_BoolProperty));
    }

    auto FBoolProperty::GetByteMask() -> uint8_t
    {
        return Helper::Casting::offset_deref<uint8_t>(this, StaticOffsetFinder::retrieve_static_offset(StaticOffsetFinder::MemberOffsets::ByteMask_BoolProperty));
    }

    auto FBoolProperty::GetFieldMask() -> uint8_t
    {
        return Helper::Casting::offset_deref<uint8_t>(this, StaticOffsetFinder::retrieve_static_offset(StaticOffsetFinder::MemberOffsets::FieldMask_BoolProperty));
    }
}

