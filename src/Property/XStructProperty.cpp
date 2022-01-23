#include <format>
#include <Unreal/Property/XStructProperty.hpp>
#include <Unreal/XStructData.hpp>

namespace RC::Unreal
{
    auto XStructProperty::get_script_struct() -> UScriptStruct*
    {
        return Helper::Casting::offset_deref<UScriptStruct*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::StructProperty_Struct));
    }

    auto XStructProperty::construct(PropertyDataVC p_data, FProperty* p_inner_this, void* v_data) -> InnerType*
    {
        // TODO: Deal with this allocation
        // There really shouldn't need to be an allocation here
        // The problem is that the return type is a pointer so somehow a stack-ptr needs to remain alive after the return ?
        return new InnerType{p_data.uobject_ptr, v_data, p_inner_this};
    }

    auto XStructProperty::read_data(PropertyDataVC data) -> InnerType
    {
        return XStructData{data.uobject_ptr, data.data_ptr, static_cast<XStructProperty*>(data.property_ptr)};
    }

    auto XStructProperty::to_string(void* p_this, std::wstring& out_line) -> void
    {
        XStructProperty* typed_this = static_cast<XStructProperty*>(p_this);

        trivial_dump_to_string(p_this, out_line);
        out_line.append(std::format(L" [ss: {:016X}]", reinterpret_cast<uintptr_t>(typed_this->get_script_struct())));
    }
}
