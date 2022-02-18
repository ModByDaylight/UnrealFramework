#ifndef RC_UNREAL_FINTERFACEPROPERTY_HPP
#define RC_UNREAL_FINTERFACEPROPERTY_HPP

#include <Unreal/FProperty.hpp>
#include <Unreal/TProperty.hpp>

namespace RC::Unreal
{
    class FScriptInterface
    {
        UObject* ObjectPointer;
        void* InterfacePointer;
    };

    class RC_UE_API FInterfaceProperty : public TProperty<FScriptInterface, FProperty>
    {
        DECLARE_FIELD_CLASS(FInterfaceProperty);
        DECLARE_VIRTUAL_TYPE(FInterfaceProperty);

    public:
        UClass* GetInterfaceClass();
        void SetInterfaceClass(UClass* NewInterfaceClass);
    };
}


#endif //RC_UNREAL_FINTERFACEPROPERTY_HPP
