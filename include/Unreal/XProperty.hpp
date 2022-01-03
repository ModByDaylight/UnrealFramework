#ifndef RC_UNREAL_XPROPERTY_HPP
#define RC_UNREAL_XPROPERTY_HPP

#include <Unreal/FField.hpp>

namespace RC::Unreal
{
    class RC_API XPropertyType
    {
    private:
        class XProperty* m_property{};

    public:
        explicit XPropertyType(class XProperty* property) : m_property(property) {};

        [[nodiscard]] auto as_string() -> std::wstring;
        [[nodiscard]] auto as_fname() -> FName;
    };

    class RC_API XProperty : public FField
    {
    private:

    public:
        // DO NOT USE... MEANT FOR INTERNAL USE ONLY
        static inline class UClass* m_static_obj{};
        static inline FFieldClassVariant m_static_obj_variant{static_cast<FFieldClass*>(nullptr)};

        auto static static_class() -> FFieldClassVariant
        {
            if (!m_static_obj_variant.is_valid()) { throw std::runtime_error{"[XProperty::static_class] m_static_obj_variant is not valid"}; }
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
        auto get_type() -> XPropertyType;
        auto get_offset_for_internal() -> int32_t;
        auto get_rep_notify_func() -> FName;
        auto get_size() -> int32_t;
        auto get_array_dim() -> int32_t;
        auto get_element_size() -> int32_t;
        auto get_property_flags() -> EPropertyFlags;
        auto has_any_property_flags(uint64_t flags_to_check) -> bool;
        auto has_all_property_flags(uint64_t flags_to_check) -> bool;
        // These functions intentionally hide the ones in UObjectBaseUtility
        auto get_full_name(const void* stop_outer = nullptr) -> std::wstring;
        auto get_full_name(std::wstring& out_name, const void* stop_outer = nullptr) -> void;

        template<typename T>
        auto container_ptr_to_value_ptr(void* container, int32_t array_index = 0) -> T*
        {
            return reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(container) + get_offset_for_internal() + get_element_size() * array_index);
        }

        // 4.25+ only, never call directly, these two path functions should never be called outside the internal API
        auto static get_path_name_recursive(const XProperty* property, const void* stop_outer, std::wstring& out_path) -> void;
        auto static get_path_name(const XProperty* property, const void* stop_outer, std::wstring& out_path) -> void;

        // Safe in all versions, it maps to the UObject version in <4.25
        auto static get_full_name(const XProperty* property, std::wstring& out_name, const void* stop_outer) -> void;

        static auto trivial_dump_to_string(void* p_this, std::wstring& out_line) -> void;
        static auto to_string(void* p_this, std::wstring& out_line) -> void;
    };
}


#endif //RC_UNREAL_XPROPERTY_HPP
