#ifndef RC_UNREAL_UACTORCOMPONENT_HPP
#define RC_UNREAL_UACTORCOMPONENT_HPP

#include <Unreal/Common.hpp>
#include <Unreal/UObject.hpp>

namespace RC::Unreal
{
    class RC_UE_API UActorComponent : public UObject
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
            if (!m_static_obj) { throw std::runtime_error{"[UActorComponent::get_static_obj_ptr] m_static_obj is nullptr"}; }

            return m_static_obj;
        }
    };

    class RC_UE_API USceneComponent : public UActorComponent
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
            if (!m_static_obj) { throw std::runtime_error{"[USceneComponent::get_static_obj_ptr] m_static_obj is nullptr"}; }

            return m_static_obj;
        }
    };
}

#endif //RC_UNREAL_UACTORCOMPONENT_HPP
