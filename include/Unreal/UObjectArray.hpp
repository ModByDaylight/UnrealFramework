#ifndef RC_UNREAL_UOBJECTARRAY_HPP
#define RC_UNREAL_UOBJECTARRAY_HPP

// User API for UObjectArray related abstractions
// See Base.hpp / DerivedX.hpp files for implementations

#include <Unreal/Common.hpp>

namespace RC::Unreal
{
    struct RC_UE_API FUObjectItem
    {
        auto IsUnreachable() -> bool;
        auto SetRootSet() -> void;
        auto UnsetRootSet() -> void;
        auto SetGCKeep() -> void;
        auto UnsetGCKeep() -> void;
        auto GetUObject() -> class UObject*;
    };

    class RC_UE_API UObjectArray
    {
    public:
        auto static SetupGUObjectArrayAddress(void* address) -> void;

    public:
        auto static IsValid(FUObjectItem* ObjectItem, bool bEvenIfPendingKill) -> bool;
    };
}


#endif //RC_UNREAL_UOBJECTARRAY_HPP
