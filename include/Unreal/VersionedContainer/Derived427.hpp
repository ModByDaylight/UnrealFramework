#ifndef RC_UNREAL_DERIVED427_HPP
#define RC_UNREAL_DERIVED427_HPP

#include <Unreal/VersionedContainer/Base.hpp>

namespace RC::Unreal
{
    class Derived427 : public Default
    {
        struct FUObjectArray
        {
            using TUObjectArray = Default::FChunkedFixedUObjectArray;

            int32_t obj_first_gc_index;                             // 0x0
            int32_t obj_last_non_gc_index;                          // 0x4
            int32_t max_objects_not_considered_by_gc;               // 0x8
            bool open_for_disregard_for_gc;                         // 0xC
            TUObjectArray obj_objects;                              // 0x10
            CRITICAL_SECTION ObjObjectsCritical;                    // 0x18
            TArray<int32> ObjAvailableList;                         // 0x58
            TArray<void*> UObjectCreateListeners;                   // 0x68
            TArray<FUObjectDeleteListener*> UObjectDeleteListeners; // 0x78

            void AddUObjectDeleteListener(FUObjectDeleteListener* Listener)
            {
                if (UObjectDeleteListeners.Contains(Listener))
                {
                    throw std::runtime_error{"Cannot add a listener because it already exists in TArray"};
                }
                UObjectDeleteListeners.Add(Listener);
            }

            void RemoveUObjectDeleteListener(FUObjectDeleteListener* Listener)
            {
                UObjectDeleteListeners.RemoveSingleSwap(Listener);
            }
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

            // DEBUG INFO -> START
            /*
            printf_s("m_guobjectarray_internal: %p\n", m_guobjectarray_internal);

            // Chunks ptr
            printf_s("chunks ptr: %p\n", m_guobjectarray_internal->obj_objects.objects);

            printf_s("max chunks: %d\n", m_guobjectarray_internal->obj_objects.max_chunks);
            printf_s("num chunks: %d\n", m_guobjectarray_internal->obj_objects.num_chunks);
            printf_s("max elements: %d\n", m_guobjectarray_internal->obj_objects.max_elements);
            printf_s("num elements: %d\n", m_guobjectarray_internal->obj_objects.num_elements);
            //*/
            // DEBUG INFO -> END

            LoopAction loop_action{LoopAction::Continue};

            for (int32_t current_chunk_index{}; current_chunk_index < m_guobjectarray_internal->obj_objects.num_chunks; ++current_chunk_index)
            {
                for (int32_t current_item_index{}; current_item_index < FChunkedFixedUObjectArray::num_elements_per_chunk; ++current_item_index)
                {
                    if (m_guobjectarray_internal->obj_objects.objects[current_chunk_index][current_item_index].object)
                    {
                        loop_action = callable(m_guobjectarray_internal->obj_objects.objects[current_chunk_index][current_item_index].object, current_chunk_index, current_item_index);
                    }

                    if (loop_action == LoopAction::Break) { break; }
                }

                if (loop_action == LoopAction::Break) { break; }
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

        auto Field_get_type_fname(FField* p_this) -> FName override;
        auto Field_get_ffield_owner(FField* p_this) -> void* override;
        auto FFieldClass_get_fname(FFieldClass* p_this) -> FName override;
    };
}


#endif //RC_UNREAL_DERIVED427_HPP
