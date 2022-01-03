#include <format>
#include <Unreal/Property/XClassProperty.hpp>

namespace RC::Unreal
{
    auto XClassProperty::construct([[maybe_unused]]PropertyDataVC p_data, [[maybe_unused]]XProperty* p_inner_this, void* v_data) -> InnerType*
    {
        return static_cast<InnerType*>(v_data);
    }

    auto XClassProperty::read_data(PropertyDataVC data) -> InnerType*
    {
        return static_cast<InnerType*>(data.data_ptr);
    }

    auto XClassProperty::get_meta_class() -> UClass*
    {
        return Helper::Casting::offset_deref<UClass*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::ClassProperty_MetaClass));
    }

    auto XClassProperty::to_string(void* p_this, std::wstring& out_line) -> void
    {
        XClassProperty* typed_this = static_cast<XClassProperty*>(p_this);

        trivial_dump_to_string(p_this, out_line);
        // mc = MetaClass
        out_line.append(std::format(L" [mc: {:016X}]", reinterpret_cast<uintptr_t>(typed_this->get_meta_class())));
    }
}
