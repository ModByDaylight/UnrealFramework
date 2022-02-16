#ifndef RC_UNREAL_TARRAY_HPP
#define RC_UNREAL_TARRAY_HPP

#include <functional>
#include <stdexcept>
#include <type_traits>
#include <unordered_set>

#include <Constructs/Loop.hpp>
#include <Helpers/Format.hpp>

#include <Unreal/Common.hpp>
#include <Unreal/PrimitiveTypes.hpp>
#include <Unreal/ChooseClass.hpp>
#include <Unreal/ContainersFwd.hpp>
#include <Unreal/ContainerAllocationPolicies.hpp>
#include <Unreal/FMemory.hpp>
#include <Unreal/VersionedContainer/Base.hpp>

#include <Unreal/TypeChecker.hpp>

//#include <Unreal/Property/FArrayProperty.hpp>
//#include <Unreal/Property/FStructProperty.hpp>

namespace RC::Unreal
{
    class XStructData;

    // Redo or remove this, need to think about it
    template<typename ArrayInnerType>
    constexpr bool ArraySizeIsImplicit = !(std::is_same_v<ArrayInnerType, struct FAssetData>);

    auto RC_UE_API get_struct_data_at(void* data_param, size_t index) -> XStructData*;

    /*
     * Problems with TArray:
     *
     * The TArray destructor doesn't call element destructors.
     *
     * You can't create a new TArray in the games memory.
     */
    template<typename InElementType, typename InAllocator>
    class TArray
    {
    public:
        constexpr static inline InternalType internal_type = InternalType::Array;
        using SizeType = typename InAllocator::SizeType;
        using ElementType = InElementType;
        using Allocator = InAllocator;

        typedef typename TChooseClass<
                Allocator::NeedsElementType,
                typename Allocator::template ForElementType<ElementType>,
                typename Allocator::ForAnyElementType
        >::Result ElementAllocatorType;

    protected:
        ElementAllocatorType AllocatorInstance{};
        int32 ArrayNum{};
        int32 ArrayMax{};

    public:
        // Custom constructor for when we already have a TArray in memory, and we'd just like to take it over
        //TArray(ArrayInnerType* data_ptr, int32_t current_size, int32_t capacity) : data(data_ptr), ArrayNum(current_size),
        //                                                                           ArrayMax(capacity) {}

        TArray(InElementType* data_ptr, int32_t current_size, int32_t capacity) : ArrayNum(current_size), ArrayMax(capacity)
        {
            AllocatorInstance.Data = std::bit_cast<FScriptContainerElement*>(data_ptr);
        }

        // Constructor for when we want to allocate a new TArray in the games memory
        TArray() = default;

        ~TArray()
        {
            // TODO: Call destructors
        }

        // Memory related -> START
    public:
        void Reserve(SizeType Number)
        {
            if (Number <= 0) { throw std::runtime_error{"TArray::Reserve called with a Number <= 0"}; }

            if (Number > ArrayMax)
            {
                ResizeTo(Number);
            }
        }

        void ResizeTo(SizeType NewMax)
        {
            if (NewMax)
            {
                NewMax = AllocatorInstance.CalculateSlackReserve(NewMax, sizeof(ElementType));
            }

            if (NewMax != ArrayMax)
            {
                ArrayMax = NewMax;
                AllocatorInstance.ResizeAllocation(ArrayNum, ArrayMax, sizeof(ElementType));
            }
        }

        SizeType AddUninitialized(SizeType Count = 1)
        {
            const SizeType OldNum = ArrayNum;
            if ((ArrayNum += Count) > ArrayMax)
            {
                ResizeGrow(OldNum);
            }
            return OldNum;
        }

        SizeType AddZeroed(SizeType Count = 1)
        {
            const SizeType Index = AddUninitialized(Count);
            FMemory::Memzero((uint8*)AllocatorInstance.GetAllocation() + Index*sizeof(ElementType), Count*sizeof(ElementType));
            return Index;
        }

        void ResizeGrow(SizeType OldNum)
        {
            ArrayMax = AllocatorInstance.CalculateSlackGrow(ArrayNum, ArrayMax, sizeof(ElementType));
            AllocatorInstance.ResizeAllocation(OldNum, ArrayMax, sizeof(ElementType));
        }
        // Memory related -> END

    public:
        auto get_data_ptr() const -> InElementType*
        {
            return std::bit_cast<InElementType*>(AllocatorInstance.GetAllocation());
        }

        auto set_data_ptr(InElementType* new_data_ptr) -> void
        {
            AllocatorInstance.Data = std::bit_cast<FScriptContainerElement*>(new_data_ptr);
        }

        auto get_array_num() const -> int32_t
        {
            return ArrayNum;
        }

        auto set_array_num(int32_t new_array_num) -> void
        {
            ArrayNum = new_array_num;
        }

        auto get_array_max() const -> int32_t
        {
            return ArrayMax;
        }

        auto set_array_max(int32_t new_array_max) -> void
        {
            ArrayMax = new_array_max;
        }

        auto copy_fast(const TArray<InElementType, Allocator>& other) -> void
        {
            AllocatorInstance.Data = other.AllocatorInstance.GetAllocation();
            ArrayNum = other.ArrayNum;
            ArrayMax = other.ArrayMax;
        }

        auto at(size_t index) -> InElementType*
        {
            // Index is zero-based and the stored max is one-based
            // Anything after ArrayMax is uninitialized, but it must succeed still so that operator[] can be used to initialize the element
            if (get_array_max() == 0 || index > get_array_max() - 1)
            {
                throw std::runtime_error{fmt("[TArrayImpl::at] out of range (num elements: %d | requested elem: %d)", (get_array_num() - 1 < 0 ? 0 : get_array_num() - 1), index)};
            }

            // Need to know the type here so that we can calculate the location of each array element
            // TODO: Come up with a better solution. TArray shouldn't need to know about StructProperty or XStructData.
            // I've hardcoded the StructProperty implementation for now but at least it's constexpr.
            if constexpr (std::is_same_v<InElementType, class XStructData>)
            {
                return get_struct_data_at(get_data_ptr(), index);
            }
            else
            {
                //if constexpr (ArraySizeIsImplicit<InElementType>)
                if constexpr (std::is_same_v<InElementType, struct FAssetData>)
                {
                    // FAssetData has a corresponding UScriptStruct that stores the size
                    return reinterpret_cast<InElementType*>(reinterpret_cast<uintptr_t>(AllocatorInstance.GetAllocation()) + (index * InElementType::get_size_in_container()));
                }
                else
                {
                    // This is for trivial types, ElementSize is known by the compiler
                    return std::bit_cast<InElementType*>(&AllocatorInstance.GetAllocation()[index * sizeof(ElementType)]);
                }
            }
        }

        auto operator[](size_t index) -> InElementType&
        {
            return *static_cast<InElementType*>(at(index));
        }

    private:
        template<typename Callable>
        auto for_each_internal(Callable callable) -> void
        {
            for (size_t i = 0; i < get_array_num(); ++i)
            {
                if (callable(at(i), i) == LoopAction::Break) { break; }
            }
        }

    public:
        using TArrayForEachCallableAllParamsImpl = const std::function<LoopAction(InElementType*, size_t)>&;
        auto for_each(TArrayForEachCallableAllParamsImpl callable) -> void
        {
            for_each_internal([&](auto elem, auto index) {
                return callable(elem, index);
            });
        }

        using TArrayForEachCallableParamElemOnlyImpl = const std::function<LoopAction(InElementType*)>&;
        auto for_each(TArrayForEachCallableParamElemOnlyImpl callable) -> void
        {
            for_each_internal([&](auto elem, [[maybe_unused]]auto index) {
                return callable(elem);
            });
        }
    };
}


#endif //RC_UNREAL_TARRAY_HPP
