#ifndef RC_UNREAL_XDOUBLEPROPERTY_HPP
#define RC_UNREAL_XDOUBLEPROPERTY_HPP

#include <Unreal/Property/XNumericProperty.hpp>
#include <Helpers/Format.hpp>

namespace RC::Unreal
{
    class RC_UE_API XDoubleProperty : public XNumericProperty
    {
    private:

    public:
        constexpr static inline bool is_trivial = true;
        constexpr static inline int32_t size = sizeof(double);
        using InnerType = double;
        using InnerTypeFull = InnerType*;

        // DO NOT USE... MEANT FOR INTERNAL USE ONLY
        static inline class UClass* m_static_obj{};
        static inline FFieldClassVariant m_static_obj_variant{static_cast<FFieldClass*>(nullptr)};

        auto static static_class() -> FFieldClassVariant
        {
            if (!m_static_obj_variant.is_valid()) { throw std::runtime_error{"[XDoubleProperty::static_class] m_static_obj_variant is not valid"}; }
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
        auto static read_data(PropertyDataVC data) -> InnerType*
        {
            return static_cast<InnerType*>(data.data_ptr);
        }

    public:
        static auto to_string(void* p_this, std::wstring& out_line) -> void
        {
            trivial_dump_to_string(p_this, out_line);
        }
    };
}


#endif //RC_UNREAL_XDOUBLEPROPERTY_HPP
