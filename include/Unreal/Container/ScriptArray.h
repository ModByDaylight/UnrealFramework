#ifndef UE4SS_SCRIPTARRAY_H
#define UE4SS_SCRIPTARRAY_H

#include <Unreal/Container/ContainerAllocationPolicies.h>
#include <Unreal/Common.hpp>

namespace RC::Unreal
{
    /**
     * Base dynamic array.
     * An untyped data array; mirrors a TArray's members, but doesn't need an exact C++ type for its elements.
     **/
    template <typename AllocatorType>
    class TScriptArray : protected AllocatorType::ForAnyElementType
    {
    public:
        FORCEINLINE void* GetData()
        {
            return this->GetAllocation();
        }

        FORCEINLINE const void* GetData() const
        {
            return this->GetAllocation();
        }

        FORCEINLINE bool IsValidIndex(int32_t i) const
        {
            return i>=0 && i<ArrayNum;
        }

        FORCEINLINE int32_t Num() const
        {
            if (!(ArrayNum >= 0 && ArrayMax >= ArrayNum)) {
                throw std::runtime_error("ScriptArray invariants are not satisfied");
            }
            return ArrayNum;
        }

        void InsertZeroed(int32_t Index, int32_t Count, int32_t NumBytesPerElement)
        {
            Insert(Index, Count, NumBytesPerElement);
            FMemory::memzero((uint8_t*) this->GetAllocation() + Index * NumBytesPerElement, Count * NumBytesPerElement);
        }

        void Insert(int32_t Index, int32_t Count, int32_t NumBytesPerElement)
        {
            if (!(ArrayNum >= 0 && ArrayMax >= ArrayNum)) {
                throw std::runtime_error("ScriptArray invariants are not satisfied");
            }
            if (!(Count >= 0)) {
                throw std::runtime_error("invalid element count");
            }

            const int32_t OldNum = ArrayNum;
            if( (ArrayNum+=Count)>ArrayMax )
            {
                ResizeGrow(OldNum, NumBytesPerElement);
            }
            FMemory::memmove(
                    (uint8_t*)this->GetAllocation() + (Index + Count) * NumBytesPerElement,
                    (uint8_t*)this->GetAllocation() + (Index) * NumBytesPerElement,
                    (OldNum - Index) * NumBytesPerElement
            );
        }
        int32_t Add(int32_t Count, int32_t NumBytesPerElement)
        {
            if (!(ArrayNum >= 0 && ArrayMax >= ArrayNum)) {
                throw std::runtime_error("ScriptArray invariants are not satisfied");
            }
            if (!(Count >= 0)) {
                throw std::runtime_error("invalid element count");
            }

            const int32_t OldNum = ArrayNum;
            if((ArrayNum += Count) > ArrayMax)
            {
                ResizeGrow(OldNum, NumBytesPerElement);
            }
            return OldNum;
        }
        int32_t AddZeroed(int32_t Count, int32_t NumBytesPerElement)
        {
            const int32_t Index = Add(Count, NumBytesPerElement);
            FMemory::memzero((uint8_t*) this->GetAllocation() + Index * NumBytesPerElement, Count * NumBytesPerElement);
            return Index;
        }
        
        void Shrink(int32_t NumBytesPerElement)
        {
            if (!(ArrayNum >= 0 && ArrayMax >= ArrayNum)) {
                throw std::runtime_error("ScriptArray invariants are not satisfied");
            }
            if (ArrayNum != ArrayMax)
            {
                ResizeTo(ArrayNum, NumBytesPerElement);
            }
        }
        
        void MoveAssign(TScriptArray& Other, int32_t NumBytesPerElement)
        {
            Empty(0, NumBytesPerElement);
            this->MoveToEmpty(Other);
            ArrayNum = Other.ArrayNum; Other.ArrayNum = 0;
            ArrayMax = Other.ArrayMax; Other.ArrayMax = 0;
        }
        
        void Empty(int32_t Slack, int32_t NumBytesPerElement)
        {
            if (!(Slack >= 0)) {
                throw std::runtime_error("invalid slack count");
            }
            ArrayNum = 0;
            if (Slack != ArrayMax)
            {
                ResizeTo(Slack, NumBytesPerElement);
            }
        }
        
        TScriptArray() : ArrayNum(0), ArrayMax(0) {
        }
        
        FORCEINLINE int32_t GetSlack() const
        {
            return ArrayMax - ArrayNum;
        }

        void Remove(int32_t Index, int32_t Count, int32_t NumBytesPerElement)
        {
            if (Count)
            {
                if (!(Count >= 0 && Index >= 0 && Index <= ArrayNum && Index + Count <= ArrayNum)) {
                    throw std::runtime_error("FScriptArray Remove is out of bounds");
                }
                
                int32_t NumToMove = ArrayNum - Index - Count;
                if (NumToMove)
                {
                    FMemory::memmove(
                        (uint8_t*) this->GetAllocation() + (Index) * NumBytesPerElement,
                        (uint8_t*) this->GetAllocation() + (Index + Count) * NumBytesPerElement,
                        NumToMove * NumBytesPerElement);
                }
                ArrayNum -= Count;

                ResizeShrink(NumBytesPerElement);
                
                if (!(ArrayNum >= 0 && ArrayMax >= ArrayNum)) {
                    throw std::runtime_error("ScriptArray invariants are not satisfied");
                }
            }
        }

    protected:
        TScriptArray(int32_t InNum, int32_t NumBytesPerElement) : ArrayNum(0), ArrayMax(InNum)
        {
            if (ArrayMax)
            {
                ResizeInit(NumBytesPerElement);
            }
            ArrayNum = InNum;
        }
        
        int32_t ArrayNum;
        int32_t ArrayMax;

        FORCENOINLINE void ResizeInit(int32_t NumBytesPerElement)
        {
            ArrayMax = this->CalculateSlackReserve(ArrayMax, NumBytesPerElement);
            this->ResizeAllocation(ArrayNum, ArrayMax, NumBytesPerElement);
        }

        FORCENOINLINE void ResizeGrow(int32_t OldNum, int32_t NumBytesPerElement)
        {
            ArrayMax = this->CalculateSlackGrow(ArrayNum, ArrayMax, NumBytesPerElement);
            this->ResizeAllocation(OldNum, ArrayMax, NumBytesPerElement);
        }

        FORCENOINLINE void ResizeShrink(int32_t NumBytesPerElement)
        {
            const int32_t NewArrayMax = this->CalculateSlackShrink(ArrayNum, ArrayMax, NumBytesPerElement);
            if (NewArrayMax != ArrayMax)
            {
                ArrayMax = NewArrayMax;
                this->ResizeAllocation(ArrayNum, ArrayMax, NumBytesPerElement);
            }
        }

        FORCENOINLINE void ResizeTo(int32_t NewMax, int32_t NumBytesPerElement)
        {
            if (NewMax)
            {
                NewMax = this->CalculateSlackReserve(NewMax, NumBytesPerElement);
            }
            if (NewMax != ArrayMax)
            {
                ArrayMax = NewMax;
                this->ResizeAllocation(ArrayNum, ArrayMax, NumBytesPerElement);
            }
        }
    private:
        TScriptArray(const TScriptArray&) = delete;
        void operator=(const TScriptArray&) = delete;
    };
    
    class FScriptArray : public TScriptArray<FHeapAllocator>
    {
        using Super = TScriptArray<FHeapAllocator>;
    public:
        FScriptArray() = default;

        void MoveAssign(FScriptArray& Other, int32_t NumBytesPerElement)
        {
            Super::MoveAssign(Other, NumBytesPerElement);
        }
    protected:
        FScriptArray(int32_t InNum, int32_t NumBytesPerElement)
                : TScriptArray<FHeapAllocator>(InNum, NumBytesPerElement) {
        }
    };
}

#endif //UE4SS_SCRIPTARRAY_H
