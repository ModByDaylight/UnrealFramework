#ifndef RC_UNREAL_UNREALCORESTRUCTS_H
#define RC_UNREAL_UNREALCORESTRUCTS_H

#include <Unreal/Common.hpp>
#include <Unreal/UnrealVersion.hpp>
#include <Unreal/FWeakObjectPtr.hpp>
#include <Unreal/NameTypes.hpp>

NS_RC_UE_START;

//TODO: Temporary location, we need a reasonable implementation involving virtual offsets
class FArchive;

struct RC_UE_API FDateTime {
    /** Holds the ticks in 100 nanoseconds resolution since January 1, 0001 A.D. */
    int64 Ticks{0};
};

enum class ESpawnActorCollisionHandlingMethod : uint8
{
    /** Fall back to default settings. */
    Undefined,
    /** Actor will spawn in desired location, regardless of collisions. */
    AlwaysSpawn,
    /** Actor will try to find a nearby non-colliding location (based on shape components), but will always spawn even if one cannot be found. */
    AdjustIfPossibleButAlwaysSpawn,
    /** Actor will try to find a nearby non-colliding location (based on shape components), but will NOT spawn unless one is found. */
    AdjustIfPossibleButDontSpawnIfColliding,
    /** Actor will fail to spawn. */
    DontSpawnIfColliding,
};

enum class EDetachmentRule : uint8 {
    /** Keeps current relative transform. */
    KeepRelative,
    /** Automatically calculates the relative transform such that the detached component maintains the same world transform. */
    KeepWorld,
};

enum class EAttachmentRule : uint8 {
    /** Keeps current relative transform as the relative transform to the new parent. */
    KeepRelative,
    /** Automatically calculates the relative transform such that the attached component maintains the same world transform. */
    KeepWorld,
    /** Snaps transform to the attach point */
    SnapToTarget,
};

/**
 * Implements a globally unique identifier.
 */
struct RC_UE_API FGuid
{
public:
    /** Default constructor. */
    FGuid() : A(0), B(0), C(0), D(0) {}

    /**
     * Creates and initializes a new GUID from the specified components.
     */
    FGuid(uint32_t InA, uint32_t InB, uint32_t InC, uint32_t InD) : A(InA), B(InB), C(InC), D(InD) {}
public:
    /**
     * Compares two GUIDs for equality.
     * @return true if the GUIDs are equal, false otherwise.
     */
    friend bool operator==(const FGuid& X, const FGuid& Y)
    {
        return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) == 0;
    }

    /**
     * Compares two GUIDs for inequality.
     * @return true if the GUIDs are not equal, false otherwise.
     */
    friend bool operator!=(const FGuid& X, const FGuid& Y)
    {
        return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) != 0;
    }

    /**
     * Compares two GUIDs.
     * @return true if the first GUID is less than the second one.
     */
    friend bool operator<(const FGuid& X, const FGuid& Y)
    {
        return	((X.A < Y.A) ? true : ((X.A > Y.A) ? false :
                ((X.B < Y.B) ? true : ((X.B > Y.B) ? false :
                ((X.C < Y.C) ? true : ((X.C > Y.C) ? false :
                ((X.D < Y.D) ? true : ((X.D > Y.D) ? false : false))))))));
    }

    /**
     * Provides access to the GUIDs components.
     * @param Index The index of the component to return (0...3).
     * @return The component.
     */
    uint32_t& operator[](int32_t index)
    {
        switch (index)
        {
            case 0: return A;
            case 1: return B;
            case 2: return C;
            case 3: return D;
        }
        return A;
    }

    /**
     * Provides read-only access to the GUIDs components.
     *
     * @param Index The index of the component to return (0...3).
     * @return The component.
     */
    const uint32_t& operator[](int32_t index) const
    {
        switch (index)
        {
            case 0: return A;
            case 1: return B;
            case 2: return C;
            case 3: return D;
        }
        return A;
    }

public:
    /**
     * Invalidates the GUID.
     */
    void invalidate()
    {
        A = B = C = D = 0;
    }

    /**
     * Checks whether this GUID is valid or not.
     * A GUID that has all its components set to zero is considered invalid.
     */
    bool is_valid() const
    {
        return ((A | B | C | D) != 0);
    }

public:
    /** Holds the first component. */
    uint32_t A;
    /** Holds the second component. */
    uint32_t B;
    /** Holds the third component. */
    uint32_t C;
    /** Holds the fourth component. */
    uint32_t D;
};



namespace Internal {
    struct FVector_UE4 {
        float X{0.0f};
        float Y{0.0f};
        float Z{0.0f};
    };
    struct FVector_UE5 {
        double X{0.0f};
        double Y{0.0f};
        double Z{0.0f};
    };
}

class RC_UE_API FVector
{
private:
    std::conditional_t<UNREAL_VERSION >= 5000, Internal::FVector_UE5, Internal::FVector_UE4> Data;
public:
    FORCEINLINE FVector() = default;

    FORCEINLINE FVector(double X, double Y, double Z) {
        SetX(X);
        SetY(Y);
        SetZ(Z);
    }

    FORCEINLINE void SetX(double NewX) { Data.X = NewX; };
    FORCEINLINE void SetY(double NewY) { Data.Y = NewY; };
    FORCEINLINE void SetZ(double NewZ) { Data.Z = NewZ; };

    FORCEINLINE double X() const { return Data.X; };
    FORCEINLINE double Y() const { return Data.Y; };
    FORCEINLINE double Z() const { return Data.Z; };
};

struct RC_UE_API FHitResult {
    int32 FaceIndex{};
    float Time{};
    float Distance{};

    FVector Location;
    FVector ImpactPoint;
    FVector Normal;
    FVector ImpactNormal;
    FVector TraceStart;
    FVector TraceEnd;

    float PenetrationDepth{};
    int32 Item{};
    uint8 ElementIndex{};

    uint8 bBlockingHit: 1;
    uint8 bStartPenetrating: 1;

    TWeakObjectPtr<class UPhysicalMaterial> PhysMaterial;
    TWeakObjectPtr<class AActor> Actor;
    TWeakObjectPtr<class UPrimitiveComponent> Component;

    FName BoneName;
    FName MyBoneName;
};

NS_RC_UE_END;

#endif //RC_UNREAL_UNREALCORESTRUCTS_H
