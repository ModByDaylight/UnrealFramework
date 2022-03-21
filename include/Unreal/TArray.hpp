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

namespace RC::Unreal
{
    class XStructData;

    // Redo or remove this, need to think about it
    template<typename ArrayInnerType>
    constexpr bool ArraySizeIsImplicit = !(std::is_same_v<ArrayInnerType, struct FAssetData>);

    auto RC_UE_API GetStructDataAt(void* data_param, size_t index) -> XStructData*;

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

        TArray([[maybe_unused]]InElementType* data_ptr, int32_t current_size, int32_t capacity) : ArrayNum(current_size), ArrayMax(capacity)
        {
            //AllocatorInstance.Data = std::bit_cast<FScriptContainerElement*>(data_ptr);
        }

        // Constructor for when we want to allocate a new TArray in the games memory
        // The TArray itself is on the stack (or local heap) but the data pointer will be in the games own heap
        TArray() = default;

        // Problems with this copying:
        // The underlying data might get deallocated from the original TArray going out of scope
        // At that point, any access to the copied TArray is undefined
        // For now, delete the copy constructor & copy assignment operator to disallow copying
        TArray(const TArray& Other)
        {
            CopyFrom_Helper(Other);
        }

        TArray& operator=(const TArray& Other)
        {
            if (this != &Other)
            {
                CopyFrom_Helper(Other);
            }

            return *this;
        }

        TArray& operator=(TArray& Other)
        {
            if (this != &Other)
            {
                CopyFrom_Helper(Other);
            }

            return *this;
        }

        ~TArray()
        {
            // TODO: Call destructors
        }

        // Memory related -> START
    private:
        SizeType GetElementSize()
        {
            if constexpr (std::is_same_v<ElementType, struct FAssetData>)
            {
                // Structs where the size is unknown to the compiler
                // This happens when a struct has had layout changes and now has different sizes in different engine versions
                return ElementType::StaticSize();
            }
            else
            {
                // Structs where the size is known to the compiler
                return sizeof(ElementType);
            }
        }

        SizeType GetElementSize() const
        {
            if constexpr (std::is_same_v<ElementType, struct FAssetData>)
            {
                // Structs where the size is unknown to the compiler
                // This happens when a struct has had layout changes and now has different sizes in different engine versions
                return ElementType::StaticSize();
            }
            else
            {
                // Structs where the size is known to the compiler
                return sizeof(ElementType);
            }
        }

        void CopyFrom_Helper(const TArray& Other)
        {
            if constexpr (IsTInlineAllocator<InAllocator>)
            {
                std::memcpy(&AllocatorInstance, &Other.AllocatorInstance, Other.AllocatorInstance.GetInitialCapacity() * GetElementSize());

                if (auto AllocatedSize = Other.AllocatorInstance.GetAllocatedSize(Other.ArrayNum, Other.GetElementSize()); AllocatedSize > 0)
                {
                    AllocatorInstance.SecondaryData.Data = static_cast<FScriptContainerElement*>(FMemory::Malloc(Other.ArrayMax * GetElementSize()));
                    std::memcpy(AllocatorInstance.SecondaryData.Data, Other.AllocatorInstance.SecondaryData.Data, AllocatedSize);
                }
                else
                {
                    AllocatorInstance.SecondaryData.Data = nullptr;
                }

                ArrayNum = Other.ArrayNum;
                ArrayMax = Other.ArrayMax;
            }
            else
            {
                if (Other.AllocatorInstance.GetAllocation())
                {
                    AllocatorInstance.Data = static_cast<FScriptContainerElement*>(FMemory::Malloc(Other.ArrayMax * GetElementSize()));
                    std::memcpy(AllocatorInstance.Data, Other.AllocatorInstance.Data, Other.ArrayNum * GetElementSize());
                    ArrayNum = Other.ArrayNum;
                    ArrayMax = Other.ArrayMax;
                }
                else
                {
                    AllocatorInstance.Data = nullptr;
                    ArrayNum = 0;
                    ArrayMax = 0;
                }
            }
        }

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
                NewMax = AllocatorInstance.CalculateSlackReserve(NewMax, GetElementSize());
            }

            if (NewMax != ArrayMax)
            {
                ArrayMax = NewMax;
                AllocatorInstance.ResizeAllocation(ArrayNum, ArrayMax, GetElementSize());
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
            FMemory::Memzero((uint8*)AllocatorInstance.GetAllocation() + Index*GetElementSize(), Count*GetElementSize());
            return Index;
        }

        void ResizeGrow(SizeType OldNum)
        {
            ArrayMax = AllocatorInstance.CalculateSlackGrow(ArrayNum, ArrayMax, GetElementSize());
            AllocatorInstance.ResizeAllocation(OldNum, ArrayMax, GetElementSize());
        }
        // Memory related -> END

    public:
        auto GetDataPtr() const -> InElementType*
        {
            return std::bit_cast<InElementType*>(AllocatorInstance.GetAllocation());
        }

        auto SetDataPtr(InElementType* new_data_ptr) -> void
        {
            if constexpr (IsTInlineAllocator<InAllocator>)
            {
                static_assert(false, "TArray::set_data_ptr cannot be used with TInlineAllocator");
            }
            else
            {
                AllocatorInstance.Data = std::bit_cast<FScriptContainerElement*>(new_data_ptr);
            }
        }

        SizeType Num() const
        {
            return ArrayNum;
        }

        auto SetNum(int32_t new_array_num) -> void
        {
            ArrayNum = new_array_num;
        }

        SizeType Max() const
        {
            return ArrayMax;
        }

        auto SetMax(int32_t new_array_max) -> void
        {
            ArrayMax = new_array_max;
        }

        // Temporary function
        // Transfers ownership of another data ptr to this instance of TArray
        // The 'other' TArray becomes zeroed
        auto CopyFast(TArray<InElementType, Allocator>& other) -> void
        {
            if constexpr (IsTInlineAllocator<InAllocator>)
            {
                static_assert(false, "TArray::copy_fast cannot be used with TInlineAllocator");
            }

            AllocatorInstance.Data = other.AllocatorInstance.GetAllocation();
            ArrayNum = other.ArrayNum;
            ArrayMax = other.ArrayMax;

            // Hack to prevent deallocation when 'other' goes out of scope
            other.AllocatorInstance.Data = nullptr;
            other.ArrayNum = 0;
            other.ArrayMax = 0;
        }

        auto At(size_t Index) -> InElementType*
        {
            // Index is zero-based and the stored max is one-based
            // Anything after ArrayMax is uninitialized, but it must succeed still so that operator[] can be used to initialize the element
            if (Max() == 0 || Index > Max() - 1)
            {
                throw std::runtime_error{fmt("[TArray::at] out of range (num elements: %d | requested elem: %d)", (Num() - 1 < 0 ? 0 : Num() - 1), Index)};
            }

            // Need to know the type here so that we can calculate the location of each array element
            // TODO: Come up with a better solution. TArray shouldn't need to know about StructProperty or XStructData.
            // I've hardcoded the StructProperty implementation for now but at least it's constexpr.
            if constexpr (std::is_same_v<InElementType, class XStructData>)
            {
                return GetStructDataAt(GetDataPtr(), Index);
            }
            else
            {
                return std::bit_cast<InElementType*>(&AllocatorInstance.GetAllocation()[Index * GetElementSize()]);
            }
        }

        auto operator[](size_t Index) -> InElementType&
        {
            return *static_cast<InElementType*>(At(Index));
        }

    private:
        template<typename Callable>
        auto ForEachInternal(Callable callable) -> void
        {
            for (size_t i = 0; i < Num(); ++i)
            {
                if (callable(At(i), i) == LoopAction::Break) { break; }
            }
        }

    public:
        using TArrayForEachCallableAllParamsImpl = const std::function<LoopAction(InElementType*, size_t)>&;
        auto ForEach(TArrayForEachCallableAllParamsImpl Callable) -> void
        {
            ForEachInternal([&](auto Elem, auto Index) {
                return Callable(Elem, Index);
            });
        }

        using TArrayForEachCallableParamElemOnlyImpl = const std::function<LoopAction(InElementType*)>&;
        auto ForEach(TArrayForEachCallableParamElemOnlyImpl Callable) -> void
        {
            ForEachInternal([&](auto Elem, [[maybe_unused]]auto Index) {
                return Callable(Elem);
            });
        }
    };
}


#endif //RC_UNREAL_TARRAY_HPP
