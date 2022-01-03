#ifndef RC_UNREAL_XSTRUCTROPERTYIMPL_HPP
#define RC_UNREAL_XSTRUCTROPERTYIMPL_HPP

#include <string>
#include <stdexcept>

#include <Helpers/Format.hpp>

#include <Unreal/VersionedContainer/Base.hpp>
#include <Unreal/XProperty.hpp>

namespace RC::Unreal
{
    class UScriptStruct;
    class XStructData;

    class RC_API XStructProperty : public XProperty
    {
    private:

    public:
        constexpr static inline bool is_trivial = false;
        constexpr static inline int32_t size = -1; // Needs to be determined at dynamically at runtime
        using InnerType = XStructData;
        using InnerTypeFull = InnerType;

        // DO NOT USE... MEANT FOR INTERNAL USE ONLY
        static inline class UClass* m_static_obj{};
        static inline FFieldClassVariant m_static_obj_variant{static_cast<FFieldClass*>(nullptr)};

        auto static static_class() -> FFieldClassVariant
        {
            if (!m_static_obj_variant.is_valid()) { throw std::runtime_error{"[XStructProperty::static_class] m_static_obj_variant is not valid"}; }
            if (m_static_obj_variant.is_uobject())
            {
                return m_static_obj_variant.to_uobject();
            }
            else
            {
                return m_static_obj_variant.to_field();
            }
        }

        auto get_script_struct() -> UScriptStruct*;

    public:
        auto static construct(PropertyDataVC p_data, XProperty* p_inner_this, void* v_data) -> InnerType*;
        auto static read_data(PropertyDataVC data) -> InnerType;

    public:
        static auto to_string(void* p_this, std::wstring& out_line) -> void;
    };
}


#endif //RC_UNREAL_XSTRUCTROPERTYIMPL_HPP
