#ifndef UE4SS_TARRAY_H
#define UE4SS_TARRAY_H

#include <Unreal/Container/ContainerAllocationPolicies.h>
#include "MemoryOps.h"

#define INDEX_NONE -1

namespace RC::Unreal {

    /**
     * Generic iterator which can operate on types that expose the following:
     * - A type called ElementType representing the contained type.
     * - A method SizeType Num() const that returns the number of items in the container.
     * - A method bool IsValidIndex(SizeType index) which returns whether a given index is valid in the container.
     * - A method T& operator\[\](SizeType index) which returns a reference to a contained object by index.
     * - A method void RemoveAt(SizeType index) which removes the element at index
     */
    template< typename ContainerType, typename ElementType, typename SizeType>
    class TIndexedContainerIterator
    {
    public:
        TIndexedContainerIterator(ContainerType& InContainer, SizeType StartIndex = 0)
                : Container(InContainer)
                , Index    (StartIndex)
        {
        }

        /** Advances iterator to the next element in the container. */
        TIndexedContainerIterator& operator++()
        {
            ++Index;
            return *this;
        }
        TIndexedContainerIterator operator++(int)
        {
            TIndexedContainerIterator Tmp(*this);
            ++Index;
            return Tmp;
        }

        /** Moves iterator to the previous element in the container. */
        TIndexedContainerIterator& operator--()
        {
            --Index;
            return *this;
        }
        TIndexedContainerIterator operator--(int)
        {
            TIndexedContainerIterator Tmp(*this);
            --Index;
            return Tmp;
        }

        /** iterator arithmetic support */
        TIndexedContainerIterator& operator+=(SizeType Offset)
        {
            Index += Offset;
            return *this;
        }

        TIndexedContainerIterator operator+(SizeType Offset) const
        {
            TIndexedContainerIterator Tmp(*this);
            return Tmp += Offset;
        }

        TIndexedContainerIterator& operator-=(SizeType Offset)
        {
            return *this += -Offset;
        }

        TIndexedContainerIterator operator-(SizeType Offset) const
        {
            TIndexedContainerIterator Tmp(*this);
            return Tmp -= Offset;
        }

        ElementType& operator* () const
        {
            return Container[ Index ];
        }

        ElementType* operator->() const
        {
            return &Container[ Index ];
        }

        /** conversion to "bool" returning true if the iterator has not reached the last element. */
        FORCEINLINE explicit operator bool() const
        {
            return Container.IsValidIndex(Index);
        }

        /** Returns an index to the current element. */
        SizeType GetIndex() const
        {
            return Index;
        }

        /** Resets the iterator to the first element. */
        void Reset()
        {
            Index = 0;
        }

        /** Sets iterator to the last element. */
        void SetToEnd()
        {
            Index = Container.Num();
        }

        /** Removes current element in array. This invalidates the current iterator value and it must be incremented */
        void RemoveCurrent()
        {
            Container.RemoveAt(Index);
            Index--;
        }

        FORCEINLINE friend bool operator==(const TIndexedContainerIterator& Lhs, const TIndexedContainerIterator& Rhs) {
            return &Lhs.Container == &Rhs.Container && Lhs.Index == Rhs.Index;
        }

        FORCEINLINE friend bool operator!=(const TIndexedContainerIterator& Lhs, const TIndexedContainerIterator& Rhs) {
            return &Lhs.Container != &Rhs.Container || Lhs.Index != Rhs.Index;
        }
    private:

        ContainerType& Container;
        SizeType      Index;
    };


    /** operator + */
    template <typename ContainerType, typename ElementType, typename SizeType>
    FORCEINLINE TIndexedContainerIterator<ContainerType, ElementType, SizeType> operator+(SizeType Offset, TIndexedContainerIterator<ContainerType, ElementType, SizeType> RHS)
    {
        return RHS + Offset;
    }

    /**
     * Templated dynamic array
     *
     * A dynamically sized array of typed elements.  Makes the assumption that your elements are relocate-able; 
     * i.e. that they can be transparently moved to new memory without a copy constructor.  The main implication 
     * is that pointers to elements in the TArray may be invalidated by adding or removing other elements to the array. 
     * Removal of elements is O(N) and invalidates the indices of subsequent elements.
     *
     * Caution: as noted below some methods are not safe for element types that require constructors.
     *
     **/
    template<typename InElementType, typename InAllocator = FDefaultAllocator>
    class TArray
    {
        template <typename OtherInElementType, typename OtherAllocator>
        friend class TArray;

    public:
        typedef typename InAllocator::SizeType SizeType;
        typedef InElementType ElementType;
        typedef InAllocator   Allocator;

        typedef typename std::conditional<
                Allocator::NeedsElementType,
                typename Allocator::template ForElementType<ElementType>,
                typename Allocator::ForAnyElementType
        >::type ElementAllocatorType;

        static_assert(std::is_signed_v<SizeType>, "TArray only supports signed index types");

        FORCEINLINE TArray()
                : ArrayNum(0)
                , ArrayMax(AllocatorInstance.GetInitialCapacity())
        {}

        FORCEINLINE TArray(const ElementType* Ptr, SizeType Count)
        {
            CopyToEmpty(Ptr, Count, 0, 0);
        }

        TArray(std::initializer_list<InElementType> InitList)
        {
            CopyToEmpty(InitList.begin(), (SizeType)InitList.size(), 0, 0);
        }

        template <typename OtherElementType, typename OtherAllocator>
        FORCEINLINE explicit TArray(const TArray<OtherElementType, OtherAllocator>& Other)
        {
            CopyToEmpty(Other.GetData(), Other.Num(), 0, 0);
        }

        FORCEINLINE TArray(const TArray& Other)
        {
            CopyToEmpty(Other.GetData(), Other.Num(), 0, 0);
        }

        FORCEINLINE TArray(const TArray& Other, SizeType ExtraSlack)
        {
            CopyToEmpty(Other.GetData(), Other.Num(), 0, ExtraSlack);
        }

        TArray& operator=(std::initializer_list<InElementType> InitList)
        {
            DestructItems(GetData(), ArrayNum);
            CopyToEmpty(InitList.begin(), (SizeType)InitList.size(), ArrayMax, 0);
            return *this;
        }

        template<typename OtherAllocator>
        TArray& operator=(const TArray<ElementType, OtherAllocator>& Other)
        {
            DestructItems(GetData(), ArrayNum);
            CopyToEmpty(Other.GetData(), Other.Num(), ArrayMax, 0);
            return *this;
        }

        TArray& operator=(const TArray& Other)
        {
            if (this != &Other)
            {
                DestructItems(GetData(), ArrayNum);
                CopyToEmpty(Other.GetData(), Other.Num(), ArrayMax, 0);
            }
            return *this;
        }
    private:
        template <typename FromArrayType, typename ToArrayType>
        static FORCEINLINE void MoveOrCopy(ToArrayType& ToArray, FromArrayType& FromArray, SizeType PrevMax)
        {
            ToArray.CopyToEmpty(FromArray.GetData(), FromArray.Num(), PrevMax, 0);
        }

        template <typename FromArrayType, typename ToArrayType>
        static FORCEINLINE void MoveOrCopyWithSlack(ToArrayType& ToArray, FromArrayType& FromArray, SizeType PrevMax, SizeType ExtraSlack)
        {
            ToArray.CopyToEmpty(FromArray.GetData(), FromArray.Num(), PrevMax, ExtraSlack);
        }
    public:
        FORCEINLINE TArray(TArray&& Other)
        {
            MoveOrCopy(*this, Other, 0);
        }

        template <typename OtherElementType, typename OtherAllocator>
        FORCEINLINE explicit TArray(TArray<OtherElementType, OtherAllocator>&& Other)
        {
            MoveOrCopy(*this, Other, 0);
        }

        template <typename OtherElementType>
        TArray(TArray<OtherElementType, Allocator>&& Other, SizeType ExtraSlack)
        {
            MoveOrCopyWithSlack(*this, Other, 0, ExtraSlack);
        }

        TArray& operator=(TArray&& Other)
        {
            if (this != &Other)
            {
                DestructItems(GetData(), ArrayNum);
                MoveOrCopy(*this, Other, ArrayMax);
            }
            return *this;
        }

        ~TArray()
        {
            DestructItems(GetData(), ArrayNum);
        }

        FORCEINLINE ElementType* GetData()
        {
            return (ElementType*)AllocatorInstance.GetAllocation();
        }

        FORCEINLINE const ElementType* GetData() const
        {
            return (const ElementType*)AllocatorInstance.GetAllocation();
        }

        FORCEINLINE uint32_t GetTypeSize() const
        {
            return sizeof(ElementType);
        }

        FORCEINLINE size_t GetAllocatedSize() const
        {
            return AllocatorInstance.GetAllocatedSize(ArrayMax, sizeof(ElementType));
        }

        FORCEINLINE SizeType GetSlack() const
        {
            return ArrayMax - ArrayNum;
        }

        FORCEINLINE void CheckInvariants() const
        {
            if (!((ArrayNum >= 0) & (ArrayMax >= ArrayNum)))
            {
                throw std::runtime_error("TArray invariants check failed");
            }
        }

        FORCEINLINE void RangeCheck(SizeType Index) const
        {
            CheckInvariants();

            if (Allocator::RequireRangeCheck)
            {
                if (!((Index >= 0) & (Index < ArrayNum)))
                {
                    throw std::runtime_error("Array index out of bounds");
                }
            }
        }

        FORCEINLINE bool IsValidIndex(SizeType Index) const
        {
            return Index >= 0 && Index < ArrayNum;
        }

        FORCEINLINE SizeType Num() const
        {
            return ArrayNum;
        }

        FORCEINLINE SizeType Max() const
        {
            return ArrayMax;
        }

        FORCEINLINE ElementType& operator[](SizeType Index)
        {
            RangeCheck(Index);
            return GetData()[Index];
        }

        FORCEINLINE const ElementType& operator[](SizeType Index) const
        {
            RangeCheck(Index);
            return GetData()[Index];
        }

        FORCEINLINE ElementType& Last(SizeType IndexFromTheEnd = 0)
        {
            RangeCheck(ArrayNum - IndexFromTheEnd - 1);
            return GetData()[ArrayNum - IndexFromTheEnd - 1];
        }

        FORCEINLINE const ElementType& Last(SizeType IndexFromTheEnd = 0) const
        {
            RangeCheck(ArrayNum - IndexFromTheEnd - 1);
            return GetData()[ArrayNum - IndexFromTheEnd - 1];
        }

        FORCEINLINE void Shrink()
        {
            CheckInvariants();
            if (ArrayMax != ArrayNum)
            {
                ResizeTo(ArrayNum);
            }
        }

        FORCEINLINE bool Find(const ElementType& Item, SizeType& Index) const
        {
            Index = this->Find(Item);
            return Index != INDEX_NONE;
        }

        SizeType Find(const ElementType& Item) const
        {
            const ElementType* RESTRICT Start = GetData();
            for (const ElementType*RESTRICT Data = Start, *RESTRICT DataEnd = Data + ArrayNum; Data != DataEnd; ++Data)
            {
                if (*Data == Item)
                {
                    return static_cast<SizeType>(Data - Start);
                }
            }
            return INDEX_NONE;
        }

        FORCEINLINE bool FindLast(const ElementType& Item, SizeType& Index) const
        {
            Index = this->FindLast(Item);
            return Index != INDEX_NONE;
        }

        SizeType FindLast(const ElementType& Item) const
        {
            for (const ElementType* RESTRICT Start = GetData(), *RESTRICT Data = Start + ArrayNum; Data != Start; )
            {
                --Data;
                if (*Data == Item)
                {
                    return static_cast<SizeType>(Data - Start);
                }
            }
            return INDEX_NONE;
        }

        template <typename Predicate>
        SizeType FindLastByPredicate(Predicate Pred, SizeType Count) const
        {
            check(Count >= 0 && Count <= this->Num());
            for (const ElementType* RESTRICT Start = GetData(), *RESTRICT Data = Start + Count; Data != Start; )
            {
                --Data;
                if (Pred(*Data))
                {
                    return static_cast<SizeType>(Data - Start);
                }
            }
            return INDEX_NONE;
        }

        template <typename Predicate>
        FORCEINLINE SizeType FindLastByPredicate(Predicate Pred) const
        {
            return FindLastByPredicate(Pred, ArrayNum);
        }

        template <typename KeyType>
        SizeType IndexOfByKey(const KeyType& Key) const
        {
            const ElementType* RESTRICT Start = GetData();
            for (const ElementType* RESTRICT Data = Start, *RESTRICT DataEnd = Start + ArrayNum; Data != DataEnd; ++Data)
            {
                if (*Data == Key)
                {
                    return static_cast<SizeType>(Data - Start);
                }
            }
            return INDEX_NONE;
        }

        template <typename Predicate>
        SizeType IndexOfByPredicate(Predicate Pred) const
        {
            const ElementType* RESTRICT Start = GetData();
            for (const ElementType* RESTRICT Data = Start, *RESTRICT DataEnd = Start + ArrayNum; Data != DataEnd; ++Data)
            {
                if (Pred(*Data))
                {
                    return static_cast<SizeType>(Data - Start);
                }
            }
            return INDEX_NONE;
        }

        template <typename KeyType>
        FORCEINLINE const ElementType* FindByKey(const KeyType& Key) const
        {
            return const_cast<TArray*>(this)->FindByKey(Key);
        }

        template <typename KeyType>
        ElementType* FindByKey(const KeyType& Key)
        {
            for (ElementType* RESTRICT Data = GetData(), *RESTRICT DataEnd = Data + ArrayNum; Data != DataEnd; ++Data)
            {
                if (*Data == Key)
                {
                    return Data;
                }
            }

            return nullptr;
        }

        template <typename Predicate>
        FORCEINLINE const ElementType* FindByPredicate(Predicate Pred) const
        {
            return const_cast<TArray*>(this)->FindByPredicate(Pred);
        }

        template <typename Predicate>
        ElementType* FindByPredicate(Predicate Pred)
        {
            for (ElementType* RESTRICT Data = GetData(), *RESTRICT DataEnd = Data + ArrayNum; Data != DataEnd; ++Data)
            {
                if (Pred(*Data))
                {
                    return Data;
                }
            }

            return nullptr;
        }

        template <typename Predicate>
        TArray<ElementType> FilterByPredicate(Predicate Pred) const
        {
            TArray<ElementType> FilterResults;
            for (const ElementType* RESTRICT Data = GetData(), *RESTRICT DataEnd = Data + ArrayNum; Data != DataEnd; ++Data)
            {
                if (Pred(*Data))
                {
                    FilterResults.Add(*Data);
                }
            }
            return FilterResults;
        }

        template <typename ComparisonType>
        bool Contains(const ComparisonType& Item) const
        {
            for (const ElementType* RESTRICT Data = GetData(), *RESTRICT DataEnd = Data + ArrayNum; Data != DataEnd; ++Data)
            {
                if (*Data == Item)
                {
                    return true;
                }
            }
            return false;
        }

        template <typename Predicate>
        FORCEINLINE bool ContainsByPredicate(Predicate Pred) const
        {
            return FindByPredicate(Pred) != nullptr;
        }

        bool operator==(const TArray& OtherArray) const
        {
            SizeType Count = Num();

            return Count == OtherArray.Num() && CompareItems(GetData(), OtherArray.GetData(), Count);
        }

        FORCEINLINE bool operator!=(const TArray& OtherArray) const
        {
            return !(*this == OtherArray);
        }

        /**
         * Adds a given number of uninitialized elements into the array.
         *
         * Caution, AddUninitialized() will create elements without calling
         * the constructor and this is not appropriate for element types that
         * require a constructor to function properly.
         *
         * @param Count Number of elements to add.
         * @returns Number of elements in array before addition.
         */
        FORCEINLINE SizeType AddUninitialized(SizeType Count = 1)
        {
            CheckInvariants();

            const SizeType OldNum = ArrayNum;
            if ((ArrayNum += Count) > ArrayMax)
            {
                ResizeGrow(OldNum);
            }
            return OldNum;
        }

    private:
        template <typename OtherSizeType>
        void InsertUninitializedImpl(SizeType Index, OtherSizeType Count)
        {
            CheckInvariants();
            checkSlow((Count >= 0) & (Index >= 0) & (Index <= ArrayNum));

            SizeType NewNum = Count;
            if ((OtherSizeType)NewNum != Count)
            {
                throw std::runtime_error("Invalid number of elements to add to this array type");
            }
            const SizeType OldNum = ArrayNum;
            if ((ArrayNum += Count) > ArrayMax)
            {
                ResizeGrow(OldNum);
            }
            ElementType* Data = GetData() + Index;
            RelocateConstructItems<ElementType>(Data + Count, Data, OldNum - Index);
        }
    public:
        FORCEINLINE void InsertUninitialized(SizeType Index, SizeType Count = 1)
        {
            InsertUninitializedImpl(Index, Count);
        }

        void InsertZeroed(SizeType Index, SizeType Count = 1)
        {
            InsertUninitializedImpl(Index, Count);
            FMemory::memzero(GetData() + Index, Count * sizeof(ElementType));
        }

        ElementType& InsertZeroed_GetRef(SizeType Index)
        {
            InsertUninitializedImpl(Index, 1);
            ElementType* Ptr = GetData() + Index;
            FMemory::memzero(Ptr, sizeof(ElementType));
            return *Ptr;
        }

        void InsertDefaulted(SizeType Index, SizeType Count = 1)
        {
            InsertUninitializedImpl(Index, Count);
            DefaultConstructItems<ElementType>(GetData() + Index, Count);
        }

        ElementType& InsertDefaulted_GetRef(SizeType Index)
        {
            InsertUninitializedImpl(Index, 1);
            ElementType* Ptr = GetData() + Index;
            DefaultConstructItems<ElementType>(Ptr, 1);
            return *Ptr;
        }

        SizeType Insert(std::initializer_list<ElementType> InitList, const SizeType InIndex)
        {
            SizeType NumNewElements = (SizeType)InitList.size();

            InsertUninitializedImpl(InIndex, NumNewElements);
            ConstructItems<ElementType>(GetData() + InIndex, InitList.begin(), NumNewElements);

            return InIndex;
        }

        template <typename OtherAllocator>
        SizeType Insert(const TArray<ElementType, OtherAllocator>& Items, const SizeType InIndex)
        {
            auto NumNewElements = Items.Num();

            InsertUninitializedImpl(InIndex, NumNewElements);
            ConstructItems<ElementType>(GetData() + InIndex, Items.GetData(), NumNewElements);

            return InIndex;
        }

        template <typename OtherAllocator>
        SizeType Insert(TArray<ElementType, OtherAllocator>&& Items, const SizeType InIndex)
        {
            auto NumNewElements = Items.Num();

            InsertUninitializedImpl(InIndex, NumNewElements);
            RelocateConstructItems<ElementType>(GetData() + InIndex, Items.GetData(), NumNewElements);
            Items.ArrayNum = 0;

            return InIndex;
        }

        SizeType Insert(const ElementType* Ptr, SizeType Count, SizeType Index)
        {
            check(Ptr != nullptr);

            InsertUninitializedImpl(Index, Count);
            ConstructItems<ElementType>(GetData() + Index, Ptr, Count);

            return Index;
        }

        SizeType Insert(ElementType&& Item, SizeType Index)
        {
            InsertUninitializedImpl(Index, 1);
            new(GetData() + Index) ElementType(MoveTempIfPossible(Item));
            return Index;
        }

        SizeType Insert(const ElementType& Item, SizeType Index)
        {
            InsertUninitializedImpl(Index, 1);
            new(GetData() + Index) ElementType(Item);
            return Index;
        }

        ElementType& Insert_GetRef(ElementType&& Item, SizeType Index)
        {
            InsertUninitializedImpl(Index, 1);
            ElementType* Ptr = GetData() + Index;
            new(Ptr) ElementType(MoveTempIfPossible(Item));
            return *Ptr;
        }

        ElementType& Insert_GetRef(const ElementType& Item, SizeType Index)
        {
            InsertUninitializedImpl(Index, 1);
            ElementType* Ptr = GetData() + Index;
            new(Ptr) ElementType(Item);
            return *Ptr;
        }

    private:
        void RemoveAtImpl(SizeType Index, SizeType Count, bool bAllowShrinking)
        {
            if (Count)
            {
                CheckInvariants();
                checkSlow((Count >= 0) & (Index >= 0) & (Index + Count <= ArrayNum));

                DestructItems(GetData() + Index, Count);

                // Skip memmove in the common case that there is nothing to move.
                SizeType NumToMove = ArrayNum - Index - Count;
                if (NumToMove)
                {
                    FMemory::memmove
                            (
                                    (uint8_t*)AllocatorInstance.GetAllocation() + (Index)* sizeof(ElementType),
                                    (uint8_t*)AllocatorInstance.GetAllocation() + (Index + Count) * sizeof(ElementType),
                                    NumToMove * sizeof(ElementType)
                            );
                }
                ArrayNum -= Count;

                if (bAllowShrinking)
                {
                    ResizeShrink();
                }
            }
        }

    public:
        FORCEINLINE void RemoveAt(SizeType Index)
        {
            RemoveAtImpl(Index, 1, true);
        }

        template <typename CountType>
        FORCEINLINE void RemoveAt(SizeType Index, CountType Count, bool bAllowShrinking = true)
        {
            static_assert(!std::is_same_v<CountType, bool>, "TArray::RemoveAt: unexpected bool passed as the Count argument");
            RemoveAtImpl(Index, (SizeType)Count, bAllowShrinking);
        }
    public:
        void Reset(SizeType NewSize = 0)
        {
            // If we have space to hold the excepted size, then don't reallocate
            if (NewSize <= ArrayMax)
            {
                DestructItems(GetData(), ArrayNum);
                ArrayNum = 0;
            }
            else
            {
                Empty(NewSize);
            }
        }

        void Empty(SizeType Slack = 0)
        {
            DestructItems(GetData(), ArrayNum);

            checkSlow(Slack >= 0);
            ArrayNum = 0;

            if (ArrayMax != Slack)
            {
                ResizeTo(Slack);
            }
        }

        void SetNum(SizeType NewNum, bool bAllowShrinking = true)
        {
            if (NewNum > Num())
            {
                const SizeType Diff = NewNum - ArrayNum;
                const SizeType Index = AddUninitialized(Diff);
                DefaultConstructItems<ElementType>((uint8_t*)AllocatorInstance.GetAllocation() + Index * sizeof(ElementType), Diff);
            }
            else if (NewNum < Num())
            {
                RemoveAt(NewNum, Num() - NewNum, bAllowShrinking);
            }
        }

        void SetNumZeroed(SizeType NewNum, bool bAllowShrinking = true)
        {
            if (NewNum > Num())
            {
                AddZeroed(NewNum - Num());
            }
            else if (NewNum < Num())
            {
                RemoveAt(NewNum, Num() - NewNum, bAllowShrinking);
            }
        }

        void SetNumUninitialized(SizeType NewNum, bool bAllowShrinking = true)
        {
            if (NewNum > Num())
            {
                AddUninitialized(NewNum - Num());
            }
            else if (NewNum < Num())
            {
                RemoveAt(NewNum, Num() - NewNum, bAllowShrinking);
            }
        }

        void SetNumUnsafeInternal(SizeType NewNum)
        {
            checkSlow(NewNum <= Num() && NewNum >= 0);
            ArrayNum = NewNum;
        }

        template <typename OtherElementType, typename OtherAllocator>
        void Append(const TArray<OtherElementType, OtherAllocator>& Source)
        {
            SizeType SourceCount = Source.Num();

            // Do nothing if the source is empty.
            if (!SourceCount)
            {
                return;
            }

            // Allocate memory for the new elements.
            Reserve(ArrayNum + SourceCount);
            ConstructItems<ElementType>(GetData() + ArrayNum, Source.GetData(), SourceCount);

            ArrayNum += SourceCount;
        }

        template <typename OtherElementType, typename OtherAllocator>
        void Append(TArray<OtherElementType, OtherAllocator>&& Source)
        {
            SizeType SourceCount = Source.Num();

            // Do nothing if the source is empty.
            if (!SourceCount)
            {
                return;
            }

            // Allocate memory for the new elements.
            Reserve(ArrayNum + SourceCount);
            RelocateConstructItems<ElementType>(GetData() + ArrayNum, Source.GetData(), SourceCount);
            Source.ArrayNum = 0;

            ArrayNum += SourceCount;
        }

        void Append(const ElementType* Ptr, SizeType Count)
        {
            SizeType Pos = AddUninitialized(Count);
            ConstructItems<ElementType>(GetData() + Pos, Ptr, Count);
        }

        FORCEINLINE void Append(std::initializer_list<ElementType> InitList)
        {
            SizeType Count = (SizeType)InitList.size();

            SizeType Pos = AddUninitialized(Count);
            ConstructItems<ElementType>(GetData() + Pos, InitList.begin(), Count);
        }

        TArray& operator+=(TArray&& Other)
        {
            Append(MoveTemp(Other));
            return *this;
        }

        TArray& operator+=(const TArray& Other)
        {
            Append(Other);
            return *this;
        }

        TArray& operator+=(std::initializer_list<ElementType> InitList)
        {
            Append(InitList);
            return *this;
        }

        template <typename... ArgsType>
        FORCEINLINE SizeType Emplace(ArgsType&&... Args)
        {
            const SizeType Index = AddUninitialized(1);
            new(GetData() + Index) ElementType(Forward<ArgsType>(Args)...);
            return Index;
        }

        template <typename... ArgsType>
        FORCEINLINE ElementType& Emplace_GetRef(ArgsType&&... Args)
        {
            const SizeType Index = AddUninitialized(1);
            ElementType* Ptr = GetData() + Index;
            new(Ptr) ElementType(Forward<ArgsType>(Args)...);
            return *Ptr;
        }

        template <typename... ArgsType>
        FORCEINLINE void EmplaceAt(SizeType Index, ArgsType&&... Args)
        {
            InsertUninitializedImpl(Index, 1);
            new(GetData() + Index) ElementType(Forward<ArgsType>(Args)...);
        }

        template <typename... ArgsType>
        FORCEINLINE ElementType& EmplaceAt_GetRef(SizeType Index, ArgsType&&... Args)
        {
            InsertUninitializedImpl(Index, 1);
            ElementType* Ptr = GetData() + Index;
            new(Ptr) ElementType(Forward<ArgsType>(Args)...);
            return *Ptr;
        }

        FORCEINLINE SizeType Add(ElementType&& Item)
        {
            return Emplace(MoveTempIfPossible(Item));
        }

        FORCEINLINE SizeType Add(const ElementType& Item)
        {
            CheckAddress(&Item);
            return Emplace(Item);
        }

        FORCEINLINE ElementType& Add_GetRef(ElementType&& Item)
        {
            return Emplace_GetRef(MoveTempIfPossible(Item));
        }

        FORCEINLINE ElementType& Add_GetRef(const ElementType& Item)
        {
            return Emplace_GetRef(Item);
        }

        SizeType AddZeroed(SizeType Count = 1)
        {
            const SizeType Index = AddUninitialized(Count);
            FMemory::memzero((uint8_t*)AllocatorInstance.GetAllocation() + Index*sizeof(ElementType), Count*sizeof(ElementType));
            return Index;
        }

        ElementType& AddZeroed_GetRef()
        {
            const SizeType Index = AddUninitialized(1);
            ElementType* Ptr = GetData() + Index;
            FMemory::memzero(Ptr, sizeof(ElementType));
            return *Ptr;
        }

        SizeType AddDefaulted(SizeType Count = 1)
        {
            const SizeType Index = AddUninitialized(Count);
            DefaultConstructItems<ElementType>((uint8_t*)AllocatorInstance.GetAllocation() + Index * sizeof(ElementType), Count);
            return Index;
        }

        ElementType& AddDefaulted_GetRef()
        {
            const SizeType Index = AddUninitialized(1);
            ElementType* Ptr = GetData() + Index;
            DefaultConstructItems<ElementType>(Ptr, 1);
            return *Ptr;
        }
    private:
        template <typename ArgsType>
        SizeType AddUniqueImpl(ArgsType&& Args)
        {
            SizeType Index;
            if (Find(Args, Index))
            {
                return Index;
            }
            return Add(Forward<ArgsType>(Args));
        }
    public:
        FORCEINLINE SizeType AddUnique(ElementType&& Item) { return AddUniqueImpl(MoveTempIfPossible(Item)); }

        FORCEINLINE SizeType AddUnique(const ElementType& Item) { return AddUniqueImpl(Item); }

        FORCEINLINE void Reserve(SizeType Number)
        {
            checkSlow(Number >= 0);
            if (Number > ArrayMax)
            {
                ResizeTo(Number);
            }
        }

        void Init(const ElementType& Element, SizeType Number)
        {
            Empty(Number);
            for (SizeType Index = 0; Index < Number; ++Index)
            {
                new(*this) ElementType(Element);
            }
        }

        SizeType RemoveSingle(const ElementType& Item)
        {
            SizeType Index = Find(Item);
            if (Index == INDEX_NONE)
            {
                return 0;
            }

            auto* RemovePtr = GetData() + Index;

            // Destruct items that match the specified Item.
            DestructItems(RemovePtr, 1);
            const SizeType NextIndex = Index + 1;
            RelocateConstructItems<ElementType>(RemovePtr, RemovePtr + 1, ArrayNum - (Index + 1));

            // Update the array count
            --ArrayNum;

            // Removed one item
            return 1;
        }

        SizeType Remove(const ElementType& Item)
        {
            return RemoveAll([&Item](ElementType& Element) { return Element == Item; });
        }

        template <class PREDICATE_CLASS>
        SizeType RemoveAll(const PREDICATE_CLASS& Predicate)
        {
            const SizeType OriginalNum = ArrayNum;
            if (!OriginalNum)
            {
                return 0; // nothing to do, loop assumes one item so need to deal with this edge case here
            }

            SizeType WriteIndex = 0;
            SizeType ReadIndex = 0;
            bool NotMatch = !Predicate(GetData()[ReadIndex]); // use a ! to guarantee it can't be anything other than zero or one
            do
            {
                SizeType RunStartIndex = ReadIndex++;
                while (ReadIndex < OriginalNum && NotMatch == !Predicate(GetData()[ReadIndex]))
                {
                    ReadIndex++;
                }
                SizeType RunLength = ReadIndex - RunStartIndex;
                checkSlow(RunLength > 0);
                if (NotMatch)
                {
                    // this was a non-matching run, we need to move it
                    if (WriteIndex != RunStartIndex)
                    {
                        FMemory::memmove(&GetData()[WriteIndex], &GetData()[RunStartIndex], sizeof(ElementType)* RunLength);
                    }
                    WriteIndex += RunLength;
                }
                else
                {
                    // this was a matching run, delete it
                    DestructItems(GetData() + RunStartIndex, RunLength);
                }
                NotMatch = !NotMatch;
            } while (ReadIndex < OriginalNum);

            ArrayNum = WriteIndex;
            return OriginalNum - ArrayNum;
        }

        // Iterators
        typedef TIndexedContainerIterator<      TArray,       ElementType, SizeType> TIterator;
        typedef TIndexedContainerIterator<const TArray, const ElementType, SizeType> TConstIterator;

        /**
         * Creates an iterator for the contents o f this array
         *
         * @returns The iterator.
         */
        TIterator CreateIterator()
        {
            return TIterator(*this);
        }

        /**
         * Creates a const iterator for the contents of this array
         *
         * @returns The const iterator.
         */
        TConstIterator CreateConstIterator() const
        {
            return TConstIterator(*this);
        }

        typedef       ElementType* RangedForIteratorType;
        typedef const ElementType* RangedForConstIteratorType;
    public:
        FORCEINLINE RangedForIteratorType      begin()       { return GetData(); }
        FORCEINLINE RangedForConstIteratorType begin() const { return GetData(); }
        FORCEINLINE RangedForIteratorType      end()         { return GetData() + Num(); }
        FORCEINLINE RangedForConstIteratorType end() const   { return GetData() + Num(); }
    private:

        FORCENOINLINE void ResizeGrow(SizeType OldNum)
        {
            ArrayMax = AllocatorInstance.CalculateSlackGrow(ArrayNum, ArrayMax, sizeof(ElementType));
            AllocatorInstance.ResizeAllocation(OldNum, ArrayMax, sizeof(ElementType));
        }

        FORCENOINLINE void ResizeShrink()
        {
            const SizeType NewArrayMax = AllocatorInstance.CalculateSlackShrink(ArrayNum, ArrayMax, sizeof(ElementType));
            if (NewArrayMax != ArrayMax)
            {
                ArrayMax = NewArrayMax;
                check(ArrayMax >= ArrayNum);
                AllocatorInstance.ResizeAllocation(ArrayNum, ArrayMax, sizeof(ElementType));
            }
        }

        FORCENOINLINE void ResizeTo(SizeType NewMax)
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

        FORCENOINLINE void ResizeForCopy(SizeType NewMax, SizeType PrevMax)
        {
            if (NewMax)
            {
                NewMax = AllocatorInstance.CalculateSlackReserve(NewMax, sizeof(ElementType));
            }
            if (NewMax > PrevMax)
            {
                AllocatorInstance.ResizeAllocation(0, NewMax, sizeof(ElementType));
                ArrayMax = NewMax;
            }
            else
            {
                ArrayMax = PrevMax;
            }
        }

        template <typename OtherElementType, typename OtherSizeType>
        void CopyToEmpty(const OtherElementType* OtherData, OtherSizeType OtherNum, SizeType PrevMax, SizeType ExtraSlack)
        {
            SizeType NewNum = (SizeType)OtherNum;

            ArrayNum = NewNum;
            if (OtherNum || ExtraSlack || PrevMax)
            {
                ResizeForCopy(NewNum + ExtraSlack, PrevMax);
                ConstructItems<ElementType>(GetData(), OtherData, OtherNum);
            }
            else
            {
                ArrayMax = AllocatorInstance.GetInitialCapacity();
            }
        }
    protected:
        ElementAllocatorType AllocatorInstance;
        SizeType             ArrayNum;
        SizeType             ArrayMax;
    public:
        const ElementAllocatorType& GetAllocatorInstance() const { return AllocatorInstance; }
        ElementAllocatorType& GetAllocatorInstance() { return AllocatorInstance; }
    };
}

#endif //UE4SS_TARRAY_H
