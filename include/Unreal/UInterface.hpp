#ifndef RC_UNREAL_UINTERFACE_HPP
#define RC_UNREAL_UINTERFACE_HPP

#include <Unreal/UObject.hpp>

namespace RC::Unreal
{
    class RC_UE_API UInterface : public UObject
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
            if (!m_static_obj) { throw std::runtime_error{"[UInterface::get_static_obj_ptr] m_static_obj is nullptr"}; }

            return m_static_obj;
        }
    };
}

#endif //RC_UNREAL_UINTERFACE_HPP
