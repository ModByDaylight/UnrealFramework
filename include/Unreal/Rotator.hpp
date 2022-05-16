#ifndef RC_UNREAL_ROTATOR_HPP
#define RC_UNREAL_ROTATOR_HPP

#include <Unreal/UObject.hpp>

#pragma warning(disable: 4305)

namespace RC::Unreal
{
    struct FQuat;

    struct RC_UE_API FRotator
    {
        float Pitch;
        float Yaw;
        float Roll;

        inline FRotator() : Pitch(0.0f), Yaw(0.0f), Roll(0.0f) {}
        inline FRotator(float pitch, float yaw, float roll) : Pitch(pitch), Yaw(yaw), Roll(roll) {}

        inline FQuat Quaternion() const;

        FORCEINLINE static float ClampAxis(float Angle) {
            Angle = fmod(Angle, 360.0f);
            if (Angle < 0.0f) {
                Angle += 360.0f;
            }
            return Angle;
        }

        FORCEINLINE static float NormalizeAxis(float Angle) {
            Angle = ClampAxis(Angle);
            if (Angle > 180.0f) {
                Angle -= 360.0f;
            }
            return Angle;
        }
    };
}

#pragma warning(default: 4305)

#endif // RC_UNREAL_ROTATOR_HPP