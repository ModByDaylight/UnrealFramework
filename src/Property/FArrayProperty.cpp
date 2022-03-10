#include <Unreal/Property/FArrayProperty.hpp>

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FArrayProperty);

    auto FArrayProperty::GetInner() -> FProperty*
    {
        return Helper::Casting::offset_deref<FProperty*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::ArrayProperty_Inner));
    }
}
