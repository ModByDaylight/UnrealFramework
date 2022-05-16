#include <Unreal/AActor.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UFunction.hpp>
#include <Unreal/World.hpp>

namespace RC::Unreal
{
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(AActor)

    USceneComponent* AActor::GetRootComponent() const {
        return *GetValuePtrByPropertyName<USceneComponent*>(STR("RootComponent"));
    }

    TArray<UActorComponent*> AActor::GetComponentsByClass(UClass* ComponentClass) const {
        static UFunction* Function = GetClass()->FindFunctionByName(FName(STR("K2_GetComponentsByClass"), FNAME_Add));

        struct GetComponentsByClassParams {
            RC::Unreal::UClass* ComponentClass{nullptr};
            RC::Unreal::TArray<RC::Unreal::UActorComponent*> ReturnValue{};
        };
        GetComponentsByClassParams Params{
            .ComponentClass = ComponentClass
        };
        ProcessEvent(Function, &Params);
        return Params.ReturnValue;
    }

    UActorComponent* AActor::FindComponentByClass(UClass* ComponentClass) const {
        TArray<UActorComponent*> AllComponents = GetComponentsByClass(ComponentClass);
        if (AllComponents.Num()) {
            return AllComponents[0];
        }
        return nullptr;
    }

    UActorComponent* AActor::AddComponentByClass(UClass* ComponentClass, bool bManualAttachment, const FTransform& RelativeTransform, bool bDeferredFinish) {
        static UFunction* Function = GetClass()->FindFunctionByName(STR("AddComponentByClass"));

        struct AddComponentByClassParams {
            UClass* ComponentClass{};
            bool bManualAttachment{};
            FTransform RelativeTransform{};
            bool bDeferredFinish{};
            UActorComponent* ReturnValue{};
        };
        AddComponentByClassParams Params{
            .ComponentClass = ComponentClass,
            .bManualAttachment = bManualAttachment,
            .RelativeTransform = RelativeTransform,
            .bDeferredFinish = bDeferredFinish
        };
        ProcessEvent(Function, &Params);
        return Params.ReturnValue;
    }

    void AActor::FinishAddComponent(UActorComponent* Component, bool bManualAttachment, const FTransform& RelativeTransform) {
        static UFunction* Function = GetClass()->FindFunctionByName(STR("FinishAddComponent"));

        struct FinishAddComponentParams {
            UActorComponent* Component{};
            bool bManualAttachment{};
            FTransform RelativeTransform{};
        };
        FinishAddComponentParams Params{
            .Component = Component,
            .bManualAttachment = bManualAttachment,
            .RelativeTransform = RelativeTransform
        };
        ProcessEvent(Function, &Params);
    }

    void AActor::AttachToComponent(USceneComponent* Parent, FName SocketName, EAttachmentRule LocationRule, EAttachmentRule RotationRule, EAttachmentRule ScaleRule, bool bWeldSimulatedBodies) {
        static UFunction* Function = GetClass()->FindFunctionByName(STR("K2_AttachToComponent"));

        struct AttachToComponentParams {
            USceneComponent* Parent{};
            FName SocketName{};
            EAttachmentRule LocationRule{};
            EAttachmentRule RotationRule{};
            EAttachmentRule ScaleRule{};
            bool bWeldSimulatedBodies{};
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
    }

    void AActor::DestroyActor() {
        static UFunction* Function = GetClass()->FindFunctionByName(STR("K2_DestroyActor"));

        struct DestroyActorParams {
        };
        DestroyActorParams Params{};
        ProcessEvent(Function, &Params);
    }

    bool AActor::HasAuthority() const {
        static UFunction* Function = GetClass()->FindFunctionByName(STR("HasAuthority"));

        struct HasAuthorityParams {
            bool ReturnValue{};
        };
        HasAuthorityParams Params;
        ProcessEvent(Function, &Params);
        return Params.ReturnValue;
    }

    bool AActor::TeleportTo(FVector DestLocation, FRotator DestRotation)  {
        static UFunction* Function = GetClass()->FindFunctionByName(STR("K2_TeleportTo"));

        struct K2TeleportToParams {
            FVector DestLocation;
            FRotator DestRotation;
            bool ReturnValue{};
        };
        K2TeleportToParams Params{
            .DestLocation = DestLocation,
            .DestRotation = DestRotation,
        };
        ProcessEvent(Function, &Params);
        return Params.ReturnValue;
    }

    FVector AActor::GetActorLocation() const {
        UFunction* Function = GetClass()->FindFunctionByName(STR("K2_GetActorLocation"));

        struct K2_GetActorLocation_Params {
            FVector ReturnValue;
        };
        K2_GetActorLocation_Params Params{};

        ProcessEvent(Function, &Params);
        return Params.ReturnValue;
    }

    FVector AActor::GetActorForwardVector() const {
        static UFunction* Function = GetClass()->FindFunctionByName(STR("GetActorForwardVector"));

        struct GetActorForwardVector_Params {
            FVector ReturnValue;
        };
        GetActorForwardVector_Params Params{};

        ProcessEvent(Function, &Params);
        return Params.ReturnValue;
    }

    FTransform AActor::GetTransform() const {
        static UFunction* Function = GetClass()->FindFunctionByName(STR("GetTransform"));

        struct GetTransform_Params {
            FTransform ReturnValue;
        };
        GetTransform_Params Params{};

        ProcessEvent(Function, &Params);
        return Params.ReturnValue;
    }

    TArray<FName>& AActor::GetTags() {
        return *GetValuePtrByPropertyName<TArray<FName>>(STR("Tags"));
    }
}
