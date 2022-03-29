#ifndef RC_UNREAL_QUAT_HPP
#define RC_UNREAL_QUAT_HPP

#include <Unreal/UObject.hpp>

namespace RC::Unreal
{
    struct RC_UE_API FQuat
    {
        float X;
        float Y;
        float Z;
        float W;

        inline FQuat() : X(0.0f), Y(0.0f), Z(0.0f), W(0.0f) {}
        inline FQuat(float X, float Y, float Z, float W) : X(X), Y(Y), Z(Z), W(W) {}
        FQuat(const struct FRotator& R);
    };
}

#endif // RC_UNREAL_QUAT_HPP