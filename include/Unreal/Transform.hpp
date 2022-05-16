#ifndef RC_UNREAL_TRANSFORM_HPP
#define RC_UNREAL_TRANSFORM_HPP

#include <Unreal/Quat.hpp>
#include <Unreal/UnrealCoreStructs.hpp>

namespace RC::Unreal
{
    // The 'alignas' used in this struct is because UE4 uses more efficient instructions to deal with this struct
    // They want everything aligned a certain way so we must do the same
    // TODO: Investigate FTransform and FVector in UE5.
    struct alignas(16) RC_UE_API FTransform
    {
        FQuat Rotation{};
        alignas(16) FVector Translation{};
        alignas(16) FVector Scale3D{};

        FORCEINLINE FTransform() = default;

        FORCEINLINE FTransform(FQuat InRotation, FVector InTranslation, FVector InScale3D) :
            Rotation(InRotation), Translation(InTranslation), Scale3D(InScale3D) {
        }
    };
}

#endif // RC_UNREAL_TRANSFORM_HPP