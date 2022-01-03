#include <Unreal/FText.hpp>
#include <Unreal/FString.hpp>
#include <stdexcept>

namespace RC::Unreal
{
    auto FText::to_string_raw() -> FString*
    {
        return m_text_data->string;
    }

    auto FText::to_string() -> std::wstring
    {
        if (!m_text_data || !m_text_data->string)
        {
            return STR("");
        }
        else
        {
            return std::wstring{m_text_data->string->get_string_data()};
        }
    }

    auto FText::set_string(FString* new_string) -> void
    {
        m_text_data->string = new_string;
    }
}
