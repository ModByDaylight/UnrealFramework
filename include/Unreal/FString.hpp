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
        auto CreateCache(UnrealInitializer::CacheInfo& Target) -> void;
        auto LoadCache(UnrealInitializer::CacheInfo& Target) -> void;
    }

    class RC_UE_API FString
    {
    protected:
        friend auto UnrealInitializer::CreateCache(UnrealInitializer::CacheInfo& Target) -> void;
        friend auto UnrealInitializer::LoadCache(UnrealInitializer::CacheInfo& Target) -> void;

    protected:
        TArray<TCHAR> Data{nullptr, 0, 0};

    public:
        FString() = default;
        explicit FString(wchar_t* Str);

        [[nodiscard]] auto GetCharArray() const -> const wchar_t*;
        auto Clear() -> void;
    };

    class FStringOut : public FString
    {
    public:
        FStringOut() = default;
        FStringOut(const FStringOut&); // Copy constructor
        FStringOut(FStringOut&&) noexcept;  // Move constructor
        ~FStringOut();
    };

}

#endif //RC_UNREAL_FSTRING_HPP
