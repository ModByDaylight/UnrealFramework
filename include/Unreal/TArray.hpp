#ifndef RC_UNREAL_TARRAY_HPP
#define RC_UNREAL_TARRAY_HPP

#include <functional>
#include <stdexcept>
#include <type_traits>
#include <Constructs/Loop.hpp>
#include <Helpers/Format.hpp>
#include <Unreal/Common.hpp>
#include <Unreal/PrimitiveTypes.hpp>
#include <Unreal/TypeTraits.hpp>
#include <Unreal/ChooseClass.hpp>
#include <Unreal/ContainersFwd.hpp>
#include <Unreal/ContainerAllocationPolicies.hpp>
#include <Unreal/FMemory.hpp>
#include <Unreal/UnrealFlags.hpp>
#include <Unreal/TypeChecker.hpp>

namespace RC::Unreal
{
    template<typename InElementType, typename InAllocator>
    class TArray
    {
    public:
        using SizeType = typename InAllocator::SizeType;
        using ElementType = InElementType;
        using Allocator = InAllocator;

        typedef typename TChooseClass<
                Allocator::NeedsElementType,
                typename Allocator::template ForElementType<ElementType>,
                typename Allocator::ForAnyElementType
        >::Result ElementAllocatorType;
    protected:
        ElementAllocatorType AllocatorInstance;
        int32 ArrayNum{};
        int32 ArrayMax{};
    public:
        /**
         * Default TArray constructor. Will not add any elements or allocate any
         * memory until the first element is added
         */
        inline TArray(): ArrayNum(0), ArrayMax(AllocatorInstance.GetInitialCapacity()) {
        }

        /**
         * Initializes TArray with the elements provided in the initializer list
         * @param initializerList elements to add into the array
         */
        inline TArray(std::initializer_list<ElementType> initializerList) {
            CopyFrom_Helper(initializerList.begin(), initializerList.size());
        }

        /**
         * Copies the elements from the provided C array with the following size
         * @param DataPtr pointer to the data to copy
         * @param Size size of the array
         */
        TArray(ElementType* DataPtr, int32_t Size) {
            CopyFrom_Helper(DataPtr, Size);
        }

        /**
         * Copies from the data of the specified TArray with a different allocator
         * @param Other the array to copy the data from
         */
        template<typename InOtherAllocator>
        TArray(const TArray<ElementType, InOtherAllocator>& Other, SizeType ExtraSlack = 0) {
            CopyFrom_Helper(Other.GetData(), Other.Num(), ExtraSlack);
        }

        /** Copies the data from the provided array with the same allocator */
        TArray(const TArray& Other) {
            CopyFrom_Helper(Other.GetData(), Other.Num());
        }

        /**
         * Copies from the data of the specified TArray, potentially with a different allocator
         * @param Other the array to copy the data from
        */
        template<typename InOtherAllocator>
        TArray& operator=(const TArray<ElementType, InOtherAllocator>& Other) {
            CopyFrom_Helper(Other.GetData(), Other.Num());
            return *this;
        }

        /** Copies the data from the array with the same allocator */
        TArray& operator=(const TArray& Other) {
            if (this != &Other) {
                CopyFrom_Helper(Other.GetData(), Other.Num());
            }
            return *this;
        }

        /**
         * Deallocates TArray and calls destructors on all of the elements inside
         */
        ~TArray() {
            if (ArrayNum) {
                DestructItems(GetData(), ArrayNum);
                ArrayNum = 0;
            }
        }
    public:
        /**
         * Empties the array by calling destructors on all of the elements and setting size to zero
         * Array allocation will be resized in accordance to the slack you want to keep
         * @param Slack slack to keep in the array
         */
        void Empty(SizeType Slack = 0) {
            if (ArrayNum) {
                DestructItems(GetData(), ArrayNum);
                ArrayNum = 0;
            }
            ResizeTo(Slack);
        }

        /**
         * Reserves specified number of the elements into the array
         * This will preallocate the memory to contain them, but will not add anything
         * @param Number number of elements to reserve
         */
        void Reserve(SizeType Number) {
            if (Number <= 0) {
                throw std::runtime_error{"TArray::Reserve called with a Number <= 0"};
            }
            if (Number > ArrayMax) {
                ResizeTo(Number);
            }
        }

        /**
         * Adds specified number of elements into the Array, without
         * calling their constructors or zeroing the memory
         *
         * @param Count amount of elements to add
         * @return index to the first element added
         */
        SizeType AddUninitialized(SizeType Count = 1) {
            const SizeType OldNum = ArrayNum;
            if ((ArrayNum += Count) > ArrayMax) {
                ResizeGrow(OldNum);
            }
            return OldNum;
        }

        /**
         * Adds uninitialized element into the array and returns reference to it
         * @return reference to the newly added element
         */
        ElementType& AddUnitialized_GetRef() {
            SizeType Index = AddUninitialized();
            return operator[](Index);
        }

        /**
         * Adds specified number of elements into the array and zero initializes them
         * This will not call element constructors
         *
         * @param Count amount of elements to add
         * @return index of the first added element
         */
        SizeType AddZeroed(SizeType Count = 1) {
            SizeType Index = AddUninitialized(Count);

            ElementType* FirstElementPtr = At(Index);
            FMemory::Memzero(FirstElementPtr, GetElementSize() * Count);
            return Index;
        }

        /**
         * Adds zeroed element into the array and returns reference to it
         * @return reference to the newly added element
         */
        ElementType& AddZeroed_GetRef() {
            SizeType Index = AddZeroed();
            return operator[](Index);
        }

        /**
         * Adds specified number of default constructed elements into the array
         * @param Count amount of elements to add
         * @return index of the first element added
         */
        SizeType AddDefaulted(SizeType Count = 1) {
            SizeType Index = AddUninitialized(Count);

            ElementType* FirstElementPtr = At(Index);
            ConstructItems(FirstElementPtr, Count);
            return Index;
        }

        /**
         * Adds default constructed element into the array and returns reference to it
         * @return reference to the newly added element
         */
        ElementType& AddDefaulted_GetRef() {
            SizeType Index = AddDefaulted();
            return operator[](Index);
        }

    private:
        template <typename OtherSizeType>
        void InsertUninitializedImpl(SizeType Index, OtherSizeType Count) {
            const SizeType OldNum = ArrayNum;
            if ((ArrayNum += Count) > ArrayMax) {
                ResizeGrow(OldNum);
            }
            ElementType* Data = GetData() + Index;
            memmove(Data + Count, Data, sizeof(ElementType) * (OldNum - Index));
        }
    public:
        /**
         * Inserts a raw array of elements at a particular index in the TArray.
         *
         * @param Ptr A pointer to an array of elements to add.
         * @param Count The number of elements to insert from Ptr.
         * @param Index The index to insert the elements at.
         * @return The index of the first element inserted.
         * @see Add, Remove
         */
        SizeType Insert(const ElementType* Ptr, SizeType Count, SizeType Index) {
            InsertUninitializedImpl(Index, Count);
            ConstructItems(GetData() + Index, Ptr, Count);

            return Index;
        }

        /**
         * Inserts a given element into the array at given location.
         *
         * @param Item The element to insert.
         * @param Index Tells where to insert the new elements.
         * @returns Location at which the insert was done.
         * @see Add, Remove
         */
        SizeType Insert(const ElementType& Item, SizeType Index) {
            InsertUninitializedImpl(Index, 1);

            new(GetData() + Index) ElementType(Item);
            return Index;
        }

        /**
         * Inserts a given element into the array at given location.
         *
         * @param Item The element to insert.
         * @param Index Tells where to insert the new element.
         * @return A reference to the newly-inserted element.
         * @see Add, Remove
         */
        ElementType& Insert_GetRef(const ElementType& Item, SizeType Index) {
            InsertUninitializedImpl(Index, 1);

            ElementType* Ptr = GetData() + Index;
            new(Ptr) ElementType(Item);
            return *Ptr;
        }

        /**
         * Recalculates the allocation size according to the number of elements in the array currently for growing
         * @param OldNum old number of elements in the array
         */
        void ResizeGrow(SizeType OldNum) {
            ArrayMax = AllocatorInstance.CalculateSlackGrow(ArrayNum, ArrayMax, GetElementSize());
            AllocatorInstance.ResizeAllocation(OldNum, ArrayMax, GetElementSize());
        }

        /**
        * Recalculates the allocation size according to the number of elements in the array currently for shrinking
        */
        void ResizeShrink() {
            SizeType NewArrayMax = AllocatorInstance.CalculateSlackShrink(ArrayNum, ArrayMax, GetElementSize());
            if (NewArrayMax != ArrayMax) {
                ArrayMax = NewArrayMax;
                AllocatorInstance.ResizeAllocation(ArrayNum, ArrayMax, GetElementSize());
            }
        }

        /**
         * Equality operator.
         * @param OtherArray array to compare
         * @returns true if this array is the same as OtherArray, false otherwise
         */
        template<typename InOtherAllocator>
        bool operator==(const TArray<ElementType, InOtherAllocator>& OtherArray) const {
            SizeType Count = Num();
            return Count == OtherArray.Num() && CompareItems(GetData(), OtherArray.GetData(), Count);
        }

        /**
         * Attempts to find index of the provided element inside of the Array
         * @param Item element to find
         * @return index of the element, or INDEX_NONE if not found
         */
        SizeType Find(const ElementType& Item) const {
            const ElementType* RESTRICT Start = GetData();
            for (const ElementType* RESTRICT Data = Start, *RESTRICT DataEnd = Data + ArrayNum; Data != DataEnd; Data++) {
                if (*Data == Item) {
                    return Data - Start;
                }
            }
            return INDEX_NONE;
        }

        /**
         * Constructs the new element inside of the array
         *
         * @param Args arguments to pass to the element constructor
         * @return index of the inserted element
         */
        template<typename... ArgsType>
        SizeType Emplace(ArgsType&&... Args) {
            const SizeType Index = AddUninitialized(1);
            new (At(Index)) ElementType(std::forward<ArgsType>(Args)...);
            return Index;
        }

        /**
         * Adds the copy of the item into the array
         *
         * @param Item item to add into the array
         * @return index of the newly added item
         */
        SizeType Add(const ElementType& Item) {
            const SizeType Index = AddUninitialized(1);
            new (At(Index)) ElementType(Item);
            return Index;
        }

        /**
         * Checks if the array contains an item equal to the provided one
         * @param Item item to search for
         * @return true if the array contains such an item, false otherwise
         */
        bool Contains(const ElementType& Item) const {
            return Find(Item) != INDEX_NONE;
        }

        template<typename CountType>
        void RemoveAtSwap(SizeType Index, CountType Count, bool bAllowShrinking = true) {
            static_assert(!TAreTypesEqual<CountType, bool>::Value, "TArray::RemoveAtSwap: unexpected bool passed as the Count argument");
            RemoveAtSwapImpl(Index, Count, bAllowShrinking);
        }

        SizeType RemoveSingleSwap(const ElementType& Item, bool bAllowShrinking = true) {
            SizeType Index = Find(Item);
            if (Index == INDEX_NONE) {
                return 0;
            }
            RemoveAtSwap(Index, 1, bAllowShrinking);
            return 1;
        }

        ElementType* GetData() {
            return (ElementType*) AllocatorInstance.GetAllocation();
        }

        const ElementType* GetData() const {
            return (const ElementType*) AllocatorInstance.GetAllocation();
        }

        /**
         * Returns the number of elements inside of the array currently
         * @return number of the elements
         */
        SizeType Num() const {
            return ArrayNum;
        }

        /**
         * Returns the maximum amount of elements that the current array allocation can hold
         * @return maximum number of elements
         */
        SizeType Max() const {
            return ArrayMax;
        }

        /**
         * Returns the pointer to the element at the specified index
         * @param Index index of the element to retrieve, throws exception if out of bounds
         * @return element at the index
         */
        ElementType* At(size_t Index) {
            if (Index >= ArrayMax) {
                throw std::runtime_error{fmt("[TArray::At] out of range (num elements: %d | requested elem: %d)", ArrayNum, Index)};
            }
            return &((ElementType*) AllocatorInstance.GetAllocation())[Index];
        }

        /**
         * Returns the pointer to the element at the specified index
         * @param Index index of the element to retrieve, throws exception if out of bounds
         * @return element at the index
         */
        const ElementType* At(size_t Index) const {
            if (Index >= ArrayMax) {
                throw std::runtime_error{fmt("[TArray::At] out of range (num elements: %d | requested elem: %d)", ArrayNum, Index)};
            }
            return &((const ElementType*) AllocatorInstance.GetAllocation())[Index];
        }

        /**
         * Returns the mutable reference to the array element at the specified index
         * @return array element at the index
         */
        ElementType& operator[](SizeType Index) {
            return *At(Index);
        }

        /**
         * Returns the immutable reference to the array element at the specified index
         * @return array element at index
         */
        const ElementType& operator[](SizeType Index) const {
            return *At(Index);
        }

        void ForEach(const std::function<LoopAction(ElementType*, size_t)>& Callable) {
            for (SizeType i = 0; i < Num(); i++) {
                if (Callable(At(i), i) == LoopAction::Break) {
                    break;
                }
            }
        }

        void ForEach(const std::function<LoopAction(ElementType*)>& Callable) {
            for (SizeType i = 0; i < Num(); i++) {
                if (Callable(At(i)) == LoopAction::Break) {
                    break;
                }
            }
        }

        void ForEach(const std::function<LoopAction(ElementType&)>& Callable) {
            for (SizeType i = 0; i < Num(); i++) {
                if (Callable(*At(i)) == LoopAction::Break) {
                    break;
                }
            }
        }

        FORCEINLINE ElementType* begin() { return GetData(); }
        FORCEINLINE const ElementType* begin() const { return GetData(); }
        FORCEINLINE ElementType* end() { return GetData() + Num(); }
        FORCEINLINE const ElementType* end() const { return GetData() + Num(); }
        
        /** Returns the size of a single array element */
        FORCEINLINE static SIZE_T GetElementSize() {
            return sizeof(ElementType);
        }
    private:
        void DestructItems(ElementType* Data, SizeType Count) {
            if constexpr(!TIsTriviallyDestructible<ElementType>::Value) {
                //Only need to call destructors on non-trivially-destructible elements
                for (SizeType i = 0; i < Count; i++) {
                    Data[i].~ElementType();
                }
            }
        }

        void ConstructItems(ElementType* Data, SizeType Count) {
            if constexpr(!TIsTriviallyConstructable<ElementType>::Value) {
                //Only need to call default constructs on non-trivially-constructable elements
                for (SizeType i = 0; i < Count; i++) {
                    new(&Data[i]) ElementType();
                }
            } else {
                //On trivially constructable types we can just zero their memory out
                FMemory::Memzero(Data, GetElementSize() * Count);
            }
        }

        void CopyItems(ElementType* Dest, const ElementType* Src, SizeType Count) {
            if constexpr(!TIsTriviallyCopyable<ElementType>::Value) {
                //If the elements are not trivially copyable, we need to manually call the copy constructors on them
                for (SizeType i = 0; i < Count; i++) {
                    new(&Dest[i]) ElementType(Src[i]);
                }
            } else {
                //On trivially copyable elements we can go with a plain Memcpy
                FMemory::Memcpy(Dest, Src, GetElementSize() * Count);
            }
        }

        bool CompareItems(const ElementType* A, const ElementType* B, SizeType Count) {
            if constexpr(!TIsBytewiseComparable<ElementType>::Value) {
                //If the elements are not bytewise comparable, we need to manually compare them
                for (SizeType i = 0; i < Count; i++) {
                    if (A[i] != B[i]) {
                        return false;
                    }
                }
                return true;
            } else {
                //Otherwise, we can compare the elements using fast Memcmp
                return memcmp(A, B, GetElementSize() * Count);
            }
        }

        void RemoveAtSwapImpl(SizeType Index, SizeType Count = 1, bool bAllowShrinking = true) {
            if (Count) {
                DestructItems(&GetData()[Index], Count);

                // Replace the elements in the hole created by the removal with elements from the end of the array,
                // so the range of indices used by the array is contiguous.
                const SizeType NumElementsInHole = Count;
                const SizeType NumElementsAfterHole = ArrayNum - (Index + Count);
                const SizeType NumElementsToMoveIntoHole = std::min(NumElementsInHole, NumElementsAfterHole);

                if (NumElementsToMoveIntoHole) {
                    FMemory::Memcpy(&GetData()[Index],
                                    &GetData()[ArrayNum - NumElementsToMoveIntoHole],
                                    NumElementsToMoveIntoHole * GetElementSize());
                }
                ArrayNum -= Count;
                if (bAllowShrinking) {
                    ResizeShrink();
                }
            }
        }

        /**
         * Resizes the allocation of the array to the provided size
         * @param NewMax new maximum number of elements requested
         */
        void ResizeTo(SizeType NewMax) {
            if (NewMax) {
                NewMax = AllocatorInstance.CalculateSlackReserve(NewMax, GetElementSize());
            }
            if (NewMax != ArrayMax) {
                ArrayMax = NewMax;
                AllocatorInstance.ResizeAllocation(ArrayNum, ArrayMax, GetElementSize());
            }
        }

        /**
         * Copies elements from the provided array
         * Will reserve enough space for the elements and then copy them
         * using their copy constructs directly
         */
        void CopyFrom_Helper(const InElementType* Data, SizeType Count, SizeType ExtraSlack = 0) {
            //Destruct other elements that we hold currently, but keep the slack
            if(ArrayNum) {
                DestructItems((ElementType*) AllocatorInstance.GetAllocation(), ArrayNum);
                ArrayNum = 0;
            }
            //Copy the elements from the provided array
            Reserve(Count + ExtraSlack);
            CopyItems((ElementType*) AllocatorInstance.GetAllocation(), Data, Count);
            ArrayNum = Count;
        }
    };
}


#endif //RC_UNREAL_TARRAY_HPP
