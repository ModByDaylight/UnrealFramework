#ifndef RC_UNREAL_XARRAYPROPERTY_HPP
#define RC_UNREAL_XARRAYPROPERTY_HPP

#include <Unreal/StaticOffsetFinder.hpp>
#include <Unreal/XProperty.hpp>
#include <Unreal/TypeChecker.hpp>
#include <format>

namespace RC::Unreal
{
    struct RC_API UntypedTArray
    {
        void* elements_ptr;
        int32_t num_elements;
        int32_t max_elements;
    };

    class RC_API XArrayProperty : public XProperty
    {
    private:

    public:
        constexpr static inline bool is_trivial = false;
        constexpr static inline int32_t size = sizeof(UntypedTArray);
        using InnerType = UntypedTArray;
        using InnerTypeFull = InnerType;

        // DO NOT USE... MEANT FOR INTERNAL USE ONLY
        static inline class UClass* m_static_obj{};
        static inline FFieldClassVariant m_static_obj_variant{static_cast<FFieldClass*>(nullptr)};

        auto static static_class() -> FFieldClassVariant
        {
            if (!m_static_obj_variant.is_valid()) { throw std::runtime_error{"[XArrayProperty::static_class] m_static_obj_variant is not valid"}; }
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
        auto get_inner() -> XProperty*;

    public:
        auto static get_inner(PropertyDataVC data) -> XProperty*;
        auto static get_inner(XArrayProperty* p_this) -> XProperty*;
        auto static read_data(PropertyDataVC data) -> InnerType;

    public:
        auto static to_string(void* p_this, std::wstring& out_line) -> void;
        auto static to_string_complex(void* p_this, std::wstring& out_line, TypeChecker::ObjectToStringComplexDeclCallable callable) -> void;
    };
}


#endif //RC_UNREAL_XARRAYPROPERTY_HPP
