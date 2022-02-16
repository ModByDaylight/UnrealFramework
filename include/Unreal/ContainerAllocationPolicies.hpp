#ifndef RC_UNREAL_CONTAINER_ALLOCATION_POLICIES_HPP
#define RC_UNREAL_CONTAINER_ALLOCATION_POLICIES_HPP

#include <Unreal/PrimitiveTypes.hpp>
#include <Unreal/FMemory.hpp>

namespace RC::Unreal
{
    template<int IndexSize>
    class TSizedDefaultAllocator;

    using FDefaultAllocator = TSizedDefaultAllocator<32>;

    template<int IndexSize>
    struct TBitsToSizeType
    {
        static_assert(IndexSize, "Unsupported allocator index size.");
    };

    template<>
    struct TBitsToSizeType<8> { using Type = int8; };
    template<>
    struct TBitsToSizeType<16> { using Type = int16; };
    template<>
    struct TBitsToSizeType<32> { using Type = int32; };
    template<>
    struct TBitsToSizeType<64> { using Type = int64; };

    template <typename SizeType>
    SizeType DefaultCalculateSlackShrink(SizeType NumElements, SizeType NumAllocatedElements, SIZE_T BytesPerElement, bool bAllowQuantize, uint32 Alignment = DEFAULT_ALIGNMENT)
    {
        // TODO: Implement, and do not use until implemented
        static_assert(false, "DefaultCalculateSlackShrink is not implemented");
        return 0;
    }

    template <typename SizeType>
    SizeType DefaultCalculateSlackGrow(SizeType NumElements, SizeType NumAllocatedElements, SIZE_T BytesPerElement, bool bAllowQuantize, uint32 Alignment = DEFAULT_ALIGNMENT)
    {
        // TODO: Implement this better
        return NumElements * 2;
    }

    template <typename SizeType>
    SizeType DefaultCalculateSlackReserve(SizeType NumElements, SIZE_T BytesPerElement, bool bAllowQuantize, uint32 Alignment = DEFAULT_ALIGNMENT)
    {
        // TODO: Implement this better
        return NumElements * 2;
    }

    /** A type which is used to represent a script type that is unknown at compile time. */
    struct FScriptContainerElement
    {
    };

    template<int IndexSize>
    class TSizedHeapAllocator
    {
    public:
        using SizeType = typename TBitsToSizeType<IndexSize>::Type;

        enum { NeedsElementType = false };
        enum { RequireRangeCheck = true };

        class ForAnyElementType
        {
            // Hack to allow the allocator to be set by the TArray constructor for when we want to interact with an already constucted TArray in memory
            template<typename T1, typename T2>
            friend class TArray;

            template <int>
            friend class TSizedHeapAllocator;

        public:
            /** Default constructor. */
            ForAnyElementType()
                    : Data(nullptr)
            {}

            /**
             * Moves the state of another allocator into this one.  The allocator can be different.
             *
             * Assumes that the allocator is currently empty, i.e. memory may be allocated but any existing elements have already been destructed (if necessary).
             * @param Other - The allocator to move the state from.  This allocator should be left in a valid empty state.
             */
            template <typename OtherAllocator>
            void MoveToEmptyFromOtherAllocator(typename OtherAllocator::ForAnyElementType& Other)
            {
                if (Data)
                {
                    FMemory::Free(Data);
                }

                Data = Other.Data;
                Other.Data = nullptr;
            }

            /**
             * Moves the state of another allocator into this one.
             * Moves the state of another allocator into this one.  The allocator can be different.
             *
             * Assumes that the allocator is currently empty, i.e. memory may be allocated but any existing elements have already been destructed (if necessary).
             * @param Other - The allocator to move the state from.  This allocator should be left in a valid empty state.
             */
            void MoveToEmpty(ForAnyElementType& Other)
            {
                this->MoveToEmptyFromOtherAllocator<TSizedHeapAllocator>(Other);
            }

            /** Destructor. */
            ~ForAnyElementType()
            {
                if(Data)
                {
                    FMemory::Free(Data);
                }
            }

            // FContainerAllocatorInterface
            FScriptContainerElement* GetAllocation() const
            {
                return Data;
            }
            void ResizeAllocation(SizeType PreviousNumElements, SizeType NumElements, SIZE_T NumBytesPerElement)
            {
                // Avoid calling FMemory::Realloc( nullptr, 0 ) as ANSI C mandates returning a valid pointer which is not what we want.
                if (Data || NumElements)
                {
                    //checkSlow(((uint64)NumElements*(uint64)ElementTypeInfo.GetSize() < (uint64)INT_MAX));
                    Data = (FScriptContainerElement*)FMemory::Realloc( Data, NumElements*NumBytesPerElement );
                }
            }
            SizeType CalculateSlackReserve(SizeType NumElements, SIZE_T NumBytesPerElement) const
            {
                return DefaultCalculateSlackReserve(NumElements, NumBytesPerElement, true);
            }
            SizeType CalculateSlackShrink(SizeType NumElements, SizeType NumAllocatedElements, SIZE_T NumBytesPerElement) const
            {
                return DefaultCalculateSlackShrink(NumElements, NumAllocatedElements, NumBytesPerElement, true);
            }
            SizeType CalculateSlackGrow(SizeType NumElements, SizeType NumAllocatedElements, SIZE_T NumBytesPerElement) const
            {
                return DefaultCalculateSlackGrow(NumElements, NumAllocatedElements, NumBytesPerElement, true);
            }

            SIZE_T GetAllocatedSize(SizeType NumAllocatedElements, SIZE_T NumBytesPerElement) const
            {
                return NumAllocatedElements * NumBytesPerElement;
            }

            bool HasAllocation() const
            {
                return !!Data;
            }

            SizeType GetInitialCapacity() const
            {
                return 0;
            }

        private:
            ForAnyElementType(const ForAnyElementType&) = default;
            ForAnyElementType& operator=(const ForAnyElementType&) = default;

            /** A pointer to the container's elements. */
            FScriptContainerElement* Data;
        };

        template<typename ElementType>
        class ForElementType : public ForAnyElementType
        {
        public:

            /** Default constructor. */
            ForElementType()
            {}

            ElementType* GetAllocation() const
            {
                return (ElementType*)ForAnyElementType::GetAllocation();
            }
        };
    };

    template<int IndexSize>
    class TSizedDefaultAllocator : public TSizedHeapAllocator<IndexSize>
    {
    public:
        using Typedef = TSizedHeapAllocator<IndexSize>;
    };
}

#endif //RC_UNREAL_CONTAINER_ALLOCATION_POLICIES_HPP
