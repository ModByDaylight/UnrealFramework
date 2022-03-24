#include <Unreal/UClass.hpp>
#include <Unreal/TArray.hpp>
#include <Unreal/FProperty.hpp>
#include <Helpers/Casting.hpp>

namespace RC::Unreal
{
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UClass)
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UBlueprintGeneratedClass)
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UWidgetBlueprintGeneratedClass)
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UAnimBlueprintGeneratedClass)

    auto UClass::HasAnyClassFlags(EClassFlags FlagsToCheck) -> bool
    {
        return (GetClassFlags() & FlagsToCheck) != 0;
    }

    auto UClass::HasAllClassFlags(EClassFlags FlagsToCheck) -> bool
    {
        return (GetClassFlags() & FlagsToCheck) == FlagsToCheck;
    }

    auto UClass::GetClassFlags() -> EClassFlags
    {
        return Helper::Casting::offset_deref<EClassFlags>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UClass_ClassFlags));
    }

    auto UClass::GetClassWithin() -> UClass*
    {
        return Helper::Casting::offset_deref<UClass*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UClass_ClassWithin));
    }

    auto UClass::GetClassConfigName() -> FName
    {
        return Helper::Casting::offset_deref<FName>(this, Offset::class_config_name);
    }

    auto UClass::GetClassDefaultObject() -> UObject*
    {
        return Helper::Casting::offset_deref<UObject*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UClass_ClassDefaultObject));
    }

    auto UClass::GetInterfaces() -> TArray<FImplementedInterface>&
    {
        return *Helper::Casting::ptr_cast<TArray<FImplementedInterface>*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UClass_Interfaces));
    }
}