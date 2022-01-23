#include <Unreal/Property/XLazyObjectProperty.hpp>

namespace RC::Unreal
{
    auto XLazyObjectProperty::construct([[maybe_unused]]PropertyDataVC p_data, [[maybe_unused]]FProperty* p_inner_this, void* v_data) -> InnerType*
    {
        return static_cast<InnerType*>(v_data);
    }

    auto XLazyObjectProperty::read_data(PropertyDataVC data) -> InnerType*
    {
        return static_cast<InnerType*>(data.data_ptr);
    }

}
