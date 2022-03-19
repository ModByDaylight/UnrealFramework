#include <Unreal/NameTypes.hpp>
#include <Unreal/FString.hpp>

namespace RC::Unreal
{
    auto FName::to_string() -> std::wstring
    {
        if (!to_string_internal.is_ready())
        {
            throw std::runtime_error{"FName::ToString was not ready but was called anyway"};
        }

        FStringOut string{};
        to_string_internal(this, string);

        std::wstring name_string{string.get_string_data() ? string.get_string_data() : L"UE4SS_None"};
        return name_string;
    }

    uint32 FName::GetPlainNameString(TCHAR(&OutName)[NAME_SIZE])
    {
        const uint32 Entry = get_display_index();
        auto String = FName(Entry).to_string();
        std::memcpy(OutName, &String[0], String.size() * sizeof(File::StringType::size_type));
        return static_cast<uint32>(String.size());
    }
}
