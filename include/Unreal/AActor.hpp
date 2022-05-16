#ifndef RC_UNREAL_AACTOR_HPP
#define RC_UNREAL_AACTOR_HPP

#include <Unreal/UObject.hpp>
#include <Unreal/Transform.hpp>
#include <Unreal/Rotator.hpp>

namespace RC::Unreal
{
    class UActorComponent;
    class USceneComponent;

    class RC_UE_API AActor : public UObject {
        DECLARE_EXTERNAL_OBJECT_CLASS(AActor, Engine)
    public:
        TArray<FName>& GetTags();

        FVector GetActorLocation() const;
        FVector GetActorForwardVector() const;
        FTransform GetTransform() const;
        bool TeleportTo(FVector DestLocation, FRotator DestRotation);

        void DestroyActor();
        bool HasAuthority() const;

        USceneComponent* GetRootComponent() const;
        TArray<UActorComponent*> GetComponentsByClass(UClass* ComponentClass) const;
        UActorComponent* FindComponentByClass(UClass* ComponentClass) const;

        UActorComponent* AddComponentByClass(UClass* ComponentClass, bool bManualAttachment, const FTransform& RelativeTransform, bool bDeferredFinish);
        void FinishAddComponent(UActorComponent* Component, bool bManualAttachment, const FTransform& RelativeTransform);

        void AttachToComponent(USceneComponent* Parent, FName SocketName, EAttachmentRule LocationRule, EAttachmentRule RotationRule, EAttachmentRule ScaleRule, bool bWeldSimulatedBodies = true);
    };
}


#endif //RC_UNREAL_AACTOR_HPP
