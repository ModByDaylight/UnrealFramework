#ifndef RC_UNREAL_XMAPPROPERTY_HPP
#define RC_UNREAL_XMAPPROPERTY_HPP

#include <Helpers/Format.hpp>

#include <Unreal/XProperty.hpp>

namespace RC::Unreal
{
    struct RC_API MapData
    {
        void* elements_ptr;
        int32_t num_elements;
        int32_t max_elements;
    };

    class RC_API XMapProperty : public XProperty
    {
    private:

    public:
        constexpr static inline bool is_trivial = true;
        constexpr static inline int32_t size = sizeof(MapData);
        using InnerType = MapData;
        using InnerTypeFull = InnerType;

        // DO NOT USE... MEANT FOR INTERNAL USE ONLY
        static inline class UClass* m_static_obj{};
        static inline FFieldClassVariant m_static_obj_variant{static_cast<FFieldClass*>(nullptr)};

        auto static static_class() -> FFieldClassVariant
        {
            if (!m_static_obj_variant.is_valid()) { throw std::runtime_error{"[XMapProperty::static_class] m_static_obj_variant is not valid"}; }
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
        auto static read_data(PropertyDataVC data) -> InnerType;

    public:
        auto get_key_property() -> XProperty*;
        auto get_value_property() -> XProperty*;

    public:
        static auto to_string(void* p_this, std::wstring& out_line) -> void;
        static auto to_string_complex(void* p_this, std::wstring& out_line, const std::function<void(void*)>& callable) -> void;
    };
}


#endif //RC_UNREAL_XMAPPROPERTY_HPP
