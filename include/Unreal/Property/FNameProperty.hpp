#ifndef RC_UNREAL_FNAMEPROPERTY_HPP
#define RC_UNREAL_FNAMEPROPERTY_HPP

#include <Unreal/FProperty.hpp>

namespace RC::Unreal
{
    class RC_UE_API FNameProperty : public FProperty
    {
        DECLARE_FIELD_CLASS(FNameProperty);
        DECLARE_VIRTUAL_TYPE(FNameProperty);
    };

    IMPLEMENT_FIELD_CLASS(FNameProperty)
}


#endif //RC_UNREAL_FNAMEPROPERTY_HPP
