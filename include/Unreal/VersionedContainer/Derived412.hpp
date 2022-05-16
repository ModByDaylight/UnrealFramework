#ifndef RC_UNREAL_DERIVED412_HPP
#define RC_UNREAL_DERIVED412_HPP

#include <Unreal/VersionedContainer/Base.hpp>

namespace RC::Unreal
{
    class Derived412 : public Base
    {
    private:
        // FUObjectItem -> START
        using UObjectBase = UObject;

        struct FUObjectItem
        {
            UObjectBase* object;
            int32_t cluster_and_flags;
            int32_t serial_number;

            [[nodiscard]] auto is_unreachable() const -> bool
            {
                return !!(cluster_and_flags & static_cast<int32_t>(EInternalObjectFlags::Unreachable));
            }
            [[nodiscard]] auto is_pending_kill() const
            {
                return !!(cluster_and_flags & static_cast<int32_t>(EInternalObjectFlags::PendingKill));
            }
            auto set_root_set(SetOrUnsetFlag set_or_unset_flag)
            {
                if (set_or_unset_flag == SetOrUnsetFlag::Set)
                {
                    cluster_and_flags = cluster_and_flags | static_cast<int32_t>(EInternalObjectFlags::RootSet);
                }
                else
                {
                    cluster_and_flags = cluster_and_flags & ~static_cast<int32_t>(EInternalObjectFlags::RootSet);
                }
            }
            auto set_gc_keep(SetOrUnsetFlag set_or_unset_flag)
            {
                if (set_or_unset_flag == SetOrUnsetFlag::Set)
                {
                    cluster_and_flags = cluster_and_flags | static_cast<int32_t>(EInternalObjectFlags::GarbageCollectionKeepFlags);
                }
                else
                {
                    cluster_and_flags = cluster_and_flags & ~static_cast<int32_t>(EInternalObjectFlags::GarbageCollectionKeepFlags);
                }
            }
        };

        // Abstraction Layer -> START
        auto FUObjectItem_is_object_unreachable(const void* p_this) const -> bool override
        {
            if (!p_this) { return true; }
            auto* p_typed_this = static_cast<const FUObjectItem*>(p_this);

            if (!p_typed_this->object) { return true; }

            return p_typed_this->is_unreachable();
        }

        auto FUObjectItem_set_object_root_set(void* p_this, SetOrUnsetFlag set_or_unset_flag) -> void override
        {
            if (!p_this) { return; }
            FUObjectItem* p_typed_this = static_cast<FUObjectItem*>(p_this);

            p_typed_this->set_root_set(set_or_unset_flag);
        }

        auto FUObjectItem_set_object_gc_keep(void* p_this, SetOrUnsetFlag set_or_unset_flag) -> void override
        {
            if (!p_this) { return; }
            FUObjectItem* p_typed_this = static_cast<FUObjectItem*>(p_this);

            p_typed_this->set_gc_keep(set_or_unset_flag);
        }

        auto FUObjectItem_is_valid(void* p_this, bool b_even_if_pending_kill) -> bool override
        {
            FUObjectItem* p_typed_this = static_cast<FUObjectItem*>(p_this);

            if (p_typed_this)
            {
                return b_even_if_pending_kill ? !p_typed_this->is_unreachable() : !(p_typed_this->is_unreachable() || p_typed_this->is_pending_kill());
            }
            return false;
        }

        auto FUObjectItem_get_serial_number(int32_t index) -> int32_t override
        {
            FUObjectItem* object_item = static_cast<FUObjectItem*>(UObjectArray_index_to_object(index));
            return object_item->serial_number;
        }

        auto FUObjectItem_get_serial_number(void* p_this) -> int32_t override
        {
            FUObjectItem* p_typed_this = static_cast<FUObjectItem*>(p_this);
            return p_typed_this->serial_number;
        }

        auto FUObjectItem_get_uobject(const void* object_item_param) -> void* override
        {
            auto* object_item = static_cast<const FUObjectItem*>(object_item_param);

            if (object_item)
            {
                return object_item->object;
            }
            else
            {
                return nullptr;
            }
        }

        bool FUObjectItem_HasAnyFlags(void* RawThis, EInternalObjectFlags InFlags) override;
        // Abstraction Layer -> END
        // FUObjectItem -> END

        // GUObjectArray -> START
        struct FFixedUObjectArray
        {
            FUObjectItem* objects;
            int32_t max_elements;
            int32_t num_elements;

            template<typename ReturnType = FUObjectItem const*>
            [[nodiscard]] auto get_object_ptr(int32_t index) const -> ReturnType
            {
                // This would be some sort of assert in UE4
                if (index >= 0 && index < num_elements)
                {
                    return &objects[index];
                }
                else
                {
                    return nullptr;
                }
            }

            [[nodiscard]] FUObjectItem const& operator[](int32_t index) const
            {
                return *get_object_ptr(index);
            }

            [[nodiscard]] FUObjectItem& operator[](int32_t index)
            {
                return *get_object_ptr<FUObjectItem*>(index);
            }
        };

    public:
        struct FUObjectArray
        {
            using TUObjectArray = FFixedUObjectArray;

            int32_t obj_first_gc_index;
            int32_t obj_last_non_gc_index;
            int32_t max_objects_not_considered_by_gc;
            bool open_for_disregard_for_gc;
            TUObjectArray obj_objects;

            CRITICAL_SECTION_414_AND_BELOW ObjObjectsCritical;      // 0x18
            // Real type: TLockFreePointerListUnordered
            // In <4.17, this is just a pointer.
            uint8 ObjAvailableList[0x8];                            // 0x40
            TArray<void*> UObjectCreateListeners;                   // 0x48
            TArray<FUObjectDeleteListener*> UObjectDeleteListeners; // 0x58
        };

        using GUObjectArray = FUObjectArray;

    protected:
        static inline GUObjectArray* m_guobjectarray_internal{};

    public:
        auto UObjectArray_set_internal_storage_ptr(void* ptr) -> void override
        {
            m_guobjectarray_internal = static_cast<GUObjectArray*>(ptr);
        }

        auto UObjectArray_get_internal_storage_ptr() -> void* override
        {
            return m_guobjectarray_internal;
        }

        auto UObjectArray_for_each_uobject(ForEachObjObjectsCallableType callable) -> void override
        {
            if (!m_guobjectarray_internal) { return; }

            for (int32_t current_item_index{}; current_item_index < m_guobjectarray_internal->obj_objects.num_elements; ++current_item_index)
            {
                if (m_guobjectarray_internal->obj_objects[current_item_index].object)
                {
                    if (callable(m_guobjectarray_internal->obj_objects.objects[current_item_index].object, 0, current_item_index) == LoopAction::Break)
                    {
                        break;
                    }
                }
            }
        }

        auto UObjectArray_index_to_object(int32_t index) -> void* override
        {
            if (index >= 0 && index < m_guobjectarray_internal->obj_objects.num_elements)
            {
                return &m_guobjectarray_internal->obj_objects[index];
            }
            return nullptr;
        }

        auto UObjectArray_get_obj_last_non_gc_index() -> int32_t override
        {
            return m_guobjectarray_internal->obj_last_non_gc_index;
        }

        auto UObjectArray_get_uobject_from_index(int32_t index) -> UObject* override
        {
            FUObjectItem* object_item = static_cast<FUObjectItem*>(UObjectArray_index_to_object(index));
            if (object_item)
            {
                return object_item->object;
            }
            else
            {
                return nullptr;
            }
        }

        auto UObjectArray_get_max_elements() -> int32_t override
        {
            return m_guobjectarray_internal->obj_objects.max_elements;
        }

        auto UObjectArray_get_num_elements() -> int32_t override
        {
            return m_guobjectarray_internal->obj_objects.num_elements;
        }

        void UObjectArray_AddUObjectCreateListener(FUObjectCreateListener* Listener) override;
        void UObjectArray_RemoveUObjectCreateListener(FUObjectCreateListener* Listener) override;
        void UObjectArray_AddUObjectDeleteListener(FUObjectDeleteListener* Listener) override;
        void UObjectArray_RemoveUObjectDeleteListener(FUObjectDeleteListener* Listener) override;
        // GUObjectArray -> END

    public:
        auto Field_get_type_fname(FField* p_this) -> FName override;
        auto Field_get_ffield_owner([[maybe_unused]]FField* p_this) -> void* override;
        auto FFieldClass_get_fname(FFieldClass* p_this) -> FName override;
    };
}


#endif //RC_UNREAL_DERIVED412_HPP
