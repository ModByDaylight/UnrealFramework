#ifndef RC_UNREAL_FOBJECTPROPERTY_HPP
#define RC_UNREAL_FOBJECTPROPERTY_HPP

#include <Unreal/FProperty.hpp>
#include <Unreal/TProperty.hpp>

namespace RC::Unreal
{
    class RC_UE_API FObjectPropertyBase : public FProperty
    {
        DECLARE_FIELD_CLASS(FObjectPropertyBase);
        DECLARE_VIRTUAL_TYPE(FObjectPropertyBase);

    public:
#include <VTableOffsets_FObjectPropertyBase.hpp>

    public:
        UClass* GetPropertyClass();
        void SetPropertyClass(UClass* NewPropertyClass);

        // Virtual Functions
    public:
        FString GetCPPTypeCustom(FString* ExtendedTypeText, uint32 CPPExportFlags, const FString& InnerNativeTypeName) const;
        UObject* LoadObjectPropertyValue(const void* PropertyValueAddress) const;
        UObject* GetObjectPropertyValue(const void* PropertyValueAddress) const;
        void SetObjectPropertyValue(void* PropertyValueAddress, UObject* Value) const;
        bool AllowCrossLevel() const;
        void CheckValidObject(void* Value) const;
    };

    template<typename InTCppType>
    class TFObjectPropertyBase : public TProperty<InTCppType, FObjectPropertyBase>
    {
    };

    class RC_UE_API FObjectProperty : public TFObjectPropertyBase<UObject*>
    {
        DECLARE_FIELD_CLASS(FObjectProperty);
        DECLARE_VIRTUAL_TYPE(FObjectProperty);
    };
}


#endif //RC_UNREAL_FOBJECTPROPERTY_HPP
