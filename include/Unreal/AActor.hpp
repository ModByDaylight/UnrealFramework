#ifndef RC_UNREAL_AACTOR_HPP
#define RC_UNREAL_AACTOR_HPP

#include <Unreal/UObject.hpp>

namespace RC::Unreal
{
    class RC_UE_API AActor : public UObject
    {
    private:
        static inline class UClass* m_static_obj{};

    public:
        // DO NOT USE... MEANT FOR INTERNAL USE ONLY
        auto static set_static_obj_ptr(class UClass* ptr)
        {
            m_static_obj = ptr;
        }

        auto static static_class() -> class UClass*
        {
            if (!m_static_obj) { throw std::runtime_error{"[UObjectImpl::get_static_obj_ptr] m_static_obj is nullptr"}; }

            return m_static_obj;
        }

    private:

    public:
        auto get_level() -> UObject*;
        auto get_world() -> UObject*;

    public:
        // UFunction interfaces -> START
        auto K2_TeleportTo(FVector DestLocation, FRotator DestRotation) -> bool;
        auto K2_GetActorLocation() -> FVector;
        auto GetActorForwardVector() -> FVector;
        // UFunction interfaces -> END
    };
}


#endif //RC_UNREAL_AACTOR_HPP
