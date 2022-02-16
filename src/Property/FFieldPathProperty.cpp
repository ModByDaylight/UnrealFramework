#include <Unreal/Property/FFieldPathProperty.hpp>

namespace RC::Unreal
{
    auto FFieldPathProperty::GetPropertyClass() -> FFieldClass*
    {
        // TODO: Stop relying on an unrelated offset that just coincidentally happens to be the same as the right one
        return Helper::Casting::offset_deref<FFieldClass*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::ObjectProperty_PropertyClass));
    }
}
