#ifndef RC_UNREAL_XDELEGATEPROPERTY_HPP
#define RC_UNREAL_XDELEGATEPROPERTY_HPP

#include <Helpers/Format.hpp>

#include <Unreal/FProperty.hpp>
#include <Unreal/FWeakObjectPtr.hpp>
#include <Unreal/Property/FDelegateProperty.hpp>

namespace RC::Unreal
{
    class RC_UE_API TScriptDelegate
    {
    private:
        using TWeakPtr = FWeakObjectPtr;

    public:
        TWeakPtr object;
        FName function_name;
    };
    using FScriptDelegate = TScriptDelegate/*<>*/;

    class RC_UE_API FDelegateProperty : public FProperty
    {
        DECLARE_FIELD_CLASS(FDelegateProperty);
        DECLARE_VIRTUAL_TYPE(FDelegateProperty);

    public:
        auto GetFunctionSignature() -> class UFunction*;
    };
}


#endif //RC_UNREAL_XDELEGATEPROPERTY_HPP
