#ifndef RC_UNREAL_XMULTICASTINLINEDELEGATEPROPERTY_HPP
#define RC_UNREAL_XMULTICASTINLINEDELEGATEPROPERTY_HPP

#include <Helpers/Format.hpp>

#include <Unreal/FProperty.hpp>
#include <Unreal/FWeakObjectPtr.hpp>
#include <Unreal/Property/XDelegateProperty.hpp>
#include <Unreal/TArray.hpp>

namespace RC::Unreal
{
    class RC_UE_API TMulticastScriptDelegate
    {
    private:
        using FInvocationList = TArray<TScriptDelegate>;

    public:
        FInvocationList invocation_list;
    };

    class RC_UE_API XMulticastInlineDelegateProperty : public FProperty
    {
    private:

    public:
        constexpr static inline bool is_trivial = true;
        constexpr static inline int32_t size = sizeof(uint64_t); // TScriptDelegate
        using InnerType = uint64_t;
        using InnerTypeFull = InnerType*;

        // DO NOT USE... MEANT FOR INTERNAL USE ONLY
        static inline class UClass* m_static_obj{};
        static inline FFieldClassVariant m_static_obj_variant{static_cast<FFieldClass*>(nullptr)};

        auto static static_class() -> FFieldClassVariant
        {
            if (!m_static_obj_variant.is_valid()) { throw std::runtime_error{"[XMulticastInlineDelegateProperty::static_class] m_static_obj_variant is not valid"}; }
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
        auto get_function_signature() -> UFunction*;

    public:
        static auto to_string(void* p_this, std::wstring& out_line) -> void;
    };

    using XMulticastDelegateProperty = XMulticastInlineDelegateProperty;
}


#endif //RC_UNREAL_XMULTICASTINLINEDELEGATEPROPERTY_HPP
