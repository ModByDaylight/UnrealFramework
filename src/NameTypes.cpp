#include <Unreal/NameTypes.hpp>
#include <Unreal/FString.hpp>

namespace RC::Unreal
{
    auto FName::ToString() -> std::wstring
    {
        if (!ToStringInternal.is_ready())
        {
            throw std::runtime_error{"FName::ToString was not ready but was called anyway"};
        }

        FStringOut string{};
        ToStringInternal(this, string);

        std::wstring name_string{string.GetCharArray() ? string.GetCharArray() : L"UE4SS_None"};
        return name_string;
    }

    uint32 FName::GetPlainNameString(TCHAR(&OutName)[NAME_SIZE])
    {
        const uint32 Entry = GetDisplayIndex();
        auto String = FName(Entry).ToString();
        std::memcpy(OutName, &String[0], String.size() * sizeof(File::StringType::size_type));
        return static_cast<uint32>(String.size());
    }
}
