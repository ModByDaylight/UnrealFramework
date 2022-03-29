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
        FQuat Rotation;
        alignas(16) FVector Translation;
        alignas(16) FVector Scale3D;

        inline FTransform() : Rotation(0.0f, 0.0f, 0.0f, 0.0f), Translation(0.0f, 0.0f, 0.0f), Scale3D(0.0f, 1.0f, 1.0f) {}
        inline FTransform(FQuat Rotation, FVector Translation, FVector Scale3D) : Rotation(Rotation), Translation(Translation), Scale3D(Scale3D) {}

        FTransform& operator=(const FTransform& Other)
        {
            this->Rotation = Other.Rotation;
            this->Translation = Other.Translation;
            this->Scale3D = Other.Scale3D;

            return *this;
        }
    };
}

#endif // RC_UNREAL_TRANSFORM_HPP