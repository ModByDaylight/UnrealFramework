#include <Unreal/FWeakObjectPtr.hpp>
#include <Unreal/VersionedContainer/Container.hpp>
#include <Unreal/UObjectArray.hpp>
#include <Unreal/UObject.hpp>
#include <DynamicOutput/Output.hpp>

namespace RC::Unreal
{
    auto FWeakObjectPtr::serial_numbers_match(FUObjectItem* object_item) const -> bool
    {
        return Container::m_unreal_vc_base->FUObjectItem_get_serial_number(object_item) == object_serial_number;
    }

    auto FWeakObjectPtr::internal_get_object_item() const -> FUObjectItem*
    {
        if (object_serial_number == 0) { return nullptr; }
        if (object_index < 0) { return nullptr; }

        FUObjectItem* const object_item = static_cast<FUObjectItem* const>(Container::m_unreal_vc_base->UObjectArray_index_to_object(object_index));

        if (!object_item) { return nullptr; }
        if (!serial_numbers_match(object_item)) { return nullptr; }

        return object_item;
    }

    auto FWeakObjectPtr::reset() -> void
    {
        object_index = INDEX_NONE;
        object_serial_number = 0;
    }

    auto FWeakObjectPtr::operator=(UObject* new_object) -> void
    {
        if (!new_object)
        {
            reset();
        }
        else
        {
            object_index = new_object->get_internal_index();
            object_serial_number = Container::m_unreal_vc_base->FUObjectItem_get_serial_number(object_index);
        }
    }

    auto FWeakObjectPtr::internal_get(bool b_even_if_pending_kill) const -> UObject*
    {
        FUObjectItem* const object_item = internal_get_object_item();
        return ((object_item != nullptr) && UObjectArray::is_valid(object_item, b_even_if_pending_kill)) ? object_item->get_uobject() : nullptr;
    }

    auto FWeakObjectPtr::get() const -> UObject*
    {
        return internal_get(false);
    }
}
