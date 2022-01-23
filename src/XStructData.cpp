#include <Unreal/XStructData.hpp>
#include <Unreal/UClass.hpp>

namespace RC
{
    auto Unreal::XStructData::get_struct_child(const wchar_t* struct_prop) -> Unreal::FProperty*
    {
        if (!struct_prop) { throw std::runtime_error{"[XStructDataImpl::get_struct_child] Param 'struct_prop' is nullptr"}; }

        FName struct_prop_name = FName(struct_prop);

        return get_struct_child(struct_prop_name);
    }

    auto Unreal::XStructData::get_struct_child(Unreal::FName struct_prop_name) -> Unreal::FProperty*
    {
        UField* script_struct = m_prop->get_script_struct();
        if (!script_struct) { throw std::runtime_error{"[XStructDataImpl::get_struct_child] Variable 'script_struct' is nullptr"}; }

        FProperty* selected_property{};
        script_struct->for_each_property([&](FProperty* child) {
            if (child->get_fname() == struct_prop_name)
            {
                selected_property = child;
                return LoopAction::Break;
            }
            else
            {
                return LoopAction::Continue;
            }
        });

        if (!selected_property)
        {
            // TODO: Implement some sort of custom property system and try it before finally throwing
            throw std::runtime_error{fmt(
                    "[XStructDataImpl::get_struct_child] Was unable to find a property with the name '%S' in StructProperty '%S'",
                    struct_prop_name.to_string().c_str(),
                    m_prop->get_fname().to_string().c_str()
            )};
        }

        return selected_property;
    }
    auto Unreal::XStructData::get_internal_complex_value(const wchar_t* struct_prop, Unreal::CustomProperty* custom_property) -> Unreal::PropertyDataVC
    {
        // Exception thrown inside the call if property was not found
        FProperty* selected_property = custom_property ? custom_property : get_struct_child(struct_prop);
        int32_t offset_internal = selected_property->get_offset_for_internal();

        void* data_ptr = static_cast<void*>(static_cast<char*>(m_data) + offset_internal);
        if (!data_ptr)
        {
            throw std::runtime_error{fmt("[XStructDataImpl::get_internal_complex] data_ptr is nullptr (selected_property: %p)", selected_property)};
        }

        PropertyDataVC data{.property_ptr = selected_property, .uobject_ptr = m_uobject, .data_ptr = data_ptr};
        return data;
    }
}
