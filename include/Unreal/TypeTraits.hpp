#ifndef RC_UNREAL_TYPETRAITS_HPP
#define RC_UNREAL_TYPETRAITS_HPP

#include <type_traits>
#include <Unreal/TypeCompatibleBytes.hpp>
#include <Unreal/FMemory.hpp>

namespace RC::Unreal
{
    /**
     * Aligns a value to the nearest higher multiple of 'Alignment', which must be a power of two.
     *
     * @param  Val        The value to align.
     * @param  Alignment  The alignment value, must be a power of two.
     *
     * @return The value aligned up to the specified alignment.
     */
    template <typename T>
    FORCEINLINE constexpr T Align(T Val, uint64 Alignment) {
        static_assert(std::is_integral_v<T> || std::is_pointer_v<T>, "Align expects an integer or pointer type");
        return (T)(((uint64)Val + Alignment - 1) & ~(Alignment - 1));
    }

    /**
     * Equivalent to std::declval.
     *
     * Note that this function is unimplemented, and is only intended to be used in unevaluated contexts, like sizeof and trait expressions.
     */
    template <typename T>
    T&& DeclVal();

    /**
     * Uses implicit conversion to create an instance of a specific type.
     * Useful to make things clearer or circumvent unintended type deduction in templates.
     * Safer than C casts and static_casts, e.g. does not allow down-casts
     *
     * @param Obj  The object (usually pointer or reference) to convert.
     *
     * @return The object converted to the specified type.
     */
    template <typename T>
    FORCEINLINE T ImplicitConv(typename std::type_identity_t<T> Obj) {
        return Obj;
    }

    /**
     * MoveTemp will cast a reference to an rvalue reference.
     * This is UE's equivalent of std::move except that it will not compile when passed an rvalue or
     * const object, because we would prefer to be informed when MoveTemp will have no effect.
     */
    template <typename T>
    FORCEINLINE typename std::remove_reference_t<T>&& MoveTemp(T&& Obj) {
        typedef typename std::remove_reference_t<T> CastType;

        // Validate that we're not being passed an rvalue or a const object - the former is redundant, the latter is almost certainly a mistake
        static_assert(std::is_lvalue_reference_v<T>, "MoveTemp called on an rvalue");
        static_assert(!std::is_same_v<CastType&, const CastType&>, "MoveTemp called on a const object");

        return (CastType&&)Obj;
    }

    /**
     * MoveTemp will cast a reference to an rvalue reference.
     * This is UE's equivalent of std::move.  It doesn't static assert like MoveTemp, because it is useful in
     * templates or macros where it's not obvious what the argument is, but you want to take advantage of move semantics
     * where you can but not stop compilation.
     */
    template <typename T>
    FORCEINLINE typename std::remove_reference_t<T>&& MoveTempIfPossible(T&& Obj) {
        typedef typename std::remove_reference_t<T> CastType;
        return (CastType&&)Obj;
    }

    /**
     * CopyTemp will enforce the creation of an rvalue which can bind to rvalue reference parameters.
     * Unlike MoveTemp, the source object will never be modifed. (i.e. a copy will be made)
     * There is no std:: equivalent.
     */
    template <typename T>
    FORCEINLINE T CopyTemp(T& Val) {
        return const_cast<const T&>(Val);
    }

    template <typename T>
    FORCEINLINE T CopyTemp(const T& Val) {
        return Val;
    }

    template <typename T>
    FORCEINLINE T&& CopyTemp(T&& Val) {
        // If we already have an rvalue, just return it unchanged, rather than needlessly creating yet another rvalue from it.
        return MoveTemp(Val);
    }

    /**
     * Forward will cast a reference to an rvalue reference.
     * This is UE's equivalent of std::forward.
     */
    template <typename T>
    FORCEINLINE T&& Forward(typename std::remove_reference_t<T>& Obj) {
        return (T&&)Obj;
    }

    template <typename T>
    FORCEINLINE T&& Forward(typename std::remove_reference_t<T>&& Obj) {
        return (T&&)Obj;
    }

    /**
     * A traits class which specifies whether a Swap of a given type should swap the bits or use a traditional value-based swap.
     */
    template <typename T>
    struct TUseBitwiseSwap {
        // We don't use bitwise swapping for 'register' types because this will force them into memory and be slower.
        static constexpr bool Value = !(std::is_enum_v<T> || std::is_pointer_v<T> || std::is_arithmetic_v<T>);
    };

    /**
     * Swap two values.  Assumes the types are trivially relocatable.
     */
    template <typename T>
    inline typename std::enable_if_t<TUseBitwiseSwap<T>::Value> Swap(T& A, T& B) {
        if (&A != &B) {
            TTypeCompatibleBytes<T> Temp;
            FMemory::Memcpy(&Temp, &A, sizeof(T));
            FMemory::Memcpy(&A, &B, sizeof(T));
            FMemory::Memcpy(&B, &Temp, sizeof(T));
        }
    }

    template <typename T>
    inline typename std::enable_if_t<!TUseBitwiseSwap<T>::Value> Swap(T& A, T& B) {
        T Temp = MoveTemp(A);
        A = MoveTemp(B);
        B = MoveTemp(Temp);
    }

    template <typename T>
    inline void Exchange(T& A, T& B) {
        Swap(A, B);
    }

    template<typename A, typename B>
    struct TAreTypesEqual {
        static constexpr bool Value = std::is_same_v<A, B>;
    };

    template<typename T>
    struct TIsTriviallyDestructible {
        static constexpr bool Value = std::is_trivially_constructible_v<T>;
    };

    template<typename T>
    struct TIsTriviallyConstructable {
        static constexpr bool Value = std::is_trivially_constructible_v<T>;
    };

    template<typename T>
    struct TIsTriviallyCopyable {
        static constexpr bool Value = std::is_trivially_copyable_v<T>;
    };

    template<typename T>
    struct TIsBytewiseComparable {
        static constexpr bool Value = std::is_enum_v<T> || std::is_arithmetic_v<T> || std::is_pointer_v<T>;
    };
}

#endif //RC_UNREAL_TYPETRAITS_HPP
