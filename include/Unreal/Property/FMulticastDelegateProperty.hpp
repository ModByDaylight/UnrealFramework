#ifndef RC_UNREAL_FMULTICASTDELEGATEPROPERTY_HPP
#define RC_UNREAL_FMULTICASTDELEGATEPROPERTY_HPP

#include <Unreal/FProperty.hpp>
#include <Unreal/Property/FDelegateProperty.hpp>
#include <Unreal/TArray.hpp>

namespace RC::Unreal
{
    class RC_UE_API TMulticastScriptDelegate
    {
    private:
        using FInvocationList = TArray<TScriptDelegate>;

    public:
        FInvocationList invocation_list;
    };
    using FMulticastScriptDelegate = TMulticastScriptDelegate/*<>*/;

    class RC_UE_API FMulticastDelegateProperty : public FProperty
    {
        DECLARE_FIELD_CLASS(FMulticastDelegateProperty);
        DECLARE_VIRTUAL_TYPE(FMulticastDelegateProperty);

    public:
#include <VTableOffsets_FMulticastDelegateProperty.hpp>

    public:
        auto GetFunctionSignature() -> UFunction*;

    // Virtual Functions
    public:
        const FMulticastScriptDelegate* GetMulticastDelegate(const void* PropertyValue) const;
        void SetMulticastDelegate(void* PropertyValue, FMulticastScriptDelegate ScriptDelegate) const;
        void AddDelegate(FScriptDelegate ScriptDelegate, UObject* Parent = nullptr, void* PropertyValue = nullptr) const;
        void RemoveDelegate(const FScriptDelegate& ScriptDelegate, UObject* Parent = nullptr, void* PropertyValue = nullptr) const;
        void ClearDelegate(UObject* Parent = nullptr, void* PropertyValue = nullptr) const;
    };
}

#endif //RC_UNREAL_FMULTICASTDELEGATEPROPERTY_HPP
