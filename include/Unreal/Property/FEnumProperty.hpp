#ifndef RC_UNREAL_XENUMPROPERTY_HPP
#define RC_UNREAL_XENUMPROPERTY_HPP

#include <Unreal/FProperty.hpp>
#include <Unreal/UnrealVersion.hpp>

namespace RC::Unreal
{
    class FNumericProperty;
    class UEnum;

    class RC_UE_API FEnumProperty : public FProperty
    {
        DECLARE_FIELD_CLASS(FEnumProperty);
        DECLARE_VIRTUAL_TYPE(FEnumProperty);

    public:
        auto GetUnderlyingProperty() -> FNumericProperty*;
        auto GetEnum() -> UEnum*;
    };
}


#endif //RC_UNREAL_XENUMPROPERTY_HPP
