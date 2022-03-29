#include <Unreal/Quat.hpp>
#include <Unreal/Rotator.hpp>

namespace RC::Unreal
{
    FQuat::FQuat(const FRotator& R)
    {
        FQuat Quat = R.Quaternion();
        X = Quat.X;
        Y = Quat.Y;
        Z = Quat.Z;
        W = Quat.W;
    }
}
