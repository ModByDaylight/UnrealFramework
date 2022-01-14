#include <Unreal/UObjectArray.hpp>
#include <Unreal/VersionedContainer/Container.hpp>

namespace RC::Unreal
{
    auto FUObjectItem::is_unreachable() -> bool
    {
        return Container::m_unreal_vc_base->FUObjectItem_is_object_unreachable(this);
    }

    auto FUObjectItem::set_root_set() -> void
    {
        Container::m_unreal_vc_base->FUObjectItem_set_object_root_set(this);
    }

    auto FUObjectItem::set_gc_keep() -> void
    {
        Container::m_unreal_vc_base->FUObjectItem_set_object_gc_keep(this);
    }

    auto FUObjectItem::get_uobject() -> UObject*
    {
        return static_cast<UObject*>(Container::m_unreal_vc_base->FUObjectItem_get_uobject(this));
    }

    auto UObjectArray::setup_guobjectarray_address(void* address) -> void
    {
        Container::m_unreal_vc_base->UObjectArray_set_internal_storage_ptr(address);
    }

    auto UObjectArray::is_valid(FUObjectItem* object_item, bool b_even_if_pending_kill) -> bool
    {
        return Container::m_unreal_vc_base->FUObjectItem_is_valid(object_item, b_even_if_pending_kill);
    }
}