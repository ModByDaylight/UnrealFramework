#include <Unreal/UStruct.hpp>
#include <Unreal/FField.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UFunction.hpp>
#include <Unreal/FProperty.hpp>
#include <Helpers/Casting.hpp>

namespace RC::Unreal
{
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UStruct);

    using MemberOffsets = ::RC::Unreal::StaticOffsetFinder::MemberOffsets;

    Function<UStruct::LinkSignature> UStruct::LinkInternal;
    std::unordered_map<std::wstring, uint32_t> UStruct::VTableLayoutMap;

#include <MemberVariableLayout_SrcWrapper_UStruct.hpp>

    UStruct* UStruct::GetInheritanceSuper() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UStruct, GetInheritanceSuper, UStruct*)
    }

    void UStruct::Link(FArchive& Ar, bool bRelinkExistingProperties)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UStruct, Link, void, PARAMS(bool), ARGS(bRelinkExistingProperties))
    }

    void UStruct::SerializeBin(FArchive& Ar, void* Data) const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UStruct, SerializeBin, void, PARAMS(void*), ARGS(Data))

    }

    void UStruct::SerializeTaggedProperties(FArchive& Ar, uint8* Data, UStruct* DefaultsStruct, uint8* Defaults, const UObject* BreakRecursionIfFullyLoad) const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UStruct,
                                         SerializeTaggedProperties,
                                         void,
                                         PARAMS(FArchive&, uint8*, UStruct*, uint8*, const UObject*),
                                         ARGS(Ar, Data, DefaultsStruct, Defaults, BreakRecursionIfFullyLoad))
    }

    void UStruct::InitializeStruct(void* Dest, int32 ArrayDim) const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UStruct, InitializeStruct, void, PARAMS(void*, int32), PARAMS(Dest, ArrayDim))
    }

    void UStruct::DestroyStruct(void* Dest, int32 ArrayDim) const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UStruct, DestroyStruct, void, PARAMS(void*, int32), ARGS(Dest, ArrayDim))
    }

    FProperty* UStruct::CustomFindProperty(const FName InName) const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UStruct, CustomFindProperty, FProperty*, PARAMS(const FName), ARGS(InName))
    }

    EExprToken UStruct::SerializeExpr(int32& iCode, FArchive& Ar)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UStruct, SerializeExpr, EExprToken, PARAMS(int32&, FArchive&), ARGS(iCode, Ar))
    }

    const TCHAR* UStruct::GetPrefixCPP() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UStruct, GetPrefixCPP, const TCHAR*)
    }

    void UStruct::SetSuperStruct(UStruct* NewSuperStruct)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UStruct, SetSuperStruct, void, PARAMS(UStruct*), ARGS(NewSuperStruct))
    }

    FString UStruct::PropertyNameToDisplayName(FName InName) const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UStruct, PropertyNameToDisplayName, FString, PARAMS(FName), ARGS(InName))
    }

    FString UStruct::GetAuthoredNameForField(const UField* Field) const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UStruct, GetAuthoredNameForField, FString, PARAMS(const UField*), ARGS(Field))
    }

    bool UStruct::IsStructTrashed() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UStruct, IsStructTrashed, bool)
    }

    FName UStruct::FindPropertyNameFromGuid(const FGuid& PropertyGuid) const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UStruct, FindPropertyNameFromGuid, FName, PARAMS(const FGuid&), ARGS(PropertyGuid))
    }

    FGuid UStruct::FindPropertyGuidFromName(const FName InName) const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UStruct, FindPropertyGuidFromName, FGuid, PARAMS(const FName), ARGS(InName))
    }

    bool UStruct::ArePropertyGuidsAvailable() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UStruct, ArePropertyGuidsAvailable, bool)
    }

    static bool IsChildOfInternal(const UStruct* This, const UStruct* Struct)
    {
        auto* CurrentStruct = This;
        do {
            if (CurrentStruct == Struct)
            {
                return true;
            }
            CurrentStruct = CurrentStruct->GetSuperStruct();
        }
        while (CurrentStruct);
        return false;
    }

    auto UStruct::IsChildOf(const UStruct* Struct) const -> bool
    {
        return IsChildOfInternal(this, Struct);
    }

    auto UStruct::IsChildOf(UStruct* Struct) const -> bool
    {
        return IsChildOfInternal(this, Struct);
    }

    auto UStruct::ForEachFunction(const std::function<LoopAction(UFunction*)>& Callable) -> void
    {
        UField* CurrentField = GetChildren();

        while (CurrentField != nullptr) {
            //Only trigger the callable on UFunction objects
            if (UFunction* Function = Cast<UFunction>(CurrentField)) {
                LoopAction LoopAction = Callable(Function);

                if (LoopAction == LoopAction::Break) {
                    break;
                }
            }
            CurrentField = CurrentField->GetNext();
        }
    }

    auto UStruct::ForEachProperty(const std::function<LoopAction(FProperty* Property)>& Callable) -> void
    {
        if constexpr(Version::IsBelow(4, 25))
        {
            UField* CurrentField = GetChildren();

            while (CurrentField != nullptr) {
                FField* CurrentFieldAsFField = CurrentField->AsFFieldUnsafe();

                //Only trigger the callable on UProperty objects
                if (FProperty* Property = CastField<FProperty>(CurrentFieldAsFField)) {
                    LoopAction LoopAction = Callable(Property);

                    if (LoopAction == LoopAction::Break) {
                        break;
                    }
                }
                CurrentField = CurrentField->GetNext();
            }
        }
        else
        {
            FField* CurrentField = GetChildProperties();

            while (CurrentField != nullptr) {
                //Only trigger the callable on FProperty objects
                if (FProperty* Property = CastField<FProperty>(CurrentField)) {
                    LoopAction LoopAction = Callable(Property);

                    if (LoopAction == LoopAction::Break) {
                        break;
                    }
                }
                CurrentField = CurrentField->GetNextFFieldUnsafe();
            }
        }
    }

    auto UStruct::ForEachSuperStruct(const ForEachSuperStructCallable& Callable) -> void
    {
        UStruct* SuperStruct = GetSuperStruct();
        LoopAction LoopAction{};

        while (SuperStruct)
        {
            LoopAction = Callable(SuperStruct);
            if (LoopAction == LoopAction::Break) { break; }

            SuperStruct = SuperStruct->GetSuperStruct();
        }
    }

    auto UStruct::ForEachPropertyInChain(const ForEachPropertyInChainCallable& Callable) -> void
    {
        UStruct* Class = this;

        while (Class)
        {
            bool bShouldOuterLoopBreak{};

            Class->ForEachProperty([&](FProperty* child) {
                if (Callable(child) == LoopAction::Break)
                {
                    bShouldOuterLoopBreak = true;
                    return LoopAction::Break;
                }
                else
                {
                    return LoopAction::Continue;
                }
            });

            if (bShouldOuterLoopBreak) { break; }

            Class->ForEachSuperStruct([&](UStruct* SuperStruct) {
                if (!SuperStruct) { return LoopAction::Continue; }

                SuperStruct->ForEachProperty([&](FProperty* Property) {
                    auto loop_action = Callable(Property);
                    if (loop_action == LoopAction::Break)
                    {
                        bShouldOuterLoopBreak = true;
                        return LoopAction::Break;
                    }
                    else
                    {
                        return LoopAction::Continue;
                    }
                });

                if (bShouldOuterLoopBreak) { return LoopAction::Break; }

                return LoopAction::Continue;
            });

            if (bShouldOuterLoopBreak) { break; }

            UStruct* NextClass = Class->GetClassPrivate();
            if (NextClass != Class)
            {
                Class = NextClass;
            }
            else
            {
                break;
            }
        }
    }

    FProperty* UStruct::FindProperty(FName PropertyName)
    {
        FProperty* PropertyFound = nullptr;
        ForEachPropertyInChain([&](FProperty* Property) {
            if (Property->GetFName() == PropertyName)
            {
                PropertyFound = Property;
                return LoopAction::Break;
            }
            else
            {
                return LoopAction::Continue;
            }
        });

        return PropertyFound;
    }

    bool UStruct::HasChildren()
    {
        if constexpr(Version::IsBelow(4, 25))
        {
            return GetChildren();
        }
        else
        {
            return GetChildProperties() || GetChildren();
        }
    }

    FProperty* UStruct::GetFirstProperty()
    {
        if constexpr(Version::IsBelow(4, 25))
        {
            // In <4.25, this is safe if a UField is a property, which 'CastField' checks
            // In <4.25, all properties are of type UField
            return CastField<FProperty>(std::bit_cast<FField*>(GetChildren()));
        }
        else
        {
            return CastField<FProperty>(GetChildProperties());
        }
    }
}