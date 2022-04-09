#include <Unreal/Property/FMapProperty.hpp>
#include <Helpers/Casting.hpp>

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FMapProperty);

    auto FMapProperty::GetKeyProp() -> FProperty*
    {
        return Helper::Casting::offset_deref<FProperty*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::MapProperty_KeyProp));
    }

    auto FMapProperty::GetValueProp() -> FProperty*
    {
        return Helper::Casting::offset_deref<FProperty*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::MapProperty_ValueProp));
    }

    /*
    FMapProperty::FScriptMapLayout FMapProperty::GetMapLayout()
    {
        static_assert(false, "FMapProperty::GetMapLayout not implemented");
    }

    FMapProperty::EMapPropertyFlags FMapProperty::GetMapFlags()
    {
        static_assert(false, "FMapProperty::GetMapFlags not implemented");
    }
    //*/
}
