#ifndef RC_UNREAL_XNAMEPROPERTY_HPP
#define RC_UNREAL_XNAMEPROPERTY_HPP

#include <Helpers/Format.hpp>

#include <Unreal/FProperty.hpp>

namespace RC::Unreal
{
    class RC_UE_API XNameProperty : public FProperty
    {
    private:

    public:
        constexpr static inline bool is_trivial = true;
        constexpr static inline int32_t size = sizeof(FName);
        using InnerType = FName;
        using InnerTypeFull = InnerType&;

        // DO NOT USE... MEANT FOR INTERNAL USE ONLY
        static inline class UClass* m_static_obj{};
        static inline FFieldClassVariant m_static_obj_variant{static_cast<FFieldClass*>(nullptr)};

        auto static static_class() -> FFieldClassVariant
        {
            if (!m_static_obj_variant.is_valid()) { throw std::runtime_error{"[XNameProperty::static_class] m_static_obj_variant is not valid"}; }
            if (m_static_obj_variant.IsUClass())
            {
                return m_static_obj_variant.ToUClass();
            }
            else
            {
                return m_static_obj_variant.ToFieldClass();
            }
        }

    public:
        auto static construct([[maybe_unused]]PropertyDataVC p_data, [[maybe_unused]]FProperty* p_inner_this, void* v_data) -> InnerType*
        {
            return static_cast<InnerType*>(v_data);
        }

        auto static read_data(PropertyDataVC data) -> InnerType&
        {
            return *static_cast<InnerType*>(data.data_ptr);
        }

    public:
        static auto to_string(void* p_this, std::wstring& out_line) -> void
        {
            trivial_dump_to_string(p_this, out_line);
        }
    };
}


#endif //RC_UNREAL_XNAMEPROPERTY_HPP
