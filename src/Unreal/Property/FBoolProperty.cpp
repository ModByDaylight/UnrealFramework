#include <Unreal/StaticOffsetFinder.hpp>
#include <Unreal/Property/FBoolProperty.hpp>
#include <Unreal/UnrealHashing.hpp>
#include <Helpers/Casting.hpp>

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FBoolProperty);

    auto FBoolProperty::GetByteOffset() const -> uint8_t
    {
        return Helper::Casting::offset_deref<uint8_t>(this, StaticOffsetFinder::retrieve_static_offset(StaticOffsetFinder::MemberOffsets::ByteOffset_BoolProperty));
    }

    auto FBoolProperty::GetByteMask() const -> uint8_t
    {
        return Helper::Casting::offset_deref<uint8_t>(this, StaticOffsetFinder::retrieve_static_offset(StaticOffsetFinder::MemberOffsets::ByteMask_BoolProperty));
    }

    auto FBoolProperty::GetFieldMask() const -> uint8_t
    {
        return Helper::Casting::offset_deref<uint8_t>(this, StaticOffsetFinder::retrieve_static_offset(StaticOffsetFinder::MemberOffsets::FieldMask_BoolProperty));
    }
}

