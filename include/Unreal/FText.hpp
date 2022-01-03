#ifndef RC_UNREAL_FTEXT_HPP
#define RC_UNREAL_FTEXT_HPP

#include <string>

#include <Unreal/Common.hpp>

namespace RC::Unreal
{
    class FString;

    struct RC_API TTextData
    {
        void* vtable{};
        FString* string{};
    };

    class RC_API FText
    {
    public:
        TTextData* m_text_data{nullptr};

    public:
        auto to_string_raw() -> FString*;
        auto to_string() -> std::wstring;
        auto set_string(FString*) -> void;
    };
}

#endif //RC_UNREAL_FTEXT_HPP
