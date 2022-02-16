#include <Unreal/UClass.hpp>
#include <Unreal/TArray.hpp>
#include <Unreal/FProperty.hpp>

namespace RC::Unreal
{
    auto UClass::has_any_class_flags(EClassFlags flags_to_check) -> bool
    {
        return (get_class_flags() & flags_to_check) != 0;
    }

    auto UClass::has_all_class_flags(EClassFlags flags_to_check) -> bool
    {
        return (get_class_flags() & flags_to_check) == flags_to_check;
    }

    auto UClass::get_class_flags() -> EClassFlags
    {
        return Helper::Casting::offset_deref<EClassFlags>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UClass_ClassFlags));
    }

    auto UClass::get_class_within() -> UClass*
    {
        return Helper::Casting::offset_deref<UClass*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UClass_ClassWithin));
    }

    auto UClass::get_class_config_name() -> FName
    {
        return Helper::Casting::offset_deref<FName>(this, Offset::class_config_name);
    }

    auto UClass::get_class_default_object() -> UObject*
    {
        return Helper::Casting::offset_deref<UObject*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UClass_ClassDefaultObject));
    }

    auto UClass::get_interfaces() -> TArray<FImplementedInterface>
    {
        return Helper::Casting::offset_deref<TArray<FImplementedInterface>>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UClass_Interfaces));
    }
}