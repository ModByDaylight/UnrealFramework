#ifndef RC_UNREAL_FSOFTOBJECTPROPERTY_HPP
#define RC_UNREAL_FSOFTOBJECTPROPERTY_HPP

#include <Unreal/Property/FObjectProperty.hpp>

namespace RC::Unreal
{
    class RC_UE_API FSoftObjectProperty : public FObjectProperty
    {
        DECLARE_FIELD_CLASS(FSoftObjectProperty);
        DECLARE_VIRTUAL_TYPE(FSoftObjectProperty);
    };
}


#endif //RC_UNREAL_FSOFTOBJECTPROPERTY_HPP
