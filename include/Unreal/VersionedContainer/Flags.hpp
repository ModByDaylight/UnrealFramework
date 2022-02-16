#ifndef RC_UNREAL_FLAGS_HPP
#define RC_UNREAL_FLAGS_HPP

#include <Helpers/Casting.hpp>

namespace RC::Unreal::Flags
{
    template<typename EnumFlagType>
    concept ValidEnumFlagType = std::is_enum_v<EnumFlagType>;

    template<ValidEnumFlagType FlagsType>
    auto get_flags(const void* p_this) -> FlagsType
    {
        // TODO: Hard-coding this for now. Consider not hard-coding this.
        return Helper::Casting::offset_deref<FlagsType>(p_this, 0x8);
    }

    template<ValidEnumFlagType FlagsType>
    auto set_flags_to(FlagsType* current_flags, FlagsType new_flags) -> void
    {
        *current_flags = new_flags;
    }

    template<ValidEnumFlagType FlagsType>
    auto set_flags_to(void* p_this, FlagsType new_flags) -> void
    {
        *static_cast<FlagsType*>(static_cast<void*>(static_cast<unsigned char*>(p_this) + 0x8)) = new_flags;
    }

    template<ValidEnumFlagType FlagsType>
    auto set_flags(void* p_this, FlagsType new_flags) -> void
    {
        using UnderlyingEnumType = std::underlying_type_t<FlagsType>;
        set_flags_to(p_this, static_cast<FlagsType>(static_cast<UnderlyingEnumType>(get_flags<FlagsType>(p_this)) | static_cast<UnderlyingEnumType>(new_flags)));
    }

    template<ValidEnumFlagType FlagsType>
    auto clear_flags(void* p_this, FlagsType flags_to_clear) -> void
    {
        set_flags_to(get_flags<FlagsType>(p_this) & (~flags_to_clear));
    }

    template<ValidEnumFlagType FlagsType>
    auto has_any_flag(void* p_this, FlagsType flags_to_check) -> bool
    {
        using UnderlyingEnumType = std::underlying_type_t<FlagsType>;
        return (static_cast<UnderlyingEnumType>(get_flags<FlagsType>(p_this)) & static_cast<UnderlyingEnumType>(flags_to_check)) != 0;
    }

    template<ValidEnumFlagType FlagsType>
    auto has_all_flags(void* p_this, FlagsType flags_to_check) -> bool
    {
        using UnderlyingEnumType = std::underlying_type_t<FlagsType>;
        auto flags_to_check_sub_typed = static_cast<UnderlyingEnumType>(flags_to_check);
        return ((static_cast<UnderlyingEnumType>(get_flags<FlagsType>(p_this)) & flags_to_check_sub_typed) == flags_to_check_sub_typed);
    }
}


#endif //RC_UNREAL_FLAGS_HPP
