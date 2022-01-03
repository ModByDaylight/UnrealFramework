#ifndef RC_UNREAL_FSTRING_HPP
#define RC_UNREAL_FSTRING_HPP

#include <Function/Function.hpp>

#include <Unreal/Common.hpp>
#include <Unreal/TArray.hpp>

namespace RC::Unreal
{
    namespace UnrealInitializer
    {
        struct CacheInfo;
        auto create_cache(UnrealInitializer::CacheInfo& cache_info) -> void;
        auto load_cache(UnrealInitializer::CacheInfo& cache_info) -> void;
    }

    class RC_API FString
    {
    protected:
        friend auto UnrealInitializer::create_cache(UnrealInitializer::CacheInfo& cache_info) -> void;
        friend auto UnrealInitializer::load_cache(UnrealInitializer::CacheInfo& cache_info) -> void;

    protected:
        TArray<wchar_t> m_str_data{nullptr, 0, 0};

    public:
        FString() = default;
        explicit FString(wchar_t* str);

        [[nodiscard]] auto get_string_data() const -> const wchar_t*;
        auto clear() -> void;
    };

    class FStringOut : public FString
    {
    public:
        FStringOut() = default;
        FStringOut(const FStringOut&); // Copy constructor
        FStringOut(FStringOut&&) noexcept;  // Move constructor
        ~FStringOut();

    protected:
        auto unreal_destruct_impl() -> void;
    };

}

#endif //RC_UNREAL_FSTRING_HPP
