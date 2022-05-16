#ifndef RC_FILE_MACROS_HPP
#define RC_FILE_MACROS_HPP


#define STR(str) L##str
#define S(str) STR(str)

#define THROW_INTERNAL_FILE_ERROR(msg) \
                                   RC::File::Internal::StaticStorage::internal_error = true;\
                                   throw std::runtime_error{msg};

// This macro needs to be moved into the DynamicOutput system
/*
#define ASSERT_DEFAULT_OUTPUT_DEVICE_IS_VALID(param_device) \
                if (DefaultTargets::get_default_devices_ref().empty() || !param_device){                       \
                THROW_INTERNAL_OUTPUT_ERROR("[Output::send] Attempted to send but there were no default devices. Please specify at least one default device or construct a Targets object and supply your own devices.") \
                }
//*/


#include <string>

namespace RC::File
{
    using StringType = std::wstring;
    using StringViewType = std::wstring_view;
    using CharType = wchar_t;
    using StreamType = std::wifstream;

    constexpr auto ToString = [](auto&& numeric_value) constexpr -> decltype(auto) { return std::to_wstring(std::forward<decltype(numeric_value)>(numeric_value)); };
}

namespace RC
{
    using StringType = std::wstring;
    using StringViewType = std::wstring_view;
    using CharType = File::CharType;
    using StreamType = File::StreamType;

    constexpr auto ToString = File::ToString;
}

inline std::wstring to_wstring(const std::string& string) {
    std::wstring resultString;

    size_t NumOfCharsConverted = 0;
    mbstowcs_s(&NumOfCharsConverted, nullptr, 0, string.data(), _TRUNCATE);

    resultString.append(NumOfCharsConverted, '\0');
    mbstowcs_s(nullptr, resultString.data(), (resultString.size() + 1), string.data(), _TRUNCATE);

    return resultString;
}

#endif //RC_FILE_MACROS_HPP
