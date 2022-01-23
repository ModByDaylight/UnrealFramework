#ifndef RC_UNREAL_XSOFTCLASSPROPERTY_HPP
#define RC_UNREAL_XSOFTCLASSPROPERTY_HPP

#include <Unreal/Property/XSoftObjectProperty.hpp>

namespace RC::Unreal
{
    class RC_UE_API XSoftClassProperty : public XSoftObjectProperty
    {
    private:

    public:
        constexpr static inline bool is_trivial = true;
        constexpr static inline int32_t size = sizeof(UObject*);
        using InnerType = UObject*;
        using InnerTypeFull = InnerType*;

        // DO NOT USE... MEANT FOR INTERNAL USE ONLY
        static inline class UClass* m_static_obj{};
        static inline FFieldClassVariant m_static_obj_variant{static_cast<FFieldClass*>(nullptr)};

        auto static static_class() -> FFieldClassVariant
        {
            if (!m_static_obj_variant.is_valid()) { throw std::runtime_error{"[XSoftClassProperty::static_class] m_static_obj_variant is not valid"}; }
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
        auto static construct([[maybe_unused]]PropertyDataVC p_data, [[maybe_unused]]FProperty* p_inner_this, void* v_data) -> InnerType*;
        auto static read_data(PropertyDataVC data) -> InnerType*;

    public:
        auto get_meta_class() -> UClass*;

    public:
    };
}


#endif //RC_UNREAL_XSOFTCLASSPROPERTY_HPP
