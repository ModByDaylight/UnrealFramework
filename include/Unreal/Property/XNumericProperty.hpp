#ifndef RC_UNREAL_XNUMERICPROPERTY_HPP
#define RC_UNREAL_XNUMERICPROPERTY_HPP

#include <Unreal/XProperty.hpp>

namespace RC::Unreal
{
    class RC_API XNumericProperty : public XProperty
    {
    public:
        // DO NOT USE... MEANT FOR INTERNAL USE ONLY
        static inline class UClass* m_static_obj{};
        static inline FFieldClassVariant m_static_obj_variant{static_cast<FFieldClass*>(nullptr)};

        auto static static_class() -> FFieldClassVariant
        {
            if (!m_static_obj_variant.is_valid()) { throw std::runtime_error{"[XNumericProperty::static_class] m_static_obj_variant is not valid"}; }
            if (m_static_obj_variant.is_uobject())
            {
                return m_static_obj_variant.to_uobject();
            }
            else
            {
                return m_static_obj_variant.to_field();
            }
        }

    public:
        auto is_floating_point() -> bool;
        // The 'this' ptr is the underlying numeric property
        // The 'object' ptr is the object where the value can be found
        // The 'property' ptr is the property container
        auto read_as_int64(UObject* object, XProperty* property) -> int64_t;
        auto read_as_int64(UObject* object) -> int64_t;
        auto read_as_double(UObject* object) -> double;
    };
}


#endif //RC_UNREAL_XNUMERICPROPERTY_HPP
