#ifndef RC_UNREAL_FUNCTION_HPP
#define RC_UNREAL_FUNCTION_HPP

#include <Unreal/TypeCompatibleBytes.hpp>
#include <Unreal/FMemory.hpp>
#include <Unreal/TypeTraits.hpp>

#ifndef NUM_TFUNCTION_INLINE_BYTES
#define NUM_TFUNCTION_INLINE_BYTES 32
#endif

#if !defined(NUM_TFUNCTION_INLINE_BYTES) || NUM_TFUNCTION_INLINE_BYTES == 0
#define TFUNCTION_USES_INLINE_STORAGE 0
#else
#define TFUNCTION_USES_INLINE_STORAGE 1
#define TFUNCTION_INLINE_SIZE         NUM_TFUNCTION_INLINE_BYTES
#define TFUNCTION_INLINE_ALIGNMENT    16
#endif

namespace RC::Unreal
{
    template <typename FuncType>
    class TFunctionRef;

    template <typename FuncType>
    class TFunction;

    template <bool bUnique>
    struct TFunctionStorage;

    /**
     * Traits class which checks if T is a TFunctionRef<> type.
     */
    template <typename T> struct TIsTFunctionRef                  { enum { Value = false }; };
    template <typename T> struct TIsTFunctionRef<TFunctionRef<T>> { enum { Value = true  }; };

    template <typename T> struct TIsTFunctionRef<const          T> { enum { Value = TIsTFunctionRef<T>::Value }; };
    template <typename T> struct TIsTFunctionRef<      volatile T> { enum { Value = TIsTFunctionRef<T>::Value }; };
    template <typename T> struct TIsTFunctionRef<const volatile T> { enum { Value = TIsTFunctionRef<T>::Value }; };

    /**
     * Traits class which checks if T is a TFunction<> type.
     */
    template <typename T> struct TIsTFunction               { enum { Value = false }; };
    template <typename T> struct TIsTFunction<TFunction<T>> { enum { Value = true  }; };

    template <typename T> struct TIsTFunction<const          T> { enum { Value = TIsTFunction<T>::Value }; };
    template <typename T> struct TIsTFunction<      volatile T> { enum { Value = TIsTFunction<T>::Value }; };
    template <typename T> struct TIsTFunction<const volatile T> { enum { Value = TIsTFunction<T>::Value }; };

    template <typename FunctorType, typename Ret, typename... ParamTypes>
    struct TFunctorReturnTypeIsCompatible : std::is_constructible<Ret, decltype(DeclVal<FunctorType>()(DeclVal<ParamTypes>()...))> {
    };

    template <typename MemberRet, typename Class, typename Ret, typename... ParamTypes>
    struct TFunctorReturnTypeIsCompatible<MemberRet Class::*, Ret, ParamTypes...> : std::is_constructible<Ret, MemberRet> {
    };

    template <typename MemberRet, typename Class, typename Ret, typename... ParamTypes>
    struct TFunctorReturnTypeIsCompatible<MemberRet Class::* const, Ret, ParamTypes...> : std::is_constructible<Ret, MemberRet> {
    };

    template <typename MemberRet, typename Class, typename... MemberParamTypes, typename Ret, typename... ParamTypes>
    struct TFunctorReturnTypeIsCompatible<MemberRet (Class::*)(MemberParamTypes...), Ret, ParamTypes...> : std::is_constructible<Ret, MemberRet> {
    };

    template <typename MemberRet, typename Class, typename... MemberParamTypes, typename Ret, typename... ParamTypes>
    struct TFunctorReturnTypeIsCompatible<MemberRet (Class::*)(MemberParamTypes...) const, Ret, ParamTypes...> : std::is_constructible<Ret, MemberRet> {
    };

    template <typename FuncType, typename FunctorType>
    struct TFuncCanBindToFunctor;

    template <typename FunctorType, typename Ret, typename... ParamTypes>
    struct TFuncCanBindToFunctor<Ret(ParamTypes...), FunctorType> {
        static constexpr bool Value = std::is_invocable_v<FunctorType, ParamTypes...> &&
                TFunctorReturnTypeIsCompatible<FunctorType, Ret, ParamTypes...>::value;
    };

    template <typename FunctorType, typename... ParamTypes>
    struct TFuncCanBindToFunctor<void(ParamTypes...), FunctorType> : std::is_invocable<FunctorType, ParamTypes...> {
    };

    /**
     * A class which is used to instantiate the code needed to call a bound function.
     */
    template <typename Functor, typename FuncType>
    struct TFunctionRefCaller;

    template <typename Functor, typename Ret, typename... ParamTypes>
    struct TFunctionRefCaller<Functor, Ret (ParamTypes...)> {
        static Ret Call(void* Obj, ParamTypes&... Params) {
            return Invoke(*(Functor*)Obj, Forward<ParamTypes>(Params)...);
        }
    };

    template <typename Functor, typename... ParamTypes>
    struct TFunctionRefCaller<Functor, void (ParamTypes...)> {
        static void Call(void* Obj, ParamTypes&... Params) {
            Invoke(*(Functor*)Obj, Forward<ParamTypes>(Params)...);
        }
    };

    template <typename T>
    FORCEINLINE bool IsBound(const T& Func) {
        if constexpr (std::is_pointer_v<T> || std::is_member_pointer_v<T> || TIsTFunction<T>::Value) {
            // Function pointers, data member pointers, member function pointers and TFunctions
            // can all be null/unbound, so test them using their boolean state.
            return !!Func;
        }
        else {
            // We can't tell if any other generic callable can be invoked, so just assume they can be.
            return true;
        }
    }

    template <typename StorageType, typename FuncType>
    struct TFunctionRefBase;

    /**
	 * A class which defines an operator() which will own storage of a callable and invoke the TFunctionRefCaller::Call function on it.
	 */
    template <typename StorageType, typename Ret, typename... ParamTypes>
    struct TFunctionRefBase<StorageType, Ret (ParamTypes...)> {
        template <typename OtherStorageType, typename OtherFuncType>
        friend struct TFunctionRefBase;

        FORCEINLINE TFunctionRefBase() : Callable(nullptr) {
        }

        FORCEINLINE TFunctionRefBase(TFunctionRefBase&& Other) : Callable(Other.Callable), Storage(MoveTemp(Other.Storage)) {
            if (Callable) {
                Other.Callable = nullptr;
            }
        }

        template <typename OtherStorage>
        FORCEINLINE TFunctionRefBase(TFunctionRefBase<OtherStorage, Ret(ParamTypes...)>&& Other) : Callable(Other.Callable), Storage (MoveTemp(Other.Storage)) {
            if (Callable) {
                Other.Callable = nullptr;
            }
        }

        template <typename OtherStorage>
        FORCEINLINE TFunctionRefBase(const TFunctionRefBase<OtherStorage, Ret (ParamTypes...)>& Other) : Callable(Other.Callable) {
            if (Callable) {
                Storage.BindCopy(Other.Storage);
            }
        }

        FORCEINLINE TFunctionRefBase(const TFunctionRefBase& Other) : Callable(Other.Callable) {
            if (Callable) {
                Storage.BindCopy(Other.Storage);
            }
        }

        template<typename FunctorType, typename = std::enable_if_t<!std::is_same_v<TFunctionRefBase, std::decay_t<FunctorType>>>>
        FORCEINLINE TFunctionRefBase(FunctorType&& InFunc) {
            if (auto* Binding = Storage.Bind(Forward<FunctorType>(InFunc))) {
                using DecayedFunctorType = std::remove_pointer_t<decltype(Binding)>;
                Callable = &TFunctionRefCaller<DecayedFunctorType, Ret (ParamTypes...)>::Call;
            }
        }

        TFunctionRefBase& operator=(TFunctionRefBase&&) = delete;
        TFunctionRefBase& operator=(const TFunctionRefBase&) = delete;

        FORCENOINLINE void CheckCallable() const {
            if (Callable == nullptr) {
                throw std::runtime_error{"Attempting to call an unbound TFunction!"};
            }
        }

        FORCEINLINE Ret operator()(ParamTypes... Params) const {
            CheckCallable();
            return Callable(Storage.GetPtr(), Params...);
        }

        FORCEINLINE ~TFunctionRefBase() {
            if (Callable) {
                Storage.Unbind();
            }
        }
    protected:
        FORCEINLINE bool IsSet() const {
            return !!Callable;
        }
    private:
        // A pointer to a function which invokes the call operator on the callable object
        Ret (*Callable)(void*, ParamTypes&...);
        StorageType Storage;
    };

    struct FFunctionRefStoragePolicy {
        template <typename FunctorType>
        typename std::remove_reference_t<FunctorType>* Bind(FunctorType&& InFunc) {
            if (!IsBound(InFunc)) {
                throw std::runtime_error{"Cannot bind a null/unbound callable to a TFunctionRef"};
            }
            Ptr = (void*)&InFunc;
            return &InFunc;
        }

        void* BindCopy(const FFunctionRefStoragePolicy& Other) {
            void* OtherPtr = Other.Ptr;
            Ptr = OtherPtr;
            return OtherPtr;
        }

        /**
         * Returns a pointer to the callable object - needed by TFunctionRefBase.
         */
        void* GetPtr() const {
            return Ptr;
        }

        /**
         * Destroy any owned bindings - called by TFunctionRefBase only if Bind() or BindCopy() was called.
         */
        void Unbind() const {
            // FunctionRefs don't own their binding - do nothing
        }
    private:
        // A pointer to the callable object
        void* Ptr;
    };

    template <typename FuncType>
    class TFunctionRef : public TFunctionRefBase<FFunctionRefStoragePolicy, FuncType> {
        using Super = TFunctionRefBase<FFunctionRefStoragePolicy, FuncType>;
    public:
        /**
         * Constructor which binds a TFunctionRef to a callable object.
         */

        template<typename FunctorType, typename = std::enable_if_t<!TIsTFunctionRef<std::decay_t<FunctorType>>::Value && TFuncCanBindToFunctor<FuncType, std::decay_t<FunctorType>>::Value>>
        TFunctionRef(FunctorType&& InFunc): Super(Forward<FunctorType>(InFunc)) {
            // This constructor is disabled for TFunctionRef types so it isn't incorrectly selected as copy/move constructors.
        }

        TFunctionRef(const TFunctionRef&) = default;

        // We delete the assignment operators because we don't want it to be confused with being related to
        // regular C++ reference assignment - i.e. calling the assignment operator of whatever the reference
        // is bound to - because that's not what TFunctionRef does, nor is it even capable of doing that.
        TFunctionRef& operator=(const TFunctionRef&) const = delete;
        ~TFunctionRef() = default;
    };

    /**
     * Common interface to a callable object owned by TFunction.
     */
    struct IFunction_OwnedObject {
        /**
         * Creates a copy of itself into the storage and returns a pointer to the new object within it.
         */
        virtual void* CloneToEmptyStorage(void* Storage) const = 0;

        /**
         * Returns the address of the object.
         */
        virtual void* GetAddress() = 0;

        /**
         * Destructor.
         */
        virtual void Destroy() = 0;

        /**
         * Destructor.
         */
        virtual ~IFunction_OwnedObject() = default;
    };

    /**
     * Common interface to a callable object owned by TFunction.
     */
    template <typename T>
    struct IFunction_OwnedObject_OnHeap : public IFunction_OwnedObject {
        /**
         * Destructor.
         */
        void Destroy() override {
            void* This = this;
            this->~IFunction_OwnedObject_OnHeap();
            FMemory::Free(This);
        }

        ~IFunction_OwnedObject_OnHeap() override {
            // It is not necessary to define this destructor but MSVC will
            // erroneously issue warning C5046 without it.
        }
    };

    /**
     * Common interface to a callable object owned by TFunction.
     */
    template <typename T>
    struct IFunction_OwnedObject_Inline : public IFunction_OwnedObject {
        /**
         * Destructor.
         */
        void Destroy() override {
            this->~IFunction_OwnedObject_Inline();
        }

        ~IFunction_OwnedObject_Inline() override {
            // It is not necessary to define this destructor but MSVC will
            // erroneously issue warning C5046 without it.
        }
    };

    struct FFunctionStorage {
        FORCEINLINE FFunctionStorage() : HeapAllocation(nullptr) {
        }

        FORCEINLINE FFunctionStorage(FFunctionStorage&& Other) : HeapAllocation(Other.HeapAllocation) {
            Other.HeapAllocation = nullptr;
#if TFUNCTION_USES_INLINE_STORAGE
            FMemory::Memcpy(&InlineAllocation, &Other.InlineAllocation, sizeof(InlineAllocation));
#endif
        }

        FFunctionStorage(const FFunctionStorage& Other) = delete;
        FFunctionStorage& operator=(FFunctionStorage&& Other) = delete;
        FFunctionStorage& operator=(const FFunctionStorage& Other) = delete;

        FORCEINLINE void* BindCopy(const FFunctionStorage& Other) {
            void* NewObj = Other.GetBoundObject()->CloneToEmptyStorage(this);
            return NewObj;
        }

        FORCEINLINE IFunction_OwnedObject* GetBoundObject() const {
            IFunction_OwnedObject* Result = (IFunction_OwnedObject*)HeapAllocation;
#if TFUNCTION_USES_INLINE_STORAGE
            if (!Result) {
                Result = (IFunction_OwnedObject*)&InlineAllocation;
            }
#endif
            return Result;
        }

        /**
         * Returns a pointer to the callable object - needed by TFunctionRefBase.
         */
        FORCEINLINE void* GetPtr() const
        {
#if TFUNCTION_USES_INLINE_STORAGE
            IFunction_OwnedObject* Owned = (IFunction_OwnedObject*)HeapAllocation;
			if (!Owned) {
				Owned = (IFunction_OwnedObject*)&InlineAllocation;
			}
			return Owned->GetAddress();
#else
            return ((IFunction_OwnedObject*)HeapAllocation)->GetAddress();
#endif
        }

        /**
         * Destroy any owned bindings - called by TFunctionRefBase only if Bind() or BindCopy() was called.
         */
        FORCEINLINE void Unbind() {
            IFunction_OwnedObject* Owned = GetBoundObject();
            Owned->Destroy();
        }

        void* HeapAllocation;
#if TFUNCTION_USES_INLINE_STORAGE
        // Inline storage for an owned object
        TAlignedBytes<TFUNCTION_INLINE_SIZE, TFUNCTION_INLINE_ALIGNMENT> InlineAllocation{};
#endif
    };

    template <typename T, bool bOnHeap>
    struct TFunction_OwnedObject : public
#if TFUNCTION_USES_INLINE_STORAGE
        std::conditional_t<bOnHeap, IFunction_OwnedObject_OnHeap<T>, IFunction_OwnedObject_Inline<T>>
#else
        IFunction_OwnedObject_OnHeap<T>
#endif
    {
        template <typename... ArgTypes>
        FORCEINLINE explicit TFunction_OwnedObject(ArgTypes&&... Args)
                : Obj(Forward<ArgTypes>(Args)...) {
        }

        FORCEINLINE void* GetAddress() override {
            return &Obj;
        }
        T Obj;
    };

    /**
     * Implementation of IFunction_OwnedObject for a given copyable T.
     */
    template <typename T, bool bOnHeap>
    struct TFunction_CopyableOwnedObject final : public TFunction_OwnedObject<T, bOnHeap>
    {
        /**
         * Constructor which creates its T by copying.
         */
        FORCEINLINE explicit TFunction_CopyableOwnedObject(const T& InObj)
                : TFunction_OwnedObject<T, bOnHeap>(InObj) {
        }

        /**
         * Constructor which creates its T by moving.
         */
        FORCEINLINE explicit TFunction_CopyableOwnedObject(T&& InObj)
                : TFunction_OwnedObject<T, bOnHeap>(MoveTemp(InObj)) {
        }

        FORCEINLINE void* CloneToEmptyStorage(void* UntypedStorage) const override;
    };

    /**
     * Implementation of IFunction_OwnedObject for a given non-copyable T.
     */
    template <typename T, bool bOnHeap>
    struct TFunction_UniqueOwnedObject final : public TFunction_OwnedObject<T, bOnHeap>
    {
        /**
         * Constructor which creates its T by moving.
         */
        FORCEINLINE explicit TFunction_UniqueOwnedObject(T&& InObj)
                : TFunction_OwnedObject<T, bOnHeap>(MoveTemp(InObj)) {
        }

        void* CloneToEmptyStorage(void* Storage) const override {
            // Should never get here - copy functions are deleted for TUniqueFunction
            throw std::runtime_error{"Attempt to copy UniqueOwnedObject"};
        }
    };

    template <typename FunctorType, bool bUnique, bool bOnHeap>
    struct TStorageOwnerType;

    template <typename FunctorType, bool bOnHeap>
    struct TStorageOwnerType<FunctorType, true, bOnHeap> {
        using Type = TFunction_UniqueOwnedObject<std::decay_t<FunctorType>, bOnHeap>;
    };

    template <typename FunctorType, bool bOnHeap>
    struct TStorageOwnerType<FunctorType, false, bOnHeap> {
        using Type = TFunction_CopyableOwnedObject<std::decay_t<FunctorType>, bOnHeap>;
    };

    template <typename FunctorType, bool bUnique, bool bOnHeap>
    using TStorageOwnerTypeT = typename TStorageOwnerType<FunctorType, bUnique, bOnHeap>::Type;

    template <bool bUnique>
    struct TFunctionStorage : FFunctionStorage {
        FORCEINLINE TFunctionStorage() = default;

        FORCEINLINE TFunctionStorage(FFunctionStorage&& Other): FFunctionStorage(MoveTemp(Other)) {
        }

        template <typename FunctorType>
		FORCEINLINE typename std::decay_t<FunctorType>* Bind(FunctorType&& InFunc) {
			if (!IsBound(InFunc)) {
				return nullptr;
			}

#if TFUNCTION_USES_INLINE_STORAGE
			constexpr bool bUseInline = sizeof(TStorageOwnerTypeT<FunctorType, bUnique, false>) <= TFUNCTION_INLINE_SIZE;
#else
			constexpr bool bUseInline = false;
#endif
			using OwnedType = TStorageOwnerTypeT<FunctorType, bUnique, !bUseInline>;

			void* NewAlloc;
#if TFUNCTION_USES_INLINE_STORAGE
			if constexpr (bUseInline) {
				NewAlloc = &InlineAllocation;
			} else
#endif
			{
				NewAlloc = FMemory::Malloc(sizeof(OwnedType), alignof(OwnedType));
				HeapAllocation = NewAlloc;
			}

			auto* NewOwned = new (NewAlloc) OwnedType(Forward<FunctorType>(InFunc));
			return &NewOwned->Obj;
		}
    };

    template <typename T, bool bOnHeap>
    FORCEINLINE void* TFunction_CopyableOwnedObject<T, bOnHeap>::CloneToEmptyStorage(void* UntypedStorage) const {
        TFunctionStorage<false> &Storage = *(TFunctionStorage<false> *) UntypedStorage;

        void *NewAlloc;
#if TFUNCTION_USES_INLINE_STORAGE
        if constexpr(!bOnHeap) {
            NewAlloc = &Storage.InlineAllocation;
        } else
#endif
        {
            NewAlloc = FMemory::Malloc(sizeof(TFunction_CopyableOwnedObject), alignof(TFunction_CopyableOwnedObject));
            Storage.HeapAllocation = NewAlloc;
        }
        auto *NewOwned = new(NewAlloc) TFunction_CopyableOwnedObject(this->Obj);
        return &NewOwned->Obj;
    }

    template <typename FuncType>
    class TFunction final : public TFunctionRefBase<TFunctionStorage<false>, FuncType> {
        using Super = TFunctionRefBase<TFunctionStorage<false>, FuncType>;
    public:
        /**
         * Default constructor.
         */
        FORCEINLINE TFunction(std::nullptr_t = nullptr) {
        }

        /**
         * Constructor which binds a TFunction to any function object.
         */
        template<typename FunctorType, typename = std::enable_if_t<!TIsTFunctionRef<std::decay_t<FunctorType>>::Value && TFuncCanBindToFunctor<FuncType, FunctorType>::Value>>
        FORCEINLINE TFunction(FunctorType&& InFunc) : Super(Forward<FunctorType>(InFunc)) {
            // This constructor is disabled for TFunction types so it isn't incorrectly selected as copy/move constructors.

            // This is probably a mistake if you expect TFunction to take a copy of what
            // TFunctionRef is bound to, because that's not possible.
            //
            // If you really intended to bind a TFunction to a TFunctionRef, you can just
            // wrap it in a lambda (and thus it's clear you're just binding to a call to another
            // reference):
            //
            // TFunction<int32(float)> MyFunction = [MyFunctionRef](float F) { return MyFunctionRef(F); };
            static_assert(!TIsTFunctionRef<std::decay_t<FunctorType>>::Value, "Cannot construct a TFunction from a TFunctionRef");
        }

        TFunction(TFunction&&) = default;
        TFunction(const TFunction& Other) = default;
        ~TFunction() = default;

        /**
         * Move assignment operator.
         */
        TFunction& operator=(TFunction&& Other) {
            Swap(*this, Other);
            return *this;
        }

        /**
         * Copy assignment operator.
         */
        TFunction& operator=(const TFunction& Other) {
            TFunction Temp = Other;
            Swap(*this, Temp);
            return *this;
        }

        /**
         * Tests if the TFunction is callable.
         */
        FORCEINLINE explicit operator bool() const {
            return Super::IsSet();
        }
    };
}

#endif //RC_UNREAL_FUNCTION_HPP
