#ifndef RC_UNREAL_XENUMPROPERTY_HPP
#define RC_UNREAL_XENUMPROPERTY_HPP

#include <Unreal/FProperty.hpp>
#include <Unreal/UnrealVersion.hpp>

namespace RC::Unreal
{
    class FNumericProperty;
    class UEnum;

    class FEnumProperty : public FProperty
    {
        DECLARE_FIELD_CLASS(FEnumProperty);
        DECLARE_VIRTUAL_TYPE(FEnumProperty);

    public:
        auto GetUnderlyingProperty() -> FNumericProperty*
        {
            return Helper::Casting::offset_deref<FNumericProperty*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::EnumProperty_UnderlyingType));
        }

        auto GetEnum() -> UEnum*
        {
            return Helper::Casting::offset_deref<UEnum*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::EnumProperty_Enum));
        }
    };
}


#endif //RC_UNREAL_XENUMPROPERTY_HPP
