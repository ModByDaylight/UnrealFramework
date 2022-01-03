#include <format>
#include <Unreal/Property/XDelegateProperty.hpp>

namespace RC::Unreal
{
    auto XDelegateProperty::construct([[maybe_unused]]PropertyDataVC p_data, [[maybe_unused]]XProperty* p_inner_this, void* v_data) -> InnerType*
    {
        return static_cast<InnerType*>(v_data);
    }

    auto XDelegateProperty::read_data(PropertyDataVC data) -> InnerType*
    {
        return static_cast<InnerType*>(data.data_ptr);
    }

    auto XDelegateProperty::get_function_signature() -> UFunction*
    {
        return Helper::Casting::offset_deref<UFunction*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::DelegateProperty_SignatureFunction));
    }

    auto XDelegateProperty::to_string(void* p_this, std::wstring& out_line) -> void
    {
        trivial_dump_to_string(p_this, out_line);

        XDelegateProperty* p_typed_this = static_cast<XDelegateProperty*>(p_this);
        out_line.append(std::format(L" [df: {:016X}]", reinterpret_cast<uintptr_t>(p_typed_this->get_function_signature())));
    }
}
