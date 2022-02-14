#include <Unreal/Property/FClassProperty.hpp>

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FClassProperty);
    IMPLEMENT_VIRTUAL_FUNCTION(FClassProperty, get_value_type_hash);

    auto FClassProperty::GetMetaClass() -> UClass*
    {
        return Helper::Casting::offset_deref<UClass*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::ClassProperty_MetaClass));
    }
}
