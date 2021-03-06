#include <Unreal/UClass.hpp>
#include <Unreal/TArray.hpp>
#include <Unreal/FProperty.hpp>
#include <Helpers/Casting.hpp>
#include <Unreal/UFunction.hpp>

namespace RC::Unreal
{
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UClass)
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UBlueprintGeneratedClass)
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UWidgetBlueprintGeneratedClass)
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UAnimBlueprintGeneratedClass)

    int32_t UClass::Offset::class_config_name{};

    auto UClass::HasAnyClassFlags(EClassFlags FlagsToCheck) const -> bool
    {
        return (GetClassFlags() & FlagsToCheck) != 0;
    }

    auto UClass::HasAllClassFlags(EClassFlags FlagsToCheck) const -> bool
    {
        return (GetClassFlags() & FlagsToCheck) == FlagsToCheck;
    }

    auto UClass::GetClassFlags() const -> EClassFlags
    {
        return Helper::Casting::offset_deref<EClassFlags>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UClass_ClassFlags));
    }

    auto UClass::GetClassWithin() const -> UClass*
    {
        return Helper::Casting::offset_deref<UClass*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UClass_ClassWithin));
    }

    auto UClass::GetClassConfigName() const -> FName
    {
        return Helper::Casting::offset_deref<FName>(this, Offset::class_config_name);
    }

    auto UClass::GetClassDefaultObject() const -> UObject*
    {
        return Helper::Casting::offset_deref<UObject*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UClass_ClassDefaultObject));
    }

    auto UClass::GetInterfaces() -> TArray<FImplementedInterface>&
    {
        return *Helper::Casting::ptr_cast<TArray<FImplementedInterface>*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UClass_Interfaces));
    }

    auto UClass::FindFunctionByName(FName FunctionName) -> UFunction* {
        UFunction* ResultFunction = nullptr;

        ForEachFunction([&](UFunction* Function) {
            if (Function->GetFName() == FunctionName) {
                ResultFunction = Function;
                return LoopAction::Break;
            }
            return LoopAction::Continue;
        });

        if (ResultFunction != nullptr) {
            return ResultFunction;
        }

        ForEachSuperStruct([&](UStruct* SuperStruct) {
            SuperStruct->ForEachFunction([&](UFunction* Function) {
                if (Function->GetFName() == FunctionName) {
                    ResultFunction = Function;
                    return LoopAction::Break;
                }
                return LoopAction::Continue;
            });
            if (ResultFunction != nullptr) {
                return LoopAction::Break;
            }
            return LoopAction::Continue;
        });
        return ResultFunction;
    }

    auto UClass::GetDefaultObject(bool bCreateIfNotFound) -> UObject* {
        UObject* ClassDefaultObject = GetClassDefaultObject();

        if (ClassDefaultObject == nullptr && bCreateIfNotFound) {
            check()
        }
        return ClassDefaultObject;
    }
}