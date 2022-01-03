#include <Unreal/VersionedContainer/UObject412.hpp>
#include <Unreal/VersionedContainer/Flags412.hpp>
#include <Unreal/VersionedContainer/Flags.hpp>

namespace RC::Unreal
{
    auto UObject412::UObject_get_flags(const void* p_this) const -> int32_t
    {
        return static_cast<int32_t>(Flags::get_flags<Flags412::EObjectFlags_Impl>(p_this));
    }

    auto UObject412::UObject_set_flags(void* p_this, const std::array<EObjectFlags, EObjectFlags_Max>& new_flags) -> void
    {
        Flags412::EObjectFlags_Impl new_impl_flags = Flags412::to_impl_flags(new_flags);
        Flags::set_flags(p_this, new_impl_flags);
    }

    auto UObject412::UObject_has_any_flag(void* p_this, const std::array<EObjectFlags, EObjectFlags_Max>& flags_to_check) -> bool
    {
        Flags412::EObjectFlags_Impl impl_flags_to_check = Flags412::to_impl_flags(flags_to_check);
        return Flags::has_any_flag(p_this, impl_flags_to_check);
    }

    auto UObject412::UObject_has_all_flags(void* p_this, const std::array<EObjectFlags, EObjectFlags_Max>& flags_to_check) -> bool
    {
        Flags412::EObjectFlags_Impl impl_flags_to_check = Flags412::to_impl_flags(flags_to_check);
        return Flags::has_all_flags(p_this, impl_flags_to_check);
    }
}
