#ifndef RC_UNREAL_FCLASSPROPERTY_HPP
#define RC_UNREAL_FCLASSPROPERTY_HPP

#include <Unreal/Property/XObjectProperty.hpp>

namespace RC::Unreal
{
    class UClass;

    class RC_UE_API FClassProperty : public XObjectProperty
    {
        DECLARE_FIELD_CLASS(FClassProperty);
        DECLARE_VIRTUAL_TYPE(FClassProperty);

    public:
        auto GetMetaClass() -> UClass*;
    };
}


#endif //RC_UNREAL_FCLASSPROPERTY_HPP
