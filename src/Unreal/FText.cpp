#include <Unreal/FText.hpp>
#include <Unreal/FString.hpp>
#include <stdexcept>

namespace RC::Unreal
{
    auto FText::ToFString() -> FString*
    {
        return Data->Data;
    }

    auto FText::ToString() -> std::wstring
    {
        if (!Data || !Data->Data)
        {
            return STR("");
        }
        else
        {
            return std::wstring{Data->Data->GetCharArray()};
        }
    }

    auto FText::SetString(FString* new_string) -> void
    {
        Data->Data = new_string;
    }
}
