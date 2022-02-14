#include <Unreal/Property/FClassProperty.hpp>

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FClassProperty);

    auto FClassProperty::GetMetaClass() -> UClass*
    {
        return Helper::Casting::offset_deref<UClass*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::ClassProperty_MetaClass));
    }
}
