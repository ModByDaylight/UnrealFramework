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
        virtual auto UObject_get_flags(const UObject* p_this) const -> EObjectFlags = 0;
        virtual auto UObject_set_flags(UObject* p_this, EObjectFlags new_flags) -> void = 0;

        virtual auto UObject_get_internal_index(const UObject* p_this) const -> int32_t
        {
            // TODO: Automate this offset
            return Helper::Casting::offset_deref<uint32_t>(this, 0xC);
        }
    };
}


#endif //RC_UNREAL_UOBJECTBASEVC_HPP
