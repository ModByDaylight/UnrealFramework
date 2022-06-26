#include <Unreal/NameTypes.hpp>
#include <Unreal/FString.hpp>

namespace RC::Unreal
{
    Function<void(const FName*, class FString&)> FName::ToStringInternal;
    Function<FName(const wchar_t*, EFindName)> FName::ConstructorInternal;

    FString FName::ToFString() const {
        if (!FName::ToStringInternal.is_ready()) {
            throw std::runtime_error{"FName::ToString was not ready but was called anyway"};
        }
        FString ResultString;
        FName::ToStringInternal(this, ResultString);

        return ResultString;
    }

    std::wstring FName::ToString() const {
        return *ToFString();
    }

    FString FName::GetPlainNameString() const {
        return FName(GetDisplayIndex()).ToFString();
    }
}
