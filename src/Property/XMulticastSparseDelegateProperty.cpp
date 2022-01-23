#include <format>
#include <Unreal/Property/XMulticastSparseDelegateProperty.hpp>

namespace RC::Unreal
{
    auto XMulticastSparseDelegateProperty::construct([[maybe_unused]]PropertyDataVC p_data, [[maybe_unused]]FProperty* p_inner_this, void* v_data) -> InnerType*
    {
        return static_cast<InnerType*>(v_data);
    }

    auto XMulticastSparseDelegateProperty::read_data(PropertyDataVC data) -> InnerType*
    {
        return static_cast<InnerType*>(data.data_ptr);
    }

    auto XMulticastSparseDelegateProperty::get_function_signature() -> UFunction*
    {
        return Helper::Casting::offset_deref<UFunction*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::MulticastSparseDelegateProperty_SignatureFunction));
    }

    auto XMulticastSparseDelegateProperty::to_string(void* p_this, std::wstring& out_line) -> void
    {
        trivial_dump_to_string(p_this, out_line);

        XMulticastSparseDelegateProperty* p_typed_this = static_cast<XMulticastSparseDelegateProperty*>(p_this);
        out_line.append(std::format(L" [df: {:016X}]", reinterpret_cast<uintptr_t>(p_typed_this->get_function_signature())));
    }
}
