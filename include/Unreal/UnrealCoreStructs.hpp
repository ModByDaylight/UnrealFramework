#ifndef RC_UNREAL_UNREALCORESTRUCTS_H
#define RC_UNREAL_UNREALCORESTRUCTS_H

#include <Unreal/Common.hpp>

NS_RC_UE_START;

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

struct RC_UE_API FVector
{
    float X;
    float Y;
    float Z;

    inline FVector() : X(0.0f), Y(0.0f), Z(0.0f) {}
    inline FVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

struct RC_UE_API FRotator
{
    float Pitch;
    float Yaw;
    float Roll;

    inline FRotator() : Pitch(0.0f), Yaw(0.0f), Roll(0.0f) {}
    inline FRotator(float pitch, float yaw, float roll) : Pitch(pitch), Yaw(yaw), Roll(roll) {}
};

struct RC_UE_API FQuat
{
    float X;
    float Y;
    float Z;
    float W;

    inline FQuat() : X(0.0f), Y(0.0f), Z(0.0f), W(0.0f) {}
    inline FQuat(float X, float Y, float Z, float W) : X(X), Y(Y), Z(Z), W(W) {}
};

// TODO: Investigate FTransform and FVector in UE5.
struct alignas(16) RC_UE_API FTransform
{
    FQuat Rotation;
    FVector Translation;
    FVector Scale3D;

    inline FTransform() : Rotation(), Translation(), Scale3D() {}
    inline FTransform(FQuat Rotation, FVector Translation, FVector Scale3D) : Rotation(Rotation), Translation(Translation), Scale3D(Scale3D) {}
};

NS_RC_UE_END;

#endif //RC_UNREAL_UNREALCORESTRUCTS_H
