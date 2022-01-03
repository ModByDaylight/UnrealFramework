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
}
