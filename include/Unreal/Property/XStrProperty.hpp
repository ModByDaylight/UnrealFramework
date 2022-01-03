#ifndef RC_UNREAL_XSTRPROPERTY_HPP
#define RC_UNREAL_XSTRPROPERTY_HPP

#include <Helpers/Format.hpp>

#include <Unreal/XProperty.hpp>
#include <Unreal/FString.hpp>

namespace RC::Unreal
{
    class RC_API XStrProperty : public XProperty
    {
    private:

    public:
        constexpr static inline bool is_trivial = true;
        constexpr static inline int32_t size = sizeof(FString);
        using InnerType = FString;
        using InnerTypeFull = InnerType*;

        // DO NOT USE... MEANT FOR INTERNAL USE ONLY
        static inline class UClass* m_static_obj{};
        static inline FFieldClassVariant m_static_obj_variant{static_cast<FFieldClass*>(nullptr)};

        auto static static_class() -> FFieldClassVariant
        {
            if (!m_static_obj_variant.is_valid()) { throw std::runtime_error{"[XStrProperty::static_class] m_static_obj_variant is not valid"}; }
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
        auto static construct([[maybe_unused]]PropertyDataVC p_data, [[maybe_unused]]XProperty* p_inner_this, void* v_data) -> InnerType*;
        auto static read_data(PropertyDataVC data) -> InnerType*;

    public:

    public:
        static auto to_string(void* p_this, std::wstring& out_line) -> void;
    };
}


#endif //RC_UNREAL_XSTRPROPERTY_HPP
