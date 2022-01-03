#include <Unreal/Property/XMapProperty.hpp>

namespace RC::Unreal
{
    auto XMapProperty::read_data(PropertyDataVC data) -> XMapProperty::InnerType
    {
        int32_t num_map_elements = Helper::Casting::offset_deref<int32_t>(data.data_ptr, 0x8);
        int32_t map_capacity = Helper::Casting::offset_deref<int32_t>(data.data_ptr, 0xC);

        void** elements_ptr = static_cast<void**>(data.data_ptr);
        if (!*elements_ptr)
        {
            // Map is empty
            return InnerType{nullptr, 0, 0};
        }
        else
        {
            return InnerType{*elements_ptr, num_map_elements, map_capacity};
        }
    }

    auto XMapProperty::get_key_property() -> XProperty*
    {
        return Helper::Casting::offset_deref<XProperty*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::MapProperty_KeyProp));
    }

    auto XMapProperty::get_value_property() -> XProperty*
    {
        return Helper::Casting::offset_deref<XProperty*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::MapProperty_ValueProp));
    }

    auto XMapProperty::to_string(void* p_this, std::wstring& out_line) -> void
    {
        trivial_dump_to_string(p_this, out_line);

        XMapProperty* typed_this = static_cast<XMapProperty*>(p_this);
        XProperty* key_property = typed_this->get_key_property();
        XProperty* value_property = typed_this->get_value_property();
        out_line.append(std::format(L" [kp: {:016X}] [vp: {:016X}]", reinterpret_cast<uintptr_t>(key_property), reinterpret_cast<uintptr_t>(value_property)));
    }

    auto XMapProperty::to_string_complex(void* p_this, std::wstring& out_line, const std::function<void(void*)>& callable) -> void
    {
        XMapProperty* typed_this = static_cast<XMapProperty*>(p_this);
        XProperty* key_property = typed_this->get_key_property();
        XProperty* value_property = typed_this->get_value_property();
        auto* key_property_class = key_property->get_ffieldclass();
        auto* value_property_class = value_property->get_ffieldclass();

        auto dump_property = [&](XProperty* property, FFieldClass* property_class) {
            if (Unreal::TypeChecker::to_string_exists(property_class))
            {
                Unreal::TypeChecker::get_to_string(property_class)(property, out_line);

                if (Unreal::TypeChecker::to_string_complex_exists(property_class))
                {
                    // If this code is executed then we'll be having another line before we return to the dumper, so we need to explicitly add a new line
                    // If this code is not executed then we'll not be having another line and the dumper will add the new line
                    out_line.append(L"\n");

                    Unreal::TypeChecker::get_to_string_complex(property_class)(property, out_line, [&]([[maybe_unused]]void* prop) {
                        // It's possible that a new line is supposed to be appended here
                    });
                }

                callable(property);
            }
            else
            {
                property->get_full_name(out_line);
                callable(property);
            }
        };

        dump_property(key_property, key_property_class);
        dump_property(value_property, value_property_class);
    }
}
