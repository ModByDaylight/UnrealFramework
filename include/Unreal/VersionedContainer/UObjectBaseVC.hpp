#ifndef RC_UNREAL_UOBJECTBASEVC_HPP
#define RC_UNREAL_UOBJECTBASEVC_HPP

#include <array>
#include <Unreal/UnrealFlags.hpp>

namespace RC::Unreal
{
    class UObjectBaseVC
    {
    private:
        using UObject = void;

    public:
        virtual ~UObjectBaseVC() = default; // Virtual Destructor
        UObjectBaseVC() = default; // Constructor
        UObjectBaseVC(const UObjectBaseVC&) = delete;  // Copy constructor
        UObjectBaseVC(UObjectBaseVC&&) = delete;  // Move constructor
        UObjectBaseVC& operator=(const UObjectBaseVC&) = delete;  // Copy assignment operator
        UObjectBaseVC& operator=(UObjectBaseVC&&) = delete;  // Move assignment operator

    public:
        virtual auto UObject_get_flags(const UObject* p_this) const -> int32_t = 0;
        virtual auto UObject_set_flags(UObject* p_this, const std::array<EObjectFlags, EObjectFlags_Max>& new_flags) -> void = 0;
        virtual auto UObject_has_any_flag(UObject* p_this, const std::array<EObjectFlags, EObjectFlags_Max>& flags_to_check) -> bool = 0;
        virtual auto UObject_has_all_flags(UObject* p_this, const std::array<EObjectFlags, EObjectFlags_Max>& flags_to_check) -> bool = 0;
    };
}


#endif //RC_UNREAL_UOBJECTBASEVC_HPP
