#include <format>
#include <Unreal/Property/XSetProperty.hpp>

namespace RC::Unreal
{
    auto XSetProperty::construct([[maybe_unused]]PropertyDataVC p_data, [[maybe_unused]]XProperty* p_inner_this, void* v_data) -> InnerType*
    {
        return static_cast<InnerType*>(v_data);
    }

    auto XSetProperty::read_data(PropertyDataVC data) -> InnerType*
    {
        return static_cast<InnerType*>(data.data_ptr);
    }

    auto XSetProperty::to_string(void* p_this, std::wstring& out_line) -> void
    {
        trivial_dump_to_string(p_this, out_line);
    }

    auto XSetProperty::get_element_prop() -> XProperty*
    {
        return Helper::Casting::offset_deref<XProperty*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::SetProperty_ElementProp));
    }
}
