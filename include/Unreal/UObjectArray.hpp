#ifndef RC_UNREAL_UOBJECTARRAY_HPP
#define RC_UNREAL_UOBJECTARRAY_HPP

// User API for UObjectArray related abstractions
// See Base.hpp / DerivedX.hpp files for implementations

#include <Unreal/Common.hpp>

namespace RC::Unreal
{
    struct RC_API FUObjectItem
    {
        auto is_unreachable() -> bool;
        auto set_root_set() -> void;
        auto unset_root_set() -> void;
        auto set_gc_keep() -> void;
        auto unset_gc_keep() -> void;
        auto get_uobject() -> class UObject*;
    };

    class RC_API UObjectArray
    {
    public:
        auto static setup_guobjectarray_address(void* address) -> void;

    public:
        auto static is_valid(FUObjectItem* object_item, bool b_even_if_pending_kill) -> bool;
    };
}


#endif //RC_UNREAL_UOBJECTARRAY_HPP
