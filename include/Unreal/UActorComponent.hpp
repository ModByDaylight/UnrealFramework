#ifndef RC_UNREAL_UACTORCOMPONENT_HPP
#define RC_UNREAL_UACTORCOMPONENT_HPP

#include <Unreal/Common.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/UnrealCoreStructs.hpp>
#include <Unreal/Transform.hpp>

namespace RC::Unreal
{
    class AActor;

    class RC_UE_API UActorComponent : public UObject {
        DECLARE_EXTERNAL_OBJECT_CLASS(UActorComponent, Engine);
    public:
        AActor* GetOwner() const;
        TArray<FName>& GetComponentTags();

        bool GetIsReplicated() const;
        void SetIsReplicated(bool ShouldReplicate);

        void DestroyComponent();
    };

    class RC_UE_API USceneComponent : public UActorComponent {
        DECLARE_EXTERNAL_OBJECT_CLASS(USceneComponent, Engine);
    public:
        FTransform GetComponentToWorld() const;

        FORCEINLINE FVector GetComponentLocation() const { return GetComponentToWorld().Translation; }
        FORCEINLINE FRotator GetComponentRotation() const { return GetComponentToWorld().Rotation.Rotator(); };
        FORCEINLINE FVector GetComponentScale() const { return GetComponentToWorld().Scale3D; };

        void SetRelativeLocationAndRotation(FVector NewLocation, FRotator NewRotation, bool bSweep, FHitResult* SweepHitResult, bool bTeleport);
        void SetWorldLocationAndRotation(FVector NewLocation, FRotator NewRotation, bool bSweep, FHitResult* SweepHitResult, bool bTeleport);

        USceneComponent* GetAttachParent() const;
        FName GetAttachSocketName() const;
        void GetParentComponents(TArray<USceneComponent*>& Parents) const;
        void GetChildrenComponents(bool bIncludeAllDescendants, TArray<USceneComponent*>& Children) const;

        /** Only call this when the component is NOT registered!!! Otherwise use AttachToComponent */
        void SetupAttachment(USceneComponent* InParent, FName InSocketName = NAME_None);

        bool AttachToComponent(USceneComponent* Parent, FName SocketName, EAttachmentRule LocationRule, EAttachmentRule RotationRule, EAttachmentRule ScaleRule, bool bWeldSimulatedBodies = true);
        void DetachFromComponent(EDetachmentRule LocationRule = EDetachmentRule::KeepRelative, EDetachmentRule RotationRule = EDetachmentRule::KeepRelative, EDetachmentRule ScaleRule = EDetachmentRule::KeepRelative, bool bCallModify = true);
    };
}

#endif //RC_UNREAL_UACTORCOMPONENT_HPP
