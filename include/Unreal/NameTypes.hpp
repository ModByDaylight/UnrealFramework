#ifndef RC_UNREAL_NAMETYPES_HPP
#define RC_UNREAL_NAMETYPES_HPP

#include <string>

#include <Function/Function.hpp>
#include <Unreal/Common.hpp>
#include <Unreal/PrimitiveTypes.hpp>

namespace RC::Unreal
{
    namespace UnrealInitializer
    {
        struct CacheInfo;
        RC_UE_API auto create_cache(UnrealInitializer::CacheInfo& cache_info) -> void;
    }

    enum {NAME_SIZE	= 1024};

    enum EFindName
    {
        FNAME_Find,
        FNAME_Add,
        FNAME_Replace_Not_Safe_For_Threading
    };

    // TODO:   Figure out what's going on here
    //         It shouldn't be required to use 'alignas' here to make sure it's aligned properly in containers (like TArray)
    //         I've never seen an FName not be 8-byte aligned in memory,
    //         but it is 4-byte aligned in the source so hopefully this doesn't cause any problems
    // UPDATE: This matters in the UE VM, when ElementSize is 0xC in memory for case-preserving games, it must be aligned by 0x4 in that case
#pragma warning(disable: 4324) // Suppressing warning about struct alignment
#ifdef WITH_CASE_PRESERVING_NAME
    struct alignas(4) RC_UE_API FName
#else
    struct alignas(8) RC_UE_API FName
#endif
    {
    private:
        friend auto UnrealInitializer::create_cache(UnrealInitializer::CacheInfo& cache_info) -> void;

        uint32_t comparison_index{};
#ifdef WITH_CASE_PRESERVING_NAME
        uint32_t display_index{};
#endif
        uint32_t number{};

    public:
        static inline Function<void(FName*, class FStringOut&)> to_string_internal{};
        static inline Function<FName(const wchar_t*, EFindName)> constructor_internal{};

    private:
        auto construct_with_string(const wchar_t* str_name, EFindName FindType, void* function_address_override) -> void
        {
            if (!constructor_internal.is_ready() && !function_address_override) { return; }

            // Assign the temporary address if one exists
            if (function_address_override) { constructor_internal.assign_temp_address(function_address_override); }

            FName name = constructor_internal(str_name, FindType);
            comparison_index = name.comparison_index;
#ifdef WITH_CASE_PRESERVING_NAME
            display_index = name.display_index;
#endif
            number = name.number;

            // Reset the address to what it was before it was overridden by a temporary address
            if (function_address_override) { constructor_internal.reset_address(); }
        }

        auto construct_with_string(const wchar_t* Name, uint32 InNumber, EFindName FindType, void* FunctionAddressOverride) -> void
        {
            construct_with_string(Name, FindType, FunctionAddressOverride);
            number = InNumber;
        }

    public:
        FName() = default;

        // Construct from an existing FName without looking up
        // Safe to pass to Unreal Engine internals
        // Not safe to use for return values from Unreal Engine internals
        explicit FName(int64_t index_and_number)
        {
            // Split the 64-bit integer into two 32-bit integers
            number = (index_and_number & 0xFFFFFFFF00000000LL) >> 32;
            comparison_index = (index_and_number & 0xFFFFFFFFLL);
#ifdef WITH_CASE_PRESERVING_NAME
            display_index = comparison_index;
#endif
        }

        // Construct from an existing FName without looking up
        // Safe to pass to Unreal Engine internals
        // Not safe to use for return values from Unreal Engine internals
        explicit FName(uint32_t index_param, uint32_t number_param)
        {
            comparison_index = index_param;
#ifdef WITH_CASE_PRESERVING_NAME
            display_index = index_param;
#endif
            number = number_param;
        }

#ifdef WITH_CASE_PRESERVING_NAME
        // Construct from an existing FName without looking up
        // Safe to pass to Unreal Engine internals
        // Not safe to use for return values from Unreal Engine internals
        explicit FName(uint32_t index_param, uint32_t display_index_param, uint32_t number_param)
        {
            comparison_index = index_param;
            display_index = display_index_param;
            number = number_param;
        }
#endif

        // Lookup & create from an existing FName
        // Not safe to pass to Unreal Engine internals
        // Safe to use for return values from Unreal Engine internals
        explicit FName(const wchar_t* str_name, EFindName FindType = FNAME_Find, void* function_address_override = nullptr)
        {
            construct_with_string(str_name, FindType, function_address_override);
        }

        explicit FName(std::wstring_view str_name, EFindName FindType = FNAME_Find, void* function_address_override = nullptr)
        {
            construct_with_string(str_name.data(), FindType, function_address_override);
        }

        explicit FName(std::wstring_view Name, uint32 InNumber, EFindName FindType = FNAME_Find, void* FunctionAddressOverride = nullptr)
        {
            construct_with_string(Name.data(), InNumber, FindType, FunctionAddressOverride);
        }

        auto inline operator==(FName other) const -> bool
        {
            return (comparison_index == other.comparison_index) & (number == other.number);
        }

        auto inline operator==(const wchar_t* other) -> bool
        {
            return to_string() == other;
        }

        auto inline operator!=(FName other) const -> bool
        {
            return !(*this == other);
        }

        auto inline operator!() const -> bool
        {
            return comparison_index == 0 && number == 0;
        }

        // Returns whether the ComparisonIndex is equal
        // Use this when you don't care for an identical match
        // The operator overloads will make sure both ComparisonIndex and Number are equal
        [[nodiscard]] auto equals(const FName& other) const -> bool
        {
            return comparison_index == other.comparison_index;
        }

        auto to_string() -> std::wstring;
        uint32 GetPlainNameString(TCHAR(&OutName)[NAME_SIZE]);

        [[nodiscard]] auto get_comparison_index() const -> uint32_t { return comparison_index; }
        [[nodiscard]] auto get_display_index() const -> uint32_t
        {
#ifdef WITH_CASE_PRESERVING_NAME
            return display_index;
#else
            return comparison_index;
#endif
        }
        [[nodiscard]] auto get_number() const -> uint32_t { return number; }
    };
#pragma warning(default: 4324)
}

namespace std
{
    template<>
    struct hash<RC::Unreal::FName>
    {
        auto operator()(const RC::Unreal::FName& fname) const -> size_t
        {
            size_t comparison_index_hash = hash<uint32_t>()(fname.get_comparison_index());
            size_t number_hash = hash<uint32_t>()(fname.get_number());
            return comparison_index_hash ^ number_hash;
        }
    };
}

#endif //RC_UNREAL_NAMETYPES_HPP
