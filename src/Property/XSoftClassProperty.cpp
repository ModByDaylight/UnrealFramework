#include <Unreal/Property/XSoftClassProperty.hpp>

namespace RC::Unreal
{
    auto XSoftClassProperty::construct([[maybe_unused]]PropertyDataVC p_data, [[maybe_unused]]XProperty* p_inner_this, void* v_data) -> InnerType*
    {
        return static_cast<InnerType*>(v_data);
    }

    auto XSoftClassProperty::read_data(PropertyDataVC data) -> InnerType*
    {
        return static_cast<InnerType*>(data.data_ptr);
    }

    auto XSoftClassProperty::get_meta_class() -> UClass*
    {
        return Helper::Casting::offset_deref<UClass*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::SoftClassProperty_MetaClass));
    }

}
