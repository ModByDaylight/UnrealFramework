#ifndef RC_UNREAL_XBOOLPROPERTY_HPP
#define RC_UNREAL_XBOOLPROPERTY_HPP

#include <Helpers/Format.hpp>

#include <Unreal/XProperty.hpp>

namespace RC::Unreal
{
    class RC_UE_API XBoolProperty : public XProperty
    {
    private:

    public:
        constexpr static inline bool is_trivial = false;
        constexpr static inline int32_t size = sizeof(bool);
        using InnerType = bool;
        using InnerTypeFull = InnerType;

        // DO NOT USE... MEANT FOR INTERNAL USE ONLY
        static inline class UClass* m_static_obj{};
        static inline FFieldClassVariant m_static_obj_variant{static_cast<FFieldClass*>(nullptr)};

        auto static static_class() -> FFieldClassVariant
        {
            if (!m_static_obj_variant.is_valid()) { throw std::runtime_error{"[XBoolProperty::static_class] m_static_obj_variant is not valid"}; }
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
        auto get_field_mask() -> uint8_t
        {
            return Helper::Casting::offset_deref<uint8_t>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::FieldMask_BoolProperty));
        }

        auto get_byte_offset() -> uint8_t
        {
            return Helper::Casting::offset_deref<uint8_t>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::ByteOffset_BoolProperty));
        }

        auto get_byte_mask() -> uint8_t
        {
            return Helper::Casting::offset_deref<uint8_t>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::ByteMask_BoolProperty));
        }

        auto static read_data(PropertyDataVC data) -> InnerType
        {
            XBoolProperty* typed_property = static_cast<XBoolProperty*>(data.property_ptr);
            uint8_t* byte_value = static_cast<uint8_t*>(data.data_ptr) + typed_property->get_byte_offset();
            return !!(*byte_value & typed_property->get_field_mask());
        }

        auto static write_data(PropertyDataVC data, InnerType new_data) -> void
        {
            // Code to set the bool here
            // This is necessary because a bool can be part of a bitfields so we cannot return a pointer to the bool in the read_data function
            XBoolProperty* typed_property = static_cast<XBoolProperty*>(data.property_ptr);
            uint8_t* byte_value = static_cast<uint8_t*>(data.data_ptr) + typed_property->get_byte_offset();
            *byte_value = ((*byte_value) & ~typed_property->get_field_mask() | (new_data ? typed_property->get_byte_mask() : 0));
        }

    public:
        // More efficient method for flipping a bool
        // Please use this instead of read_data & write_data whenever possible
        auto flip_bool(void* uobject) -> void
        {
            void* data_ptr = static_cast<void*>(static_cast<char*>(uobject) + get_offset_for_internal());

            uint8_t* byte_value = static_cast<uint8_t*>(data_ptr) + get_byte_offset();
            uint8_t field_mask = get_field_mask();
            bool current_data = !!(*byte_value & field_mask);

            *byte_value = ((*byte_value) & ~field_mask | (!current_data ? get_byte_mask() : 0));
        }

        static auto to_string(void* p_this, std::wstring& out_line) -> void
        {
            trivial_dump_to_string(p_this, out_line);
        }
    };
}


#endif //RC_UNREAL_XBOOLPROPERTY_HPP
