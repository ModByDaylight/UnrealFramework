#ifndef RC_UNREAL_FWEAKOBJECTPTR_HPP
#define RC_UNREAL_FWEAKOBJECTPTR_HPP

#include <cstdint>

#include <Unreal/Common.hpp>

namespace RC::Unreal
{
    struct RC_API FWeakObjectPtr
    {
    private:
        constexpr static int32_t INDEX_NONE = -1;

        int32_t object_index;
        int32_t object_serial_number;

        auto internal_get_object_item() const -> struct FUObjectItem*;
        auto internal_get(bool b_even_if_pending_kill) const -> class UObject*;
        auto reset() -> void;

    public:
        auto operator=(class UObject*) -> void;

    public:
        auto serial_numbers_match(struct FUObjectItem*) const -> bool;
        auto get() const -> class UObject*;
    };
}


#endif //RC_UNREAL_FWEAKOBJECTPTR_HPP
