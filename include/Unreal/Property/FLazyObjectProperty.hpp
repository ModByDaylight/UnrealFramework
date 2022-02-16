#ifndef RC_UNREAL_FLAZYOBJECTPROPERTY_HPP
#define RC_UNREAL_FLAZYOBJECTPROPERTY_HPP

#include <Unreal/Property/FObjectProperty.hpp>

namespace RC::Unreal
{
    class RC_UE_API FLazyObjectProperty : public FObjectProperty
    {
        DECLARE_FIELD_CLASS(FLazyObjectProperty);
        DECLARE_VIRTUAL_TYPE(FLazyObjectProperty);

    public:
    };
}


#endif //RC_UNREAL_FLAZYOBJECTPROPERTY_HPP
