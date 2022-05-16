#include <chrono>
#include <format>

#include <DynamicOutput/OutputDevice.hpp>

namespace RC::Output
{
    auto OutputDevice::has_optional_arg() const -> bool
    {
        return false;
    }

    auto OutputDevice::receive_with_optional_arg([[maybe_unused]]std::wstring_view fmt, [[maybe_unused]]int32_t optional_arg) const -> void
    {
        // This only exists to make it not required to implement
        // Most devices probably won't use this
        throw std::runtime_error{"OutputDevice::receive_with_optional_arg called but no derived implementation found"};
    }

    auto OutputDevice::set_formatter(Formatter new_formatter) -> void
    {
        m_formatter = new_formatter;
    }

    auto OutputDevice::get_now_as_string() -> const std::wstring
    {
        auto now = std::chrono::system_clock::now();
        const std::wstring when_as_string = std::format(STR("{:%Y-%m-%d %X}"), now);
        return when_as_string;
    }

    auto OutputDevice::default_format_string(std::wstring_view string_to_format) -> std::wstring
    {
        return std::format(STR("[{}] {}"), get_now_as_string(), string_to_format);
    }
}
