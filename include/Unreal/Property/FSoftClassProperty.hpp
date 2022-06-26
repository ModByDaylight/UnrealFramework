#ifndef RC_UNREAL_FSOFTCLASSPROPERTY_HPP
#define RC_UNREAL_FSOFTCLASSPROPERTY_HPP

#include <Unreal/Property/FSoftObjectProperty.hpp>

namespace RC::Unreal
{
    class RC_UE_API FSoftClassProperty : public FSoftObjectProperty
    {
        DECLARE_FIELD_CLASS(FSoftClassProperty);
        DECLARE_VIRTUAL_TYPE(FSoftClassProperty);

    public:
        UClass* GetMetaClass() const;
        void SetMetaClass(UClass* NewMetaClass);
    };
}


#endif //RC_UNREAL_FSOFTCLASSPROPERTY_HPP
