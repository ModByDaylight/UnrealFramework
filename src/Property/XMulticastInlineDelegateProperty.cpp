#include <format>
#include <Unreal/Property/XMulticastInlineDelegateProperty.hpp>

namespace RC::Unreal
{
    auto XMulticastInlineDelegateProperty::construct([[maybe_unused]]PropertyDataVC p_data, [[maybe_unused]]XProperty* p_inner_this, void* v_data) -> InnerType*
    {
        return static_cast<InnerType*>(v_data);
    }

    auto XMulticastInlineDelegateProperty::read_data(PropertyDataVC data) -> InnerType*
    {
        return static_cast<InnerType*>(data.data_ptr);
    }

    auto XMulticastInlineDelegateProperty::get_function_signature() -> UFunction*
    {
        return Helper::Casting::offset_deref<UFunction*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::MulticastInlineDelegateProperty_SignatureFunction));
    }

    auto XMulticastInlineDelegateProperty::to_string(void* p_this, std::wstring& out_line) -> void
    {
        trivial_dump_to_string(p_this, out_line);

        XMulticastInlineDelegateProperty* p_typed_this = static_cast<XMulticastInlineDelegateProperty*>(p_this);
        out_line.append(std::format(L" [df: {:016X}]", reinterpret_cast<uintptr_t>(p_typed_this->get_function_signature())));
    }
}
