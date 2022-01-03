#include <format>
#include <Unreal/Property/XInterfaceProperty.hpp>

namespace RC::Unreal
{
    auto XInterfaceProperty::get_interface_class() -> UClass*
    {
        return Helper::Casting::offset_deref<UClass*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::InterfaceProperty_InterfaceClass));
    }

    auto XInterfaceProperty::construct([[maybe_unused]]PropertyDataVC p_data, [[maybe_unused]]XProperty* p_inner_this, void* v_data) -> InnerType*
    {
        return static_cast<InnerType*>(v_data);
    }

    auto XInterfaceProperty::read_data(PropertyDataVC data) -> InnerType*
    {
        return static_cast<InnerType*>(data.data_ptr);
    }

    auto XInterfaceProperty::to_string(void* p_this, std::wstring& out_line) -> void
    {
        XInterfaceProperty* typed_this = static_cast<XInterfaceProperty*>(p_this);

        trivial_dump_to_string(p_this, out_line);
        out_line.append(std::format(L" [ic: {:016X}]", reinterpret_cast<uintptr_t>(typed_this->get_interface_class())));
    }
}
