#define NOMINMAX

#include <Unreal/FString.hpp>
#include <Unreal/FMemory.hpp>

namespace RC::Unreal
{
    FString::FString(wchar_t* str) : m_str_data(TArray<wchar_t>(str, 0, 0))
    {
        size_t str_length = wcslen(str);

        // IDE, how the heck is this condition always true ???
        if (str_length > std::numeric_limits<int32_t>::max())
        {
            throw std::runtime_error{"Tried to construct an FString with a size larger than int32"};
        }

        int32_t safe_str_length = static_cast<int32_t>(str_length);
        m_str_data.set_array_max(safe_str_length);
        m_str_data.set_array_num(safe_str_length);
    }

    FStringOut::FStringOut(const FStringOut& other)
    {
        m_str_data = other.m_str_data;
    }

    FStringOut::FStringOut(FStringOut&& other) noexcept
    {
        m_str_data = other.m_str_data;

        // Set the data pointer to nullptr so that the destructor of the old object won't free the memory
        // This is because the data pointer is still valid & in use
        // Set array_num to 0 to prevent accidentally accessing invalid memory
        other.m_str_data.set_array_num(0);
        other.m_str_data.set_data_ptr(nullptr);
    }

    auto FString::get_string_data() const -> const wchar_t*
    {
        return m_str_data.get_array_num() ? m_str_data.get_data_ptr() : STR("");
    }

    auto FString::clear() -> void
    {
        m_str_data.set_array_num(0);
        m_str_data.set_array_max(0);
        m_str_data.set_data_ptr(nullptr);
    }

    FStringOut::~FStringOut() = default;
}