#include <Unreal/UActorComponent.hpp>
#include <Unreal/AActor.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/Property/FBoolProperty.hpp>

namespace RC::Unreal
{
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UActorComponent);

    AActor* UActorComponent::GetOwner() const {
        return GetTypedOuter<AActor>();
    }

    TArray<FName>& UActorComponent::GetComponentTags() {
        return *GetValuePtrByPropertyName<TArray<FName>>(STR("ComponentTags"));
    }

    void UActorComponent::DestroyComponent() {
        static UFunction* Function = GetClass()->FindFunctionByName(STR("K2_DestroyComponent"));

        struct DestroyComponentParams {
            UObject* Object{};
        };
        DestroyComponentParams Params{.Object = this};
        ProcessEvent(Function, &Params);
    }

    bool UActorComponent::GetIsReplicated() const {
        FBoolProperty* BoolProperty = CastField<FBoolProperty>(GetClass()->FindProperty(STR("bReplicates")));
        return BoolProperty->GetPropertyValueInContainer(this);
    }

    void UActorComponent::SetIsReplicated(bool ShouldReplicate) {
        static UFunction* Function = GetClass()->FindFunctionByName(STR("SetIsReplicated"));

        struct SetIsReplicatedParams {
            bool ShouldReplicate{};
        };
        SetIsReplicatedParams Params{ .ShouldReplicate = ShouldReplicate };
        ProcessEvent(Function, &Params);
    }

    IMPLEMENT_EXTERNAL_OBJECT_CLASS(USceneComponent);

    FTransform USceneComponent::GetComponentToWorld() const {
        return *GetValuePtrByPropertyName<FTransform>(STR("ComponentToWorld"));
    }

    USceneComponent* USceneComponent::GetAttachParent() const {
        return *GetValuePtrByPropertyName<USceneComponent*>(STR("AttachParent"));
    }

    FName USceneComponent::GetAttachSocketName() const {
        return *GetValuePtrByPropertyName<FName>(STR("AttachSocketName"));
    }

    void USceneComponent::SetRelativeLocationAndRotation(FVector NewLocation, FRotator NewRotation, bool bSweep, FHitResult* SweepHitResult, bool bTeleport) {
        static UFunction* Function = GetClass()->FindFunctionByName(STR("K2_SetRelativeLocationAndRotation"));

        struct SetRelativeLocationAndRotationParams {
            FVector NewLocation;
            FRotator NewRotation;
            bool bSweep{};
            FHitResult SweepHitResult;
            bool bTeleport{};
        };
        SetRelativeLocationAndRotationParams Params{
                .NewLocation = NewLocation,
                .NewRotation = NewRotation,
                .bSweep = bSweep,
                .bTeleport = bTeleport
        };
        ProcessEvent(Function, &Params);
        if (SweepHitResult != nullptr) {
            *SweepHitResult = Params.SweepHitResult;
        }
    }

    void USceneComponent::SetWorldLocationAndRotation(FVector NewLocation, FRotator NewRotation, bool bSweep, FHitResult* SweepHitResult, bool bTeleport) {
        static UFunction* Function = GetClass()->FindFunctionByName(STR("K2_SetWorldLocationAndRotation"));

        struct SetWorldLocationAndRotationParams {
            FVector NewLocation;
            FRotator NewRotation;
            bool bSweep{};
            FHitResult SweepHitResult;
            bool bTeleport{};
        };
        SetWorldLocationAndRotationParams Params{
            .NewLocation = NewLocation,
            .NewRotation = NewRotation,
            .bSweep = bSweep,
            .bTeleport = bTeleport
        };
        ProcessEvent(Function, &Params);
        if (SweepHitResult != nullptr) {
            *SweepHitResult = Params.SweepHitResult;
        }
    }

    void USceneComponent::GetParentComponents(TArray<USceneComponent*>& OutParents) const {
        USceneComponent* ParentIterator = GetAttachParent();
        while (ParentIterator != nullptr) {
            OutParents.Add(ParentIterator);
            ParentIterator = ParentIterator->GetAttachParent();
        }
    }

    void USceneComponent::GetChildrenComponents(bool bIncludeAllDescendants, TArray<USceneComponent*>& OutChildren) const {
        const TArray<USceneComponent*>& AttachedChildren = *GetValuePtrByPropertyName<TArray<USceneComponent*>>(STR("AttachedChildren"));

        for (USceneComponent* Child : AttachedChildren) {
            if (Child != nullptr) {
                OutChildren.Add(Child);
            }
        }
        if (bIncludeAllDescendants) {
            for (USceneComponent* Child : AttachedChildren) {
                if (Child != nullptr) {
                    Child->GetChildrenComponents(true, OutChildren);
                }
            }
        }
    }

    void USceneComponent::SetupAttachment(USceneComponent* InParent, FName InSocketName) {
        *GetValuePtrByPropertyName<USceneComponent*>(STR("AttachParent")) = InParent;
        *GetValuePtrByPropertyName<FName>(STR("AttachSocketName")) = InSocketName;

        FBoolProperty* ShouldBeAttached = CastField<FBoolProperty>(GetClass()->FindProperty(STR("bShouldBeAttached")));
        ShouldBeAttached->SetPropertyValueInContainer(this, InParent != nullptr);
    }

    void USceneComponent::DetachFromComponent(EDetachmentRule LocationRule, EDetachmentRule RotationRule, EDetachmentRule ScaleRule, bool bCallModify) {
        static UFunction* Function = GetClass()->FindFunctionByName(STR("K2_DetachFromComponent"));

        struct DetachFromComponentParams {
            EDetachmentRule LocationRule{};
            EDetachmentRule RotationRule{};
            EDetachmentRule ScaleRule{};
            bool bCallModify{};
        };
        DetachFromComponentParams Params{
            .LocationRule = LocationRule,
            .RotationRule = RotationRule,
            .ScaleRule = ScaleRule,
            .bCallModify = bCallModify
        };
        ProcessEvent(Function, &Params);
    }

    bool USceneComponent::AttachToComponent(USceneComponent *Parent, FName SocketName, EAttachmentRule LocationRule, EAttachmentRule RotationRule, EAttachmentRule ScaleRule, bool bWeldSimulatedBodies) {
        static UFunction* Function = GetClass()->FindFunctionByName(STR("K2_AttachToComponent"));

        struct AttachToComponentParams {
            USceneComponent* Parent{};
            FName SocketName{};
            EAttachmentRule LocationRule{};
            EAttachmentRule RotationRule{};
            EAttachmentRule ScaleRule{};
            bool bWeldSimulatedBodies{};
            bool ReturnValue{};
        };
        AttachToComponentParams Params{
                .Parent = Parent,
                .SocketName = SocketName,
                .LocationRule = LocationRule,
                .RotationRule = RotationRule,
                .ScaleRule = ScaleRule,
                .bWeldSimulatedBodies = bWeldSimulatedBodies
        };
        ProcessEvent(Function, &Params);
        return Params.ReturnValue;
    }
}
