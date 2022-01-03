#include <format>
#include <Unreal/Property/XWeakObjectProperty.hpp>

namespace RC::Unreal
{
    auto XWeakObjectProperty::construct([[maybe_unused]]PropertyDataVC p_data, [[maybe_unused]]XProperty* p_inner_this, void* v_data) -> InnerType*
    {
        return static_cast<InnerType*>(v_data);
    }

    auto XWeakObjectProperty::read_data(PropertyDataVC data) -> InnerType*
    {
        return static_cast<FWeakObjectPtr*>(data.data_ptr);
    }

    auto XWeakObjectProperty::to_string(void* p_this, std::wstring& out_line) -> void
    {
        XObjectProperty::to_string(p_this, out_line);
    }
}
