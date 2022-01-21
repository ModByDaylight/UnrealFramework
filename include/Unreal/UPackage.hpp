#ifndef RC_UNREAL_UPACKAGE_HPP
#define RC_UNREAL_UPACKAGE_HPP

#include <Unreal/UObject.hpp>

namespace RC::Unreal
{
    class RC_API UPackage : public UObject
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
            if (!m_static_obj) {
                throw std::runtime_error{"[UPackage::get_static_obj_ptr] m_static_obj is nullptr"};
            }
            return m_static_obj;
        }
        // Placeholder for now, but is there even anything inside that would be useful ?
        // It's needed regardless because it's used in a few places but not in any way that's different to a UObject
    };
}


#endif //RC_UNREAL_UPACKAGE_HPP
