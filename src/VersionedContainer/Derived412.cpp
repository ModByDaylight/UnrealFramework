#include <Unreal/VersionedContainer/Derived412.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/UStruct.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/FField.hpp>
#include <Unreal/FProperty.hpp>

namespace RC::Unreal
{
    auto Derived412::Field_get_type_fname(FField* p_this) -> FName
    {
        return p_this->GetClass().GetFName();
    }

    auto Derived412::Field_get_ffield_owner([[maybe_unused]]FField* p_this) -> void*
    {
        // FField does not exist before 4.25 so therefor FField::Owner also does not exist
        // It's up to the caller to make sure the returned value isn't nullptr
        return nullptr;
    }

    auto Derived412::FFieldClass_get_fname(FFieldClass* p_this) -> FName
    {
        return Helper::Casting::offset_deref<FName>(p_this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UObject_NamePrivate));
    }

    bool Derived412::FUObjectItem_HasAnyFlags(void* RawThis, EInternalObjectFlags InFlags)
    {
        FUObjectItem* TypedThis = static_cast<FUObjectItem*>(RawThis);
        return !!(TypedThis->cluster_and_flags & int32(InFlags));
    }
}
