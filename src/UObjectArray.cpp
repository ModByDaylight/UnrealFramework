#include <Unreal/UObjectArray.hpp>
#include <Unreal/VersionedContainer/Container.hpp>

namespace RC::Unreal
{
    auto FUObjectItem::IsUnreachable() const -> bool
    {
        return Container::UnrealVC->FUObjectItem_is_object_unreachable(this);
    }

    auto FUObjectItem::SetRootSet() -> void
    {
        Container::UnrealVC->FUObjectItem_set_object_root_set(this, Base::SetOrUnsetFlag::Set);
    }

    auto FUObjectItem::UnsetRootSet() -> void
    {
        Container::UnrealVC->FUObjectItem_set_object_root_set(this, Base::SetOrUnsetFlag::Unset);
    }

    auto FUObjectItem::SetGCKeep() -> void
    {
        Container::UnrealVC->FUObjectItem_set_object_gc_keep(this, Base::SetOrUnsetFlag::Set);
    }

    auto FUObjectItem::UnsetGCKeep() -> void
    {
        Container::UnrealVC->FUObjectItem_set_object_gc_keep(this, Base::SetOrUnsetFlag::Unset);
    }

    auto FUObjectItem::GetUObject() const -> UObject*
    {
        return static_cast<UObject*>(Container::UnrealVC->FUObjectItem_get_uobject(this));
    }

    auto UObjectArray::SetupGUObjectArrayAddress(void* address) -> void
    {
        Container::UnrealVC->UObjectArray_set_internal_storage_ptr(address);
    }

    auto UObjectArray::IsValid(FUObjectItem* ObjectItem, bool bEvenIfPendingKill) -> bool
    {
        return Container::UnrealVC->FUObjectItem_is_valid(ObjectItem, bEvenIfPendingKill);
    }

    void UObjectArray::AddUObjectDeleteListener(FUObjectDeleteListener* Listener)
    {
        Container::UnrealVC->UObjectArray_AddUObjectDeleteListener(Listener);
    }

    void UObjectArray::RemoveUObjectDeleteListener(FUObjectDeleteListener* Listener)
    {
        Container::UnrealVC->UObjectArray_RemoveUObjectDeleteListener(Listener);
    }
}