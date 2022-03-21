#ifndef RC_UNREAL_FTEXT_HPP
#define RC_UNREAL_FTEXT_HPP

#include <string>

#include <Unreal/Common.hpp>

namespace RC::Unreal
{
    class FString;

    struct RC_UE_API TTextData
    {
        void* vtable{};
        FString* Data{};
    };

    class RC_UE_API FText
    {
    public:
        TTextData* Data{nullptr};

    public:
        auto ToFString() -> FString*;
        auto ToString() -> std::wstring;
        auto SetString(FString*) -> void;
    };
}

#endif //RC_UNREAL_FTEXT_HPP
