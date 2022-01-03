#include <Unreal/VersionedContainer/UObject415.hpp>
#include <Unreal/VersionedContainer/Flags415.hpp>
#include <Unreal/VersionedContainer/Flags.hpp>

namespace RC::Unreal
{
    auto UObject415::UObject_get_flags(const void* p_this) const -> int32_t
    {
        return static_cast<int32_t>(Flags::get_flags<Flags415::EObjectFlags_Impl>(p_this));
    }

    auto UObject415::UObject_set_flags(void* p_this, const std::array<EObjectFlags, EObjectFlags_Max>& new_flags) -> void
    {
        Flags415::EObjectFlags_Impl new_impl_flags = Flags415::to_impl_flags(new_flags);
        Flags::set_flags(p_this, new_impl_flags);
    }

    auto UObject415::UObject_has_any_flag(void* p_this, const std::array<EObjectFlags, EObjectFlags_Max>& flags_to_check) -> bool
    {
        Flags415::EObjectFlags_Impl impl_flags_to_check = Flags415::to_impl_flags(flags_to_check);
        return Flags::has_any_flag(p_this, impl_flags_to_check);
    }

    auto UObject415::UObject_has_all_flags(void* p_this, const std::array<EObjectFlags, EObjectFlags_Max>& flags_to_check) -> bool
    {
        Flags415::EObjectFlags_Impl impl_flags_to_check = Flags415::to_impl_flags(flags_to_check);
        return Flags::has_all_flags(p_this, impl_flags_to_check);
    }
}
