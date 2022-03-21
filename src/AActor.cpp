#include <Unreal/AActor.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UObjectArray.hpp>
#include <Unreal/UFunction.hpp>

namespace RC::Unreal
{
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(AActor)

    auto AActor::GetLevel() -> UObject*
    {
        UObject* Outer = GetOuter();
        if (!Outer) { return nullptr; }

        if (!HasAnyFlags(Unreal::RF_ClassDefaultObject) && !Outer->HasAnyFlags(Unreal::RF_BeginDestroyed) && !Outer->GetObjectItem()->IsUnreachable())
        {
            while (Outer)
            {
                if (Outer->GetClass()->GetFName() == TypeChecker::get_level_name())
                {
                    return Outer;
                }

                Outer = Outer->GetOuter();
            }
        }

        return nullptr;
    }

    auto AActor::GetWorld() -> UObject*
    {
        UObject* Outer = GetOuter();
        if (!Outer) { return nullptr; }

        if (!HasAnyFlags(Unreal::RF_ClassDefaultObject) && !Outer->HasAnyFlags(Unreal::RF_BeginDestroyed) && !Outer->GetObjectItem()->IsUnreachable())
        {
            while (Outer)
            {
                if (Outer->GetClass()->GetFName() == TypeChecker::get_world_name())
                {
                    return Outer;
                }

                Outer = Outer->GetOuter();
            }
        }

        return nullptr;
    }

    auto AActor::K2_TeleportTo(FVector DestLocation, FRotator DestRotation) -> bool
    {
        UFunction* Function = UObjectGlobals::StaticFindObject<UFunction*>(nullptr, nullptr, L"/Script/Engine.Actor:K2_TeleportTo");

        struct K2_TeleportTo_Params
        {
            FVector DestLocation;
            FRotator DestRotation;
            bool ReturnValue;
        };
        K2_TeleportTo_Params Params{
            .DestLocation = DestLocation,
            .DestRotation = DestRotation,
        };

        ProcessEvent(Function, &Params);
        return Params.ReturnValue;
    }

    auto AActor::K2_GetActorLocation() -> FVector
    {
        UFunction* Function = UObjectGlobals::StaticFindObject<UFunction*>(nullptr, nullptr, L"/Script/Engine.Actor:K2_GetActorLocation");

        struct K2_GetActorLocation_Params
        {
            FVector ReturnValue;
        };
        K2_GetActorLocation_Params Params{};

        ProcessEvent(Function, &Params);
        return Params.ReturnValue;
    }

    auto AActor::GetActorForwardVector() -> FVector
    {
        UFunction* Function = UObjectGlobals::StaticFindObject<UFunction*>(nullptr, nullptr, L"/Script/Engine.Actor:GetActorForwardVector");

        struct GetActorForwardVector_Params
        {
            FVector ReturnValue;
        };
        GetActorForwardVector_Params Params{};

        ProcessEvent(Function, &Params);
        return Params.ReturnValue;
    }
}
