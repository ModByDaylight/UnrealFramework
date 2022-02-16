#ifndef RC_UNREAL_FCLASSPROPERTY_HPP
#define RC_UNREAL_FCLASSPROPERTY_HPP

#include <Unreal/Property/FObjectProperty.hpp>

namespace RC::Unreal
{
    class UClass;

    class RC_UE_API FClassProperty : public FObjectProperty
    {
        DECLARE_FIELD_CLASS(FClassProperty);
        DECLARE_VIRTUAL_TYPE(FClassProperty);

    public:
        UClass* GetMetaClass();
        void SetMetaClass(UClass* NewMetaClass);
    };
}


#endif //RC_UNREAL_FCLASSPROPERTY_HPP
