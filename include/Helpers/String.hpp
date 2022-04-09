#ifndef RC_STRING_HPP
#define RC_STRING_HPP

#include <string>
#include <vector>

namespace RC
{
    /* explode_by_occurrence -> START

    FUNCTION: explode_by_occurrence
    Find or explode a string by a delimiter

    The defaults for these functions are set in stone
    If you change them then you'll have to review every single usage in the entire solution
    */

    enum class ExplodeType
    {
        FromStart,
        FromEnd
    };

    auto inline explode_by_occurrence(const std::wstring& in_str_wide, const wchar_t delimiter, ExplodeType start_or_end) -> std::wstring
    {
        size_t occurrence = (start_or_end == ExplodeType::FromStart ? in_str_wide.find_first_of(delimiter) : in_str_wide.find_last_of(delimiter));

        std::wstring return_value;
        if (occurrence != std::wstring::npos)
        {
            return_value = start_or_end == ExplodeType::FromEnd ? in_str_wide.substr(occurrence + 1, std::wstring::npos) : in_str_wide.substr(0, occurrence);
        }
        else
        {
            return_value = in_str_wide;
        }

        return return_value;
    }

    auto inline explode_by_occurrence(const std::string& in_str, const char delimiter, ExplodeType start_or_end) -> std::string
    {
        size_t occurrence = (start_or_end == ExplodeType::FromStart ? in_str.find_first_of(delimiter) : in_str.find_last_of(delimiter));

        std::string return_value;
        if (occurrence != std::string::npos)
        {
            return_value = start_or_end == ExplodeType::FromEnd ? in_str.substr(occurrence + 1, std::string::npos) : in_str.substr(0, occurrence);
        }
        else
        {
            return_value = in_str;
        }

        return return_value;
    }

    auto inline explode_by_occurrence(const std::string& in_str, const char delimiter, const int32_t occurrence) -> std::string
    {
        size_t found_occurrence{};
        for (int64_t i = 0; i < std::count(in_str.begin(), in_str.end(), delimiter); i++)
        {
            found_occurrence = in_str.find(delimiter, found_occurrence + 1);
            if (i + 1 == occurrence) { return in_str.substr(0, found_occurrence); }
        }

        // No occurrence was found, returning empty string for now
        return {};
    }

    auto inline explode_by_occurrence(const std::string& in_str, const char delimiter) -> std::vector<std::string>
    {
        std::vector<std::string> result;

        size_t counter{};
        size_t start_offset{};

        for (const char* current_char = in_str.c_str(); *current_char; ++current_char)
        {
            if (*current_char == delimiter || counter == in_str.length() - 1)
            {
                std::string sub_str = in_str.substr(start_offset, counter - start_offset + (counter == in_str.length() - 1 ? 1 : 0));
                if (start_offset > 0)
                {
                    sub_str.erase(0, 1);
                }
                result.emplace_back(sub_str);

                start_offset = counter;
            }

            ++counter;
        }

        return result;
    }

    auto inline explode_by_occurrence(const std::wstring& in_str_wide, const wchar_t delimiter) -> std::vector<std::wstring>
    {
        std::vector<std::wstring> result;

        size_t counter{};
        size_t start_offset{};

        for (const wchar_t* current_char = in_str_wide.c_str(); *current_char; ++current_char)
        {
            if (*current_char == delimiter || counter == in_str_wide.length() - 1)
            {
                std::wstring sub_str = in_str_wide.substr(start_offset, counter - start_offset + (counter == in_str_wide.length() - 1 ? 1 : 0));
                if (start_offset > 0)
                {
                    sub_str.erase(0, 1);
                }
                result.emplace_back(sub_str);

                start_offset = counter;
            }

            ++counter;
        }

        return result;
    }

    auto inline explode_by_occurrence(const std::wstring& in_str, const wchar_t delimiter, const int32_t occurrence) -> std::wstring
    {
        size_t found_occurrence{};
        for (int64_t i = 0; i < std::count(in_str.begin(), in_str.end(), delimiter); i++)
        {
            found_occurrence = in_str.find(delimiter, found_occurrence + 1);
            if (i + 1 == occurrence) { return in_str.substr(0, found_occurrence); }
        }

        // No occurrence was found, returning empty string for now
        return {};
    }
    /* explode_by_occurrence -> END */

    auto inline to_wstring(std::string& input) -> std::wstring
    {
#pragma warning(disable: 4244)
        return std::wstring{input.begin(), input.end()};
#pragma warning(default: 4244)
    }

    auto inline to_wstring(std::string_view input) -> std::wstring
    {
#pragma warning(disable: 4244)
        return std::wstring{input.begin(), input.end()};
#pragma warning(default: 4244)
    }

    auto inline to_string(std::wstring& input) -> std::string
    {
#pragma warning(disable: 4244)
        return std::string{input.begin(), input.end()};
#pragma warning(default: 4244)
    }

    auto inline to_string(std::wstring_view input) -> std::string
    {
#pragma warning(disable: 4244)
        return std::string{input.begin(), input.end()};
#pragma warning(default: 4244)
    }
}

#endif //RC_STRING_HPP
