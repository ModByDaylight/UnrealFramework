#ifndef RC_UNREAL_AACTOR_HPP
#define RC_UNREAL_AACTOR_HPP

#include <Unreal/UObject.hpp>
#include <Unreal/FVector.hpp>
#include <Unreal/FRotator.hpp>

namespace RC::Unreal
{
    class RC_UE_API AActor : public UObject
    {
    DECLARE_EXTERNAL_OBJECT_CLASS(AActor, Engine)

    public:
        auto get_level() -> UObject*;
        auto get_world() -> UObject*;

    public:
        // UFunction interfaces -> START
        auto K2_TeleportTo(FVector DestLocation, FRotator DestRotation) -> bool;
        auto K2_GetActorLocation() -> FVector;
        auto GetActorForwardVector() -> FVector;
        // UFunction interfaces -> END
    };
}


#endif //RC_UNREAL_AACTOR_HPP
