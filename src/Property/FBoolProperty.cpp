#include <Unreal/StaticOffsetFinder.hpp>
#include "Unreal/Property/FBoolProperty.h"
#include "Unreal/UnrealHashing.h"
#include <Unreal/FString.hpp>
#include <Unreal/VersionedContainer/Container.hpp>

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FBoolProperty);
    IMPLEMENT_VIRTUAL_FUNCTION(FBoolProperty, get_value_type_hash);

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

    auto FBoolProperty::get_value_type_hash_impl(const void* src) -> uint32
    {
        return get_type_hash(*(const bool*)src);
    }
}

