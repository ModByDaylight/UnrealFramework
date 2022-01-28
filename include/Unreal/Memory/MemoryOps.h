#ifndef UE4SS_MEMORYOPS_H
#define UE4SS_MEMORYOPS_H

#include <type_traits>
#include "Unreal/Memory/FMemory.hpp"

namespace RC::Unreal
{
    namespace UE4MemoryOps_Private
    {
        template<typename T>
        constexpr bool is_zero_construct_v = std::is_enum_v<T> || std::is_arithmetic_v<T> || std::is_pointer_v<T>;

        template <typename T, typename Arg>
        struct TIsBitwiseConstructible
        {
            static_assert(
                    !std::is_reference_v<T> &&
                    !std::is_reference_v<Arg>,
                    "TIsBitwiseConstructible is not designed to accept reference types");

            static_assert(
                    std::is_same_v<T, std::remove_cv_t<T>> &&
                    std::is_same_v<Arg, std::remove_cv_t<Arg>>,
                    "TIsBitwiseConstructible is not designed to accept qualified types");

            // Assume no bitwise construction in general
            enum { Value = false };
        };

        template <typename T>
        struct TIsBitwiseConstructible<T, T>
        {
            enum { Value = std::is_trivially_copy_constructible_v<T> };
        };

        template <typename T, typename U>
        struct TIsBitwiseConstructible<const T, U> : TIsBitwiseConstructible<T, U> {
        };

        template <typename T>
        struct TIsBitwiseConstructible<const T*, T*> {
            enum { Value = true };
        };

        template <> struct TIsBitwiseConstructible<uint8_t, int8_t>  { enum { Value = true }; };
        template <> struct TIsBitwiseConstructible<int8_t, uint8_t>  { enum { Value = true }; };
        template <> struct TIsBitwiseConstructible<uint16_t, int16_t> { enum { Value = true }; };
        template <> struct TIsBitwiseConstructible<int16_t, uint16_t> { enum { Value = true }; };
        template <> struct TIsBitwiseConstructible<uint32_t,  int32_t> { enum { Value = true }; };
        template <> struct TIsBitwiseConstructible< int32_t, uint32_t> { enum { Value = true }; };
        template <> struct TIsBitwiseConstructible<uint64_t,  int64_t> { enum { Value = true }; };
        template <> struct TIsBitwiseConstructible<int64_t, uint64_t> { enum { Value = true }; };

        template <typename DestinationElementType, typename SourceElementType>
        constexpr bool is_bitwise_relocatable_v = std::is_same_v<DestinationElementType, SourceElementType> ||
                                                  (TIsBitwiseConstructible<DestinationElementType, SourceElementType>::Value &&
                                                   std::is_trivially_destructible_v<SourceElementType>);

        template<typename T>
        constexpr bool is_bytewise_comparable_v = std::is_enum_v<T> || std::is_arithmetic_v<T> || std::is_pointer_v<T>;
    }

	template <typename ElementType, typename SizeType>
	inline void DefaultConstructItems(void* Address, SizeType Count)
	{
		if constexpr (UE4MemoryOps_Private::is_zero_construct_v<ElementType>)
		{
			FMemory::memset(Address, 0, sizeof(ElementType) * Count);
		}
		else
		{
			ElementType* Element = (ElementType*)Address;
			while (Count)
			{
				new (Element) ElementType;
				++Element;
				--Count;
			}
		}
	}

	template <typename ElementType>
	inline void DestructItem(ElementType* Element)
	{
		if constexpr (!std::is_trivially_destructible_v<ElementType>)
		{
			// We need a typedef here because VC won't compile the destructor call below if ElementType itself has a member called ElementType
			typedef ElementType DestructItemsElementTypeTypedef;

			Element->DestructItemsElementTypeTypedef::~DestructItemsElementTypeTypedef();
		}
	}

	template <typename ElementType, typename SizeType>
	inline void DestructItems(ElementType* Element, SizeType Count)
	{
		if constexpr (!std::is_trivially_destructible_v<ElementType>)
		{
			while (Count)
			{
				// We need a typedef here because VC won't compile the destructor call below if ElementType itself has a member called ElementType
				typedef ElementType DestructItemsElementTypeTypedef;

				Element->DestructItemsElementTypeTypedef::~DestructItemsElementTypeTypedef();
				++Element;
				--Count;
			}
		}
	}

	template <typename DestinationElementType, typename SourceElementType, typename SizeType>
	inline void ConstructItems(void* Dest, const SourceElementType* Source, SizeType Count)
	{
		if constexpr (UE4MemoryOps_Private::TIsBitwiseConstructible<DestinationElementType, SourceElementType>::Value)
		{
			FMemory::memcpy(Dest, Source, sizeof(SourceElementType) * Count);
		}
		else
		{
			while (Count)
			{
				new (Dest) DestinationElementType(*Source);
				++(DestinationElementType*&)Dest;
				++Source;
				--Count;
			}
		}
	}

	template <typename ElementType, typename SizeType>
	inline void CopyAssignItems(ElementType* Dest, const ElementType* Source, SizeType Count)
	{
		if constexpr (std::is_trivially_copy_assignable_v<ElementType>)
		{
			FMemory::memcpy(Dest, Source, sizeof(ElementType) * Count);
		}
		else
		{
			while (Count)
			{
				*Dest = *Source;
				++Dest;
				++Source;
				--Count;
			}
		}
	}

	template <typename DestinationElementType, typename SourceElementType, typename SizeType>
	inline void RelocateConstructItems(void* Dest, const SourceElementType* Source, SizeType Count)
	{
		if constexpr (UE4MemoryOps_Private::is_bitwise_relocatable_v<DestinationElementType, SourceElementType>)
		{
			FMemory::memmove(Dest, Source, sizeof(SourceElementType) * Count);
		}
		else
		{
			while (Count)
			{
				// We need a typedef here because VC won't compile the destructor call below if SourceElementType itself has a member called SourceElementType
				typedef SourceElementType RelocateConstructItemsElementTypeTypedef;

				new (Dest) DestinationElementType(*Source);
				++(DestinationElementType*&)Dest;
				(Source++)->RelocateConstructItemsElementTypeTypedef::~RelocateConstructItemsElementTypeTypedef();
				--Count;
			}
		}
	}

	template <typename ElementType, typename SizeType>
	inline void MoveConstructItems(void* Dest, const ElementType* Source, SizeType Count)
	{
		if constexpr (std::is_trivially_copy_constructible_v<ElementType>)
		{
			FMemory::memmove(Dest, Source, sizeof(ElementType) * Count);
		}
		else
		{
			while (Count)
			{
				new (Dest) ElementType((ElementType&&)*Source);
				++(ElementType*&)Dest;
				++Source;
				--Count;
			}
		}
	}

	template <typename ElementType, typename SizeType>
	inline void MoveAssignItems(ElementType* Dest, const ElementType* Source, SizeType Count)
	{
		if constexpr (std::is_trivially_copy_assignable_v<ElementType>)
		{
			FMemory::memmove(Dest, Source, sizeof(ElementType) * Count);
		}
		else
		{
			while (Count)
			{
				*Dest = (ElementType&&)*Source;
				++Dest;
				++Source;
				--Count;
			}
		}
	}

    template <typename ElementType, typename SizeType>
	inline bool CompareItems(const ElementType* A, const ElementType* B, SizeType Count)
	{
		if constexpr (UE4MemoryOps_Private::is_bytewise_comparable_v<ElementType>)
		{
			return !FMemory::memcmp(A, B, sizeof(ElementType) * Count);
		}
		else
		{
			while (Count)
			{
                if (*A == *B) {
                    ++A;
                    ++B;
                    --Count;
                } else {
                    return false;
                }
            }
			return true;
		}
	}
}

#endif //UE4SS_MEMORYOPS_H
