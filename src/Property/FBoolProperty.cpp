#include <Unreal/StaticOffsetFinder.hpp>
#include "Unreal/Property/FBoolProperty.h"
#include "Unreal/UnrealHashing.h"

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FBoolProperty);
    IMPLEMENT_VIRTUAL_FUNCTION(FBoolProperty, identical);
    IMPLEMENT_VIRTUAL_FUNCTION(FBoolProperty, export_text_item);
    IMPLEMENT_VIRTUAL_FUNCTION(FBoolProperty, import_text);
    IMPLEMENT_VIRTUAL_FUNCTION(FBoolProperty, get_value_type_hash);
    IMPLEMENT_VIRTUAL_FUNCTION(FBoolProperty, clear_value);
    IMPLEMENT_VIRTUAL_FUNCTION(FBoolProperty, initialize_value);
    IMPLEMENT_VIRTUAL_FUNCTION(FBoolProperty, copy_values_internal);

    auto FBoolProperty::get_byte_offset() -> uint8_t
    {
        return Helper::Casting::offset_deref<uint8_t>(this, StaticOffsetFinder::retrieve_static_offset(StaticOffsetFinder::MemberOffsets::ByteOffset_BoolProperty));
    }

    auto FBoolProperty::get_byte_mask() -> uint8_t
    {
        return Helper::Casting::offset_deref<uint8_t>(this, StaticOffsetFinder::retrieve_static_offset(StaticOffsetFinder::MemberOffsets::ByteMask_BoolProperty));
    }

    auto FBoolProperty::get_field_mask() -> uint8_t
    {
        return Helper::Casting::offset_deref<uint8_t>(this, StaticOffsetFinder::retrieve_static_offset(StaticOffsetFinder::MemberOffsets::FieldMask_BoolProperty));
    }

    auto FBoolProperty::identical_impl(const void* A, const void* B, uint32_t port_flags) -> bool
    {
        const uint8_t byte_offset = get_byte_offset();
        const uint8_t* byte_value_A = (const uint8_t*)A + byte_offset;
        const uint8_t* byte_value_B = (const uint8_t*)B + byte_offset;
        return ((*byte_value_A ^ (B ? *byte_value_B : 0)) & get_field_mask()) == 0;
    }

    auto FBoolProperty::export_text_item_impl(std::wstring& value_str, const void* property_value, const void* default_value, UObject* parent, int32_t port_flags, UObject* export_root_scope) -> void
    {

    }

    auto FBoolProperty::import_text_impl(const wchar_t* buffer, void* data, int32_t port_flags, UObject* owner_object) -> const wchar_t*
    {
        FString Temp;
        Buffer = FPropertyHelpers::ReadToken( Buffer, Temp );
        if( !Buffer )
        {
            return NULL;
        }

        check(FieldSize != 0);
        uint8* ByteValue = (uint8*)Data + ByteOffset;

        const FCoreTexts& CoreTexts = FCoreTexts::Get();
        if( Temp==TEXT("1") || Temp==TEXT("True") || Temp==*CoreTexts.True.ToString() || Temp == TEXT("Yes") || Temp == *CoreTexts.Yes.ToString() )
        {
            *ByteValue |= ByteMask;
        }
        else if( Temp==TEXT("0") || Temp==TEXT("False") || Temp==*CoreTexts.False.ToString() || Temp == TEXT("No") || Temp == *CoreTexts.No.ToString() )
        {
            *ByteValue &= ~FieldMask;
        }
        else
        {
            //UE_LOG(LogProperty, Log,  "Import: Failed to get bool" );
            return NULL;
        }
        return Buffer;
    }

    uint32_t FBoolProperty::get_value_type_hash_impl(const void* src)
    {
        return get_type_hash(*(const bool*)src);
    }

    auto FBoolProperty::clear_value_impl(void* data) -> void
    {
        uint8_t* byte_value = (uint8_t*) data + get_byte_offset();
        *byte_value &= ~get_field_mask();
    }

    auto FBoolProperty::initialize_value_impl(void* dest) -> void
    {
        uint8_t* byte_value = (uint8_t*) dest + get_byte_offset();
        *byte_value &= ~get_field_mask();
    }

    auto FBoolProperty::copy_values_internal_impl(void* dest, const void* src, int32_t count) -> void
    {
        int32_t element_size = get_element_size();
        uint8_t byte_offset = get_byte_offset();
        uint8_t field_mask = get_field_mask();

        for (int32_t i = 0; i < count; i++)
        {
            uint8_t* dest_byte_value = (uint8_t*)dest + i * element_size + byte_offset;
            uint8_t* src_byte_value = (uint8_t*)src + i * element_size + byte_offset;
            *dest_byte_value = (*dest_byte_value & ~field_mask) | (*src_byte_value & field_mask);
        }
    }
}

