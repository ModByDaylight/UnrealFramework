#ifndef UE4SS_CONTAINERALLOCATIONPOLICIES_H
#define UE4SS_CONTAINERALLOCATIONPOLICIES_H

#include <type_traits>
#include "Unreal/FMemory.hpp"
#include <Unreal/Common.hpp>

namespace RC::Unreal {

    /** A type which is used to represent a script type that is unknown at compile time. */
    struct FScriptContainerElement
    {
    };

    /** The indirect allocation policy always allocates the elements indirectly. */
    template<typename SizeType>
    class TSizedHeapAllocator {
    public:
        enum { NeedsElementType = false };
        enum { RequireRangeCheck = true };

        class ForAnyElementType {
            template<typename>
            friend class TSizedHeapAllocator;
        public:
            /** Default constructor. */
            ForAnyElementType() : Data(nullptr) {
            }

            /**
             * Moves the state of another allocator into this one.  The allocator can be different.
             *
             * Assumes that the allocator is currently empty, i.e. memory may be allocated but any existing elements have already been destructed (if necessary).
             * @param Other - The allocator to move the state from.  This allocator should be left in a valid empty state.
             */
            template<typename OtherAllocator>
            inline void MoveToEmptyFromOtherAllocator(typename OtherAllocator::ForAnyElementType& Other)
            {
                if (Data) {
                    FMemory::free(Data);
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
            inline void MoveToEmpty(ForAnyElementType &Other) {
                this->MoveToEmptyFromOtherAllocator<TSizedHeapAllocator>(Other);
            }

            /** Destructor. */
            inline ~ForAnyElementType() {
                if (Data) {
                    FMemory::free(Data);
                }
            }

            inline FScriptContainerElement* GetAllocation() const {
                return Data;
            }

            inline void ResizeAllocation(SizeType PreviousNumElements, SizeType NumElements, size_t NumBytesPerElement)
            {
                if (Data || NumElements) {
                    Data = (FScriptContainerElement*) FMemory::realloc(Data, NumElements * NumBytesPerElement);
                }
            }

            inline SizeType CalculateSlackReserve(SizeType NumElements, size_t NumBytesPerElement) const
            {
                return DefaultCalculateSlackReserve(NumElements, NumBytesPerElement, true);
            }

            inline SizeType CalculateSlackShrink(SizeType NumElements, SizeType NumAllocatedElements, size_t NumBytesPerElement) const
            {
                return DefaultCalculateSlackShrink(NumElements, NumAllocatedElements, NumBytesPerElement, true);
            }

            inline SizeType CalculateSlackGrow(SizeType NumElements, SizeType NumAllocatedElements, size_t NumBytesPerElement) const
            {
                return DefaultCalculateSlackGrow(NumElements, NumAllocatedElements, NumBytesPerElement, true);
            }

            size_t GetAllocatedSize(SizeType NumAllocatedElements, size_t NumBytesPerElement) const {
                return NumAllocatedElements * NumBytesPerElement;
            }

            bool HasAllocation() const {
                return !!Data;
            }

            SizeType GetInitialCapacity() const {
                return 0;
            }
        private:
            ForAnyElementType(const ForAnyElementType&);
            ForAnyElementType& operator=(const ForAnyElementType&);

            /** A pointer to the container's elements. */
            FScriptContainerElement *Data;
        };

        template<typename ElementType>
        class ForElementType : public ForAnyElementType {
        public:
            /** Default constructor. */
            ForElementType() {}

            inline ElementType* GetAllocation() const
            {
                return (ElementType*) ForAnyElementType::GetAllocation();
            }
        };
    };

    using FHeapAllocator = TSizedHeapAllocator<int32_t>;
    using FDefaultAllocator = FHeapAllocator;
}
#endif //UE4SS_CONTAINERALLOCATIONPOLICIES_H
