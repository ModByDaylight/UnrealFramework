#ifndef UE4SS_UNREALCORESTRUCTS_H
#define UE4SS_UNREALCORESTRUCTS_H

#include <Unreal/Common.hpp>

struct RC_UE_API FVector
{
    float X;
    float Y;
    float Z;

    inline FVector() : X(0.0f), Y(0.0f), Z(0.0f) {
    }
    inline FVector(float x, float y, float z) : X(x), Y(y), Z(z) {
    }
};

struct RC_UE_API FRotator
{
    float Pitch;
    float Yaw;
    float Roll;

    inline FRotator() : Pitch(0.0f), Yaw(0.0f), Roll(0.0f) {
    }
    inline FRotator(float pitch, float yaw, float roll) : Pitch(pitch), Yaw(yaw), Roll(roll) {
    }
};

#endif //UE4SS_UNREALCORESTRUCTS_H
