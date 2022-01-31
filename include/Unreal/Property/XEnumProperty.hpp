#ifndef RC_UNREAL_XENUMPROPERTY_HPP
#define RC_UNREAL_XENUMPROPERTY_HPP

#include <Unreal/FProperty.hpp>
#include <format>
#include <Unreal/UnrealVersion.hpp>

namespace RC::Unreal
{
    class XNumericProperty;
    class UEnum;
    template<typename InnerType>
    class TUEnum;

    template<typename InnerEnumType>
    class XEnumProperty : public FProperty
    {
    private:

    public:
        constexpr static inline bool is_trivial = true;
        constexpr static inline int32_t size = sizeof(uint8_t); // Assuming it's always the size of a byte
        using InnerType = TUEnum<InnerEnumType>;
        using InnerTypeFull = InnerType*;

        // DO NOT USE... MEANT FOR INTERNAL USE ONLY
        static inline class UClass* m_static_obj{};
        static inline FFieldClassVariant m_static_obj_variant{static_cast<FFieldClass*>(nullptr)};

        auto static static_class() -> FFieldClassVariant
        {
            if (Version::is_atmost(4, 15) || !m_static_obj_variant.is_valid()) { throw std::runtime_error{"[XEnumProperty::static_class] m_static_obj_variant is not valid"}; }
            if (m_static_obj_variant.is_uclass())
            {
                return m_static_obj_variant.to_uclass();
            }
            else
            {
                return m_static_obj_variant.to_field_class();
            }
        }

    public:
        auto static construct([[maybe_unused]]PropertyDataVC p_data, [[maybe_unused]]FProperty* p_inner_this, void* v_data) -> InnerType*
        {
            return static_cast<InnerType*>(v_data);
        }

        auto static read_data(PropertyDataVC data) -> InnerType*
        {
            return static_cast<uint8_t*>(data.data_ptr);
        }

    public:
        auto get_unreal_enum() -> UEnum*
        {
            return Helper::Casting::offset_deref<UEnum*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::EnumProperty_Enum));
        }

        auto get_underlying_type() -> XNumericProperty*
        {
            return Helper::Casting::offset_deref<XNumericProperty*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::EnumProperty_UnderlyingType));
        }

    public:
        static auto to_string(void* p_this, std::wstring& out_line) -> void
        {
            trivial_dump_to_string(p_this, out_line);

            auto* typed_this = static_cast<XEnumProperty<uint8_t>*>(p_this);
            out_line.append(std::format(L" [em: {:016X}]", reinterpret_cast<uintptr_t>(typed_this->get_unreal_enum())));
        }
    };
}


#endif //RC_UNREAL_XENUMPROPERTY_HPP
