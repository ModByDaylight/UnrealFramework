#include <Unreal/Property/FSetProperty.hpp>
#include <Helpers/Casting.hpp>

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FSetProperty);

    FProperty* FSetProperty::GetElementProp()
    {
        return Helper::Casting::offset_deref<FProperty*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::SetProperty_ElementProp));
    }

    /*
    FSetProperty::FScriptSetLayout FSetProperty::GetSetLayout()
    {
        static_assert(false, "FSetProperty::GetSetLayout not implemented");
    }
    //*/
}
