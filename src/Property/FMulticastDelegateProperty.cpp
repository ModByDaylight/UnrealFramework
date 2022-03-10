#include <Unreal/Property/FMulticastDelegateProperty.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtualBaseVC.hpp>

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FMulticastDelegateProperty);

    auto FMulticastDelegateProperty::GetFunctionSignature() -> UFunction*
    {
        return Helper::Casting::offset_deref<UFunction*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::MulticastDelegateProperty_SignatureFunction));
    }

    const FMulticastScriptDelegate* FMulticastDelegateProperty::GetMulticastDelegate(const void* PropertyValue) const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FMulticastDelegateProperty,
                                         GetMulticastDelegate,
                                         FMulticastScriptDelegate*,
                                         PARAMS(const void*),
                                         ARGS(PropertyValue))
    }
    void FMulticastDelegateProperty::SetMulticastDelegate(void* PropertyValue, FMulticastScriptDelegate ScriptDelegate) const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FMulticastDelegateProperty,
                                         SetMulticastDelegate,
                                         void,
                                         PARAMS(void*, FMulticastScriptDelegate),
                                         ARGS(PropertyValue, ScriptDelegate))
    }

    void FMulticastDelegateProperty::AddDelegate(FScriptDelegate ScriptDelegate, UObject* Parent, void* PropertyValue) const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FMulticastDelegateProperty,
                                         AddDelegate,
                                         void,
                                         PARAMS(FScriptDelegate, UObject*, void*),
                                         ARGS(ScriptDelegate, Parent, PropertyValue))
    }

    void FMulticastDelegateProperty::RemoveDelegate(const FScriptDelegate& ScriptDelegate, UObject* Parent, void* PropertyValue) const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FMulticastDelegateProperty,
                                         RemoveDelegate,
                                         void,
                                         PARAMS(const FScriptDelegate&, UObject*, void*),
                                         ARGS(ScriptDelegate, Parent, PropertyValue))
    }

    void FMulticastDelegateProperty::ClearDelegate(UObject* Parent, void* PropertyValue) const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FMulticastDelegateProperty,
                                         ClearDelegate,
                                         void,
                                         PARAMS(UObject*, void*),
                                         ARGS(Parent, PropertyValue))
    }
}