#ifndef RC_UNREAL_XBYTEPROPERTY_HPP
#define RC_UNREAL_XBYTEPROPERTY_HPP

#include <Unreal/UEnum.hpp>
#include <Helpers/Format.hpp>

#include <Unreal/Property/XNumericProperty.hpp>

namespace RC::Unreal
{
    class RC_API XByteProperty : public XNumericProperty
    {
    private:

    public:
        constexpr static inline bool is_trivial = true;
        constexpr static inline int32_t size = sizeof(uint8_t);
        using InnerType = uint8_t;
        using InnerTypeFull = InnerType*;

        // DO NOT USE... MEANT FOR INTERNAL USE ONLY
        static inline class UClass* m_static_obj{};
        static inline FFieldClassVariant m_static_obj_variant{static_cast<FFieldClass*>(nullptr)};

        auto static static_class() -> FFieldClassVariant
        {
            if (!m_static_obj_variant.is_valid()) { throw std::runtime_error{"[XByteProperty::static_class] m_static_obj_variant is not valid"}; }
            if (m_static_obj_variant.is_uobject())
            {
                return m_static_obj_variant.to_uobject();
            }
            else
            {
                return m_static_obj_variant.to_field();
            }
        }

    private:
        struct Offset
        {
            // Temporary implementation
            constexpr static inline int32_t underlying_enum = 0x78; // "UEnum* Enum"
        };

    public:
        auto static read_data(PropertyDataVC data) -> InnerType*
        {
            return static_cast<InnerType*>(data.data_ptr);
        }

        auto get_enum() -> UEnum*;

    public:
        static auto to_string(void* p_this, std::wstring& out_line) -> void
        {
            trivial_dump_to_string(p_this, out_line);
        }
    };
}


#endif //RC_UNREAL_XBYTEPROPERTY_HPP
