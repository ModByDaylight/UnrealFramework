#include <Unreal/Property/FSetProperty.hpp>

namespace RC::Unreal
{
    FProperty* FSetProperty::GetElementProp()
    {
        return Helper::Casting::offset_deref<FProperty*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::SetProperty_ElementProp));
    }

    FSetProperty::FScriptSetLayout FSetProperty::GetSetLayout()
    {
        static_assert(false, "FSetProperty::GetSetLayout not implemented");
    }
}
