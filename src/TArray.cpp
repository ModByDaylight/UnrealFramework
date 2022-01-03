#include <Unreal/TArray.hpp>
#include <Unreal/XStructData.hpp>

namespace RC::Unreal
{
    auto get_struct_data_at(void* data_param, size_t index) -> XStructData*
    {
        XStructData* data = static_cast<XStructData*>(data_param);

        XStructProperty* property = data->get_property_ptr();
        XStructData* struct_data = property->construct({nullptr, data->get_uobject_ptr(), data->get_data_ptr()}, property, static_cast<void*>(data->get_data_ptr()));

        auto next_data_ptr = static_cast<void*>(static_cast<uint8_t*>(struct_data->get_data_ptr()) + (index * property->get_script_struct()->get_size()));
        struct_data->set_data_ptr(next_data_ptr);
        return struct_data;
    }
}