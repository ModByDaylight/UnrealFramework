#include <Unreal/GameplayStatics.hpp>
#include <Unreal/AActor.hpp>
#include <Unreal/UClass.hpp>
#include <DynamicOutput/DynamicOutput.hpp>

namespace RC::Unreal
{
    auto UGameplayStatics::VerifySelf() -> bool
    {
        if (!Self)
        {
            Self = UObjectGlobals::StaticFindObject<UGameplayStatics*>(nullptr, nullptr, STR("/Script/Engine.Default__GameplayStatics"));
            if (!Self)
            {
                Output::send(STR("Was unable to locate GameplayStatics\n"));
                return false;
            }
        }

        return true;
    }

    AActor* UGameplayStatics::BeginDeferredActorSpawnFromClass(const UObject* WorldContextObject,
                                                               class UClass* ActorClass,
                                                               const FTransform& SpawnTransform,
                                                               ESpawnActorCollisionHandlingMethod CollisionHandlingOverride,
                                                               AActor* Owner)
    {
        if (!VerifySelf()) { return nullptr; }
        if (!StaticFunctions::BeginDeferredActorSpawnFromClass.IsValid()) { return nullptr; }

        BeginDeferredActorSpawnFromClass_Params Params{
                .WorldContextObject = WorldContextObject,
                .ActorClass = ActorClass,
                .SpawnTransform = SpawnTransform,
                .CollisionHandlingOverride = CollisionHandlingOverride,
                .Owner = Owner,
        };
        StaticFunctions::BeginDeferredActorSpawnFromClass(Params);
        return Params.ReturnValue;
    }

    AActor* UGameplayStatics::FinishSpawningActor(class AActor* Actor, const FTransform& SpawnTransform)
    {
        if (!VerifySelf()) { return nullptr; }
        if (!StaticFunctions::FinishSpawningActor.IsValid()) { return nullptr; }

        FinishSpawningActor_Params Params{
                .Actor = Actor,
                .SpawnTransform = SpawnTransform,
        };
        StaticFunctions::FinishSpawningActor(Params);
        return Params.ReturnValue;
    }
}
