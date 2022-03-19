#ifndef RC_UNREAL_BASE_HPP
#define RC_UNREAL_BASE_HPP

#include <functional>

#include <Constructs/Loop.hpp>
#include <Helpers/Casting.hpp>

#include <Unreal/Common.hpp>
#include <Unreal/StaticOffsetFinder.hpp>
#include <Unreal/NameTypes.hpp>

namespace RC::Unreal
{
    class CustomProperty;
    class FField;
    class FFieldClass;
    class FProperty;

    struct RC_UE_API PropertyDataVC
    {
        void* property_ptr{};
        void* uobject_ptr{};
        void* data_ptr{};
        bool is_custom{false};
    };

    enum class ExcludeSelf
    {
        Yes,
        No
    };

    class Base
    {
    protected:
        using MemberOffsets = ::RC::Unreal::StaticOffsetFinder::MemberOffsets;
        class FProperty* m_current_property{};

    public:
        virtual ~Base() = default; // Virtual Destructor
        Base() = default; // Constructor
        Base(const Base&) = delete;  // Copy constructor
        Base(Base&&) = delete;  // Move constructor
        Base& operator=(const Base&) = delete;  // Copy assignment operator
        Base& operator=(Base&&) = delete;  // Move assignment operator

        // FUObjectItem -> START
        using FUObjectItem = void;
        using UObject = void;
        enum class SetOrUnsetFlag { Set, Unset };
        virtual auto FUObjectItem_is_object_unreachable(FUObjectItem* p_this) const -> bool = 0;
        virtual auto FUObjectItem_set_object_root_set(FUObjectItem* p_this, SetOrUnsetFlag) -> void = 0;
        virtual auto FUObjectItem_set_object_gc_keep(FUObjectItem* p_this, SetOrUnsetFlag) -> void = 0;
        virtual auto FUObjectItem_is_valid(FUObjectItem* p_this, bool b_even_if_pending_kill) -> bool = 0;
        virtual auto FUObjectItem_get_serial_number(int32_t index) -> int32_t = 0;
        virtual auto FUObjectItem_get_serial_number(FUObjectItem* p_this) -> int32_t = 0;
        virtual auto FUObjectItem_get_uobject(void* p_this) -> UObject* = 0;
        virtual bool FUObjectItem_HasAnyFlags(void* p_this, EInternalObjectFlags InFlags) = 0;
        // FUObjectItem -> END

        // GUObjectArray -> START
        using GUObjectArray = void;
        virtual auto UObjectArray_set_internal_storage_ptr(GUObjectArray* ptr) -> void = 0;
        virtual auto UObjectArray_get_internal_storage_ptr() -> GUObjectArray* = 0;

        using ForEachObjObjectsCallableType = std::function<LoopAction(void*, int32_t, int32_t)>;
        virtual auto UObjectArray_for_each_uobject(ForEachObjObjectsCallableType callable) -> void = 0;

        virtual auto UObjectArray_index_to_object(int32_t index) -> FUObjectItem* = 0;
        virtual auto UObjectArray_get_obj_last_non_gc_index() -> int32_t = 0;
        virtual auto UObjectArray_get_uobject_from_index(int32_t index) -> UObject* = 0;
        virtual auto UObjectArray_get_max_elements() -> int32_t = 0;
        virtual auto UObjectArray_get_num_elements() -> int32_t = 0;
        // GUObjectArray -> END

        // FField -> START
        virtual auto Field_get_type_fname(FField* p_this) -> FName = 0;
        virtual auto Field_get_ffield_owner(FField* p_this) -> void* = 0;
        // FField -> END

        // FFieldClass -> START
        // Should map to UField in <4.25
        virtual auto FFieldClass_get_fname(FFieldClass* p_this) -> FName = 0;
        // FFieldClass -> END
    };

    class Default : public Base
    {
    private:
        // FUObjectItem -> START
        using UObjectBase = Unreal::UObject;

        struct FUObjectItem
        {
            UObjectBase* object;
            int32_t flags;
            int32_t cluster_root_index;
            int32_t serial_number;

            [[nodiscard]] auto is_unreachable() const -> bool
            {
                return !!(flags & static_cast<int32_t>(EInternalObjectFlags::Unreachable));
            }

            [[nodiscard]] auto is_pending_kill() const
            {
                return !!(flags & static_cast<int32_t>(EInternalObjectFlags::PendingKill));
            }

            auto set_root_set(SetOrUnsetFlag set_or_unset_flag)
            {
                if (set_or_unset_flag == SetOrUnsetFlag::Set)
                {
                    flags = flags | static_cast<int32_t>(EInternalObjectFlags::RootSet);
                }
                else
                {
                    flags = flags & ~static_cast<int32_t>(EInternalObjectFlags::RootSet);
                }
            }

            auto set_gc_keep(SetOrUnsetFlag set_or_unset_flag)
            {
                if (set_or_unset_flag == SetOrUnsetFlag::Set)
                {
                    flags = flags | static_cast<int32_t>(EInternalObjectFlags::GarbageCollectionKeepFlags);
                }
                else
                {
                    flags = flags & ~static_cast<int32_t>(EInternalObjectFlags::GarbageCollectionKeepFlags);
                }
            }
        };

        // Abstraction Layer -> START
        auto FUObjectItem_is_object_unreachable(void* p_this) const -> bool override
        {
            if (!p_this) { return true; }
            FUObjectItem* p_typed_this = static_cast<FUObjectItem*>(p_this);

            if (!p_typed_this->object) { return true; }

            return p_typed_this->is_unreachable();
        }

        auto FUObjectItem_set_object_root_set(void* p_this, SetOrUnsetFlag set_or_unset_flag) -> void override
        {
            if (!p_this) { return; }
            FUObjectItem* typed_this = static_cast<FUObjectItem*>(p_this);

            typed_this->set_root_set(set_or_unset_flag);
        }

        auto FUObjectItem_set_object_gc_keep(void* p_this, SetOrUnsetFlag set_or_unset_flag) -> void override
        {
            if (!p_this) { return; }
            FUObjectItem* typed_this = static_cast<FUObjectItem*>(p_this);

            typed_this->set_gc_keep(set_or_unset_flag);
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

        auto FUObjectItem_is_valid(void* p_this, bool b_even_if_pending_kill) -> bool override
        {
            FUObjectItem* p_typed_this = static_cast<FUObjectItem*>(p_this);

            if (p_typed_this)
            {
                return b_even_if_pending_kill ? !p_typed_this->is_unreachable() : !(p_typed_this->is_unreachable() || p_typed_this->is_pending_kill());
            }
            return false;
        }

        auto FUObjectItem_get_uobject(void* p_this) -> void* override
        {
            FUObjectItem* p_typed_this = static_cast<FUObjectItem*>(p_this);

            if (p_typed_this)
            {
                return p_typed_this->object;
            }
            else
            {
                return nullptr;
            }
        }

        bool FUObjectItem_HasAnyFlags(void* RawThis, EInternalObjectFlags InFlags) override
        {
            FUObjectItem* TypedThis = static_cast<FUObjectItem*>(RawThis);
            return !!(TypedThis->flags & int32(InFlags));
        }
        // Abstraction Layer -> END
        // FUObjectItem -> END

        // GUObjectArray -> START
        struct FChunkedFixedUObjectArray
        {
            enum
            {
                num_elements_per_chunk = 64 * 1024,
            };

            FUObjectItem** objects;
            FUObjectItem* pre_allocated_objects;
            int32_t max_elements;
            int32_t num_elements;
            int32_t max_chunks;
            int32_t num_chunks;

            [[nodiscard]] auto get_object_ptr(int32_t index) const -> FUObjectItem*
            {
                const int32_t chunk_index = index / num_elements_per_chunk;
                const int32_t within_chunk_index = index % num_elements_per_chunk;
                FUObjectItem* chunk = objects[chunk_index];
                return chunk + within_chunk_index;
            }

            [[nodiscard]] auto operator[](int32_t index) const -> FUObjectItem&
            {
                FUObjectItem* item_ptr = get_object_ptr(index);
                return *item_ptr;
            }
        };

        struct FUObjectArray
        {
            using TUObjectArray = FChunkedFixedUObjectArray;

            int32_t obj_first_gc_index;
            int32_t obj_last_non_gc_index;
            int32_t max_objects_not_considered_by_gc;
            bool open_for_disregard_for_gc;
            TUObjectArray obj_objects;
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
        // GUObjectArray -> END
    };
}


#endif //RC_UNREAL_BASE_HPP
