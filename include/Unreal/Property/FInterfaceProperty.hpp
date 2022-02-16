#ifndef RC_UNREAL_FINTERFACEPROPERTY_HPP
#define RC_UNREAL_FINTERFACEPROPERTY_HPP

#include <Unreal/FProperty.hpp>

namespace RC::Unreal
{
    class RC_UE_API FInterfaceProperty : public FProperty
    {
        DECLARE_FIELD_CLASS(FInterfaceProperty);
        DECLARE_VIRTUAL_TYPE(FInterfaceProperty);

    public:
        UClass* GetInterfaceClass();
        void SetInterfaceClass(UClass* NewInterfaceClass);
    };
}


#endif //RC_UNREAL_FINTERFACEPROPERTY_HPP
