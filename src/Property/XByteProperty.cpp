#include <format>
#include <Unreal/Property/XByteProperty.hpp>

namespace RC::Unreal
{
    auto XByteProperty::get_enum() -> UEnum*
    {
        return Helper::Casting::offset_deref<UEnum*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::ByteProperty_Enum));
    }
}
