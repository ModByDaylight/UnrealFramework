#ifndef UNREAL_DELEGATEBASE_HPP
#define UNREAL_DELEGATEBASE_HPP

#include <Unreal/TypeCompatibleBytes.hpp>
#include <Unreal/FMemory.hpp>
#include <Unreal/ContainerAllocationPolicies.hpp>
#include <Unreal/IDelegateInstance.hpp>

#ifndef NUM_DELEGATE_INLINE_BYTES
#define NUM_DELEGATE_INLINE_BYTES 0
#endif

NS_RC_UE_START

struct FWeakObjectPtr;

typedef TAlignedBytes<16, 16> FAlignedInlineDelegateType;

#if !defined(NUM_DELEGATE_INLINE_BYTES) || NUM_DELEGATE_INLINE_BYTES == 0
    typedef FHeapAllocator FDelegateAllocatorType;
#else
    typedef TInlineAllocator<(NUM_DELEGATE_INLINE_BYTES / 16)> FDelegateAllocatorType;
#endif

struct FWeakObjectPtr;

template <typename UserPolicy>
class TMulticastDelegateBase;

/**
 * Base class for unicast delegates.
 */
class FDelegateBase {
    template<typename>
    friend class TMulticastDelegateBase;

    template<typename>
    friend class TDelegateBase;
protected:
    /**
     * Creates and initializes a new instance.
     *
     * @param InDelegateInstance The delegate instance to assign.
     */
    FORCEINLINE explicit FDelegateBase(): DelegateSize(0) {
    }

    FORCEINLINE ~FDelegateBase() {
        Unbind();
    }

    /**
     * Move constructor.
     */
    FORCEINLINE FDelegateBase(FDelegateBase&& Other) {
        DelegateAllocator.MoveToEmpty(Other.DelegateAllocator);
        DelegateSize = Other.DelegateSize;
        Other.DelegateSize = 0;
    }

    /**
     * Move assignment.
     */
    FORCEINLINE FDelegateBase& operator=(FDelegateBase&& Other) {
        Unbind();
        DelegateAllocator.MoveToEmpty(Other.DelegateAllocator);
        DelegateSize = Other.DelegateSize;
        Other.DelegateSize = 0;
        return *this;
    }
public:
    /**
     * Unbinds this delegate
     */
    FORCEINLINE void Unbind() {
        if (IDelegateInstance* Ptr = GetDelegateInstanceProtected()) {
            Ptr->~IDelegateInstance();
            DelegateAllocator.ResizeAllocation(0, 0, sizeof(FAlignedInlineDelegateType));
            DelegateSize = 0;
        }
    }
protected:
    /**
     * Gets the delegate instance.  Not intended for use by user code.
     *
     * @return The delegate instance.
     * @see SetDelegateInstance
     */
    FORCEINLINE IDelegateInstance* GetDelegateInstanceProtected() const {
        return DelegateSize ? (IDelegateInstance*) DelegateAllocator.GetAllocation() : nullptr;
    }
public:
    void* AllocateInternal(int32 Size) {
        if (IDelegateInstance* CurrentInstance = GetDelegateInstanceProtected()) {
            CurrentInstance->~IDelegateInstance();
        }

        int32 NewDelegateSize = DivideAndRoundUp(Size, (int32)sizeof(FAlignedInlineDelegateType));
        if (DelegateSize != NewDelegateSize) {
            DelegateAllocator.ResizeAllocation(0, NewDelegateSize, sizeof(FAlignedInlineDelegateType));
            DelegateSize = NewDelegateSize;
        }
        return DelegateAllocator.GetAllocation();
    }
private:
    FDelegateAllocatorType::ForElementType<FAlignedInlineDelegateType> DelegateAllocator;
    int32 DelegateSize;
};

struct FDefaultDelegateUserPolicy {
    using FDelegateInstanceExtras  = IDelegateInstance;
    using FDelegateExtras          = FDelegateBase;
    using FMulticastDelegateExtras = TMulticastDelegateBase<FDefaultDelegateUserPolicy>;
};

template <typename UserPolicy = FDefaultDelegateUserPolicy>
class TDelegateBase : public UserPolicy::FDelegateExtras {
    template <typename>
    friend class TMulticastDelegateBase;

    using Super = typename UserPolicy::FDelegateExtras;
public:
#if USE_DELEGATE_TRYGETBOUNDFUNCTIONNAME
    /**
     * Tries to return the name of a bound function.  Returns NAME_None if the delegate is unbound or
     * a binding name is unavailable.
     *
     * Note: Only intended to be used to aid debugging of delegates.
     *
     * @return The name of the bound function, NAME_None if no name was available.
     */
    FName TryGetBoundFunctionName() const {
        if (IDelegateInstance* Ptr = Super::GetDelegateInstanceProtected()) {
            return Ptr->TryGetBoundFunctionName();
        }
        return NAME_None;
    }
#endif

    /**
     * If this is a UFunction or UObject delegate, return the UObject.
     *
     * @return The object associated with this delegate if there is one.
     */
    FORCEINLINE class UObject* GetUObject() const {
        if (IDelegateInstance* Ptr = Super::GetDelegateInstanceProtected()) {
            return Ptr->GetUObject();
        }
        return nullptr;
    }

    /**
     * Checks to see if the user object bound to this delegate is still valid.
     *
     * @return True if the user object is still valid and it's safe to execute the function call.
     */
    FORCEINLINE bool IsBound() const {
        IDelegateInstance* Ptr = Super::GetDelegateInstanceProtected();

        return Ptr && Ptr->IsSafeToExecute();
    }

    /**
     * Returns a pointer to an object bound to this delegate, intended for quick lookup in the timer manager,
     *
     * @return A pointer to an object referenced by the delegate.
     */
    FORCEINLINE const void* GetObjectForTimerManager() const {
        IDelegateInstance* Ptr = Super::GetDelegateInstanceProtected();

        const void* Result = Ptr ? Ptr->GetObjectForTimerManager() : nullptr;
        return Result;
    }

    /**
     * Returns the address of the method pointer which can be used to learn the address of the function that will be executed.
     * Returns nullptr if this delegate type does not directly invoke a function pointer.
     *
     * Note: Only intended to be used to aid debugging of delegates.
     *
     * @return The address of the function pointer that would be executed by this delegate
     */
    uint64 GetBoundProgramCounterForTimerManager() const {
        if (IDelegateInstance* Ptr = Super::GetDelegateInstanceProtected()) {
            return Ptr->GetBoundProgramCounterForTimerManager();
        }
        return 0;
    }

    /**
     * Checks to see if this delegate is bound to the given user object.
     *
     * @return True if this delegate is bound to InUserObject, false otherwise.
     */
    FORCEINLINE bool IsBoundToObject(const void* InUserObject ) const {
        if (!InUserObject) {
            return false;
        }

        IDelegateInstance* Ptr = Super::GetDelegateInstanceProtected();
        return Ptr && Ptr->HasSameObject(InUserObject);
    }

    /**
     * Gets a handle to the delegate.
     *
     * @return The delegate instance.
     */
    FORCEINLINE FDelegateHandle GetHandle() const {
        FDelegateHandle Result;
        if (IDelegateInstance* Ptr = Super::GetDelegateInstanceProtected()) {
            Result = Ptr->GetHandle();
        }
        return Result;
    }
};

NS_RC_UE_END

FORCEINLINE void* operator new(size_t Size, RC::Unreal::FDelegateBase& Base) {
    return Base.AllocateInternal((RC::Unreal::int32) Size);
}

#endif //UNREAL_DELEGATEBASE_HPP
