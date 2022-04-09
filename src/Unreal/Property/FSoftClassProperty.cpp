#include <Unreal/Property/FSoftClassProperty.hpp>
#include <Helpers/Casting.hpp>

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FSoftClassProperty);

    UClass* FSoftClassProperty::GetMetaClass()
    {
        return Helper::Casting::offset_deref<UClass*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::SoftClassProperty_MetaClass));
    }

    void FSoftClassProperty::SetMetaClass(UClass* NewMetaClass)
    {
        UClass** MetaClass = Helper::Casting::ptr_cast<UClass**>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::SoftClassProperty_MetaClass));
        if (!MetaClass) { throw std::runtime_error{"FSoftClassProperty::SetMetaClass: The location of 'MetaClass' was nullptr"}; }

        *MetaClass = NewMetaClass;
    }
}
