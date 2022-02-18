#ifndef RC_UNREAL_FFIELDPATHPROPERTY_HPP
#define RC_UNREAL_FFIELDPATHPROPERTY_HPP

#include <Unreal/FProperty.hpp>
#include <Unreal/TProperty.hpp>
#include <Unreal/FWeakObjectPtr.hpp>
#include <Unreal/NameTypes.hpp>
#include <Unreal/TArray.hpp>

namespace RC::Unreal
{
    struct FFieldPath
    {
        mutable FField* ResolvedField = nullptr;
        mutable /*TWeakObjectPtr<UStruct>*/FWeakObjectPtr ResolvedOwner;
        TArray<FName> Path;
    };

    class RC_UE_API FFieldPathProperty : public TProperty<FFieldPath, FProperty>
    {
        DECLARE_FIELD_CLASS(FFieldPathProperty);
        DECLARE_VIRTUAL_TYPE(FFieldPathProperty);

    public:
        auto GetPropertyClass() -> FFieldClass*;
    };
}


#endif //RC_UNREAL_FFIELDPATHPROPERTY_HPP
