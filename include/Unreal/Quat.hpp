#ifndef RC_UNREAL_QUAT_HPP
#define RC_UNREAL_QUAT_HPP

#include <Unreal/UObject.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <Unreal/Rotator.hpp>

namespace RC::Unreal {
    namespace Internal {
        struct FQuat_UE4 {
            float X{0.0f};
            float Y{0.0f};
            float Z{0.0f};
            float W{0.0f};
        };

        struct FQuat_UE5 {
            double X{0.0};
            double Y{0.0};
            double Z{0.0};
            double W{0.0};
        };
    }

    /**
     * Engine version independent definition of FQuat
     */
    class RC_UE_API FQuat {
    private:
        std::conditional_t<UNREAL_VERSION >= 5000, Internal::FQuat_UE5, Internal::FQuat_UE4> Data;
    public:
        FORCEINLINE FQuat() = default;

        FORCEINLINE FQuat(double X, double Y, double Z, double W) {
            SetX(X);
            SetY(Y);
            SetZ(Z);
            SetW(W);
        }

        FORCEINLINE double X() const { return Data.X; }
        FORCEINLINE double Y() const { return Data.Y; };
        FORCEINLINE double Z() const { return Data.Z; };
        FORCEINLINE double W() const { return Data.W; };

        FORCEINLINE void SetX(double NewX) { Data.X = NewX; };
        FORCEINLINE void SetY(double NewY) { Data.Y = NewY; };
        FORCEINLINE void SetZ(double NewZ) { Data.Z = NewZ; };
        FORCEINLINE void SetW(double NewW) { Data.W = NewW; };

        FRotator Rotator() const {
            const double SingularityTest = Z() * X() - W() * Y();
            const double YawY = 2.0f * (W() * Z() + X() * Y());
            const double YawX = (1.0f - 2.0f * ((Y() * Y()) + (Z() * Z())));

            const double SINGULARITY_THRESHOLD = 0.4999995f;
            const double RAD_TO_DEG = (180.0f) / M_PI;
            FRotator RotatorFromQuat;

            if (SingularityTest < -SINGULARITY_THRESHOLD) {
                RotatorFromQuat.Pitch = -90.0f;
                RotatorFromQuat.Yaw = atan2(YawY, YawX) * RAD_TO_DEG;
                RotatorFromQuat.Roll = FRotator::NormalizeAxis(-RotatorFromQuat.Yaw - (2.0f * atan2(X(), W()) * RAD_TO_DEG));
            } else if (SingularityTest > SINGULARITY_THRESHOLD) {
                RotatorFromQuat.Pitch = 90.0f;
                RotatorFromQuat.Yaw = atan2(YawY, YawX) * RAD_TO_DEG;
                RotatorFromQuat.Roll = FRotator::NormalizeAxis(RotatorFromQuat.Yaw - (2.0f * atan2(X(), W()) * RAD_TO_DEG));
            } else {
                RotatorFromQuat.Pitch = asin(2.0f * (SingularityTest)) * RAD_TO_DEG;
                RotatorFromQuat.Yaw = atan2(YawY, YawX) * RAD_TO_DEG;
                RotatorFromQuat.Roll = atan2(-2.0f * (W() * X() + Y() * Z()), (1.0f - 2.0f * ((X() * X()) + (Y() * Y())))) * RAD_TO_DEG;
            }
            return RotatorFromQuat;
        }
    };

    inline FQuat FRotator::Quaternion() const {
        const float DEG_TO_RAD = M_PI/(180.f);
        const float RADS_DIVIDED_BY_2 = DEG_TO_RAD/2.f;
        float SP, SY, SR;
        float CP, CY, CR;

        const float PitchNoWinding = std::fmod(Pitch, 360.0f);
        const float YawNoWinding = std::fmod(Yaw, 360.0f);
        const float RollNoWinding = std::fmod(Roll, 360.0f);

        SP = std::sin(PitchNoWinding * RADS_DIVIDED_BY_2);
        CP = std::cos(PitchNoWinding * RADS_DIVIDED_BY_2);
        SY = std::sin(YawNoWinding * RADS_DIVIDED_BY_2);
        CY = std::cos(YawNoWinding * RADS_DIVIDED_BY_2);
        SR = std::sin(RollNoWinding * RADS_DIVIDED_BY_2);
        CR = std::cos(RollNoWinding * RADS_DIVIDED_BY_2);

        FQuat RotationQuat;
        RotationQuat.SetX(CR*SP*SY - SR*CP*CY);
        RotationQuat.SetY(CR*SP*CY - SR*CP*SY);
        RotationQuat.SetZ(CR*CP*SY - SR*SP*CY);
        RotationQuat.SetW(CR*CP*CY + SR*SP*SY);

        return RotationQuat;
    }
}

#endif // RC_UNREAL_QUAT_HPP