#ifndef RC_UNREAL_UOBJECT415_HPP
#define RC_UNREAL_UOBJECT415_HPP

#include <Unreal/VersionedContainer/UObjectBaseVC.hpp>
#include <unordered_map>
#include <Unreal/UnrealFlags.hpp>

namespace RC::Unreal
{
    class UObject415 : public UObjectBaseVC
    {
    private:
        // Overrides -> START
        auto UObject_get_flags(const void* p_this) const -> int32_t override;
        auto UObject_set_flags(void* p_this, const std::array<EObjectFlags, EObjectFlags_Max>& new_flags) -> void override;
        auto UObject_has_any_flag(void* p_this, const std::array<EObjectFlags, EObjectFlags_Max>& flags_to_check) -> bool override;
        auto UObject_has_all_flags(void* p_this, const std::array<EObjectFlags, EObjectFlags_Max>& flags_to_check) -> bool override;
        // Overrides -> END
    };
}


#endif //RC_UNREAL_UOBJECT415_HPP
