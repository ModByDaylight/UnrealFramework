#ifndef RC_UNREAL_FFIELDPATHPROPERTY_HPP
#define RC_UNREAL_FFIELDPATHPROPERTY_HPP

#include <Unreal/FProperty.hpp>

namespace RC::Unreal
{
    class RC_UE_API FFieldPathProperty : public FProperty
    {
        DECLARE_FIELD_CLASS(FFieldPathProperty);
        DECLARE_VIRTUAL_TYPE(FFieldPathProperty);

    public:
        auto GetPropertyClass() -> FFieldClass*;
    };
}


#endif //RC_UNREAL_FFIELDPATHPROPERTY_HPP
