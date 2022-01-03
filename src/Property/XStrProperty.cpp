#include <format>
#include <Unreal/Property/XStrProperty.hpp>

namespace RC::Unreal
{
    auto XStrProperty::construct([[maybe_unused]]PropertyDataVC p_data, [[maybe_unused]]XProperty* p_inner_this, void* v_data) -> InnerType*
    {
        return static_cast<InnerType*>(v_data);
    }

    auto XStrProperty::read_data(PropertyDataVC data) -> InnerType*
    {
        return static_cast<InnerType*>(data.data_ptr);
    }

    auto XStrProperty::to_string(void* p_this, std::wstring& out_line) -> void
    {
        trivial_dump_to_string(p_this, out_line);
    }
}
