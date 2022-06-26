#include <Unreal/Property/FClassProperty.hpp>
#include <Helpers/Casting.hpp>

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FClassProperty);
    IMPLEMENT_FIELD_CLASS(FClassPtrProperty);

    UClass* FClassProperty::GetMetaClass() const
    {
        return Helper::Casting::offset_deref<UClass*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::ClassProperty_MetaClass));
    }

    void FClassProperty::SetMetaClass(UClass* NewMetaClass)
    {
        UClass** MetaClass = Helper::Casting::ptr_cast<UClass**>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::ClassProperty_MetaClass));
        if (!MetaClass) { throw std::runtime_error{"FClassProperty::SetMetaClass: The location of 'MetaClass' was nullptr"}; }

        *MetaClass = NewMetaClass;
    }
}
