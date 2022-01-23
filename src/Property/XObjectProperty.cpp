#include <format>
#include <Unreal/Property/XObjectProperty.hpp>

namespace RC::Unreal
{
    auto XObjectProperty::get_property_class() -> UClass*
    {
        return Helper::Casting::offset_deref<UClass*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::ObjectProperty_PropertyClass));
    }

    auto XObjectProperty::construct([[maybe_unused]]PropertyDataVC p_data, [[maybe_unused]]FProperty* p_inner_this, void* v_data) -> InnerType*
    {
        return static_cast<InnerType*>(v_data);
    }

    auto XObjectProperty::read_data(PropertyDataVC data) -> InnerType*
    {
        return static_cast<InnerType*>(data.data_ptr);
    }

    auto XObjectProperty::to_string(void* p_this, std::wstring& out_line) -> void
    {
        XObjectProperty* typed_this = static_cast<XObjectProperty*>(p_this);

        trivial_dump_to_string(p_this, out_line);
        out_line.append(std::format(L" [pc: {:016X}]", reinterpret_cast<uintptr_t>(typed_this->get_property_class())));
    }
}
