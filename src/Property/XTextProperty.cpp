#include <format>
#include <Unreal/Property/XTextProperty.hpp>

namespace RC::Unreal
{
    auto XTextProperty::construct([[maybe_unused]]PropertyDataVC p_data, [[maybe_unused]]FProperty* p_inner_this, void* v_data) -> InnerType*
    {
        return static_cast<InnerType*>(v_data);
    }

    auto XTextProperty::read_data(PropertyDataVC data) -> InnerType*
    {
        return static_cast<InnerType*>(data.data_ptr);
    }

    auto XTextProperty::to_string(void* p_this, std::wstring& out_line) -> void
    {
        trivial_dump_to_string(p_this, out_line);
    }
}
