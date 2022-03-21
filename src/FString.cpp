#define NOMINMAX

#include <Unreal/FString.hpp>
#include <Unreal/FMemory.hpp>

namespace RC::Unreal
{
    FString::FString(wchar_t* Str) : Data(TArray<wchar_t>(Str, 0, 0))
    {
        size_t StrLength = wcslen(Str);

        // IDE, how the heck is this condition always true ???
        if (StrLength > std::numeric_limits<int32_t>::max())
        {
            throw std::runtime_error{"Tried to construct an FString with a size larger than int32"};
        }

        int32_t SafeStrLength = static_cast<int32_t>(StrLength);
        Data.SetMax(SafeStrLength);
        Data.SetNum(SafeStrLength);
    }

    FStringOut::FStringOut(const FStringOut& Other)
    {
        Data = Other.Data;
    }

    FStringOut::FStringOut(FStringOut&& Other) noexcept
    {
        Data = Other.Data;

        // Set the data pointer to nullptr so that the destructor of the old object won't free the memory
        // This is because the data pointer is still valid & in use
        // Set array_num to 0 to prevent accidentally accessing invalid memory
        Other.Data.SetNum(0);
        Other.Data.SetDataPtr(nullptr);
    }

    auto FString::GetCharArray() const -> const wchar_t*
    {
        return Data.Num() ? Data.GetDataPtr() : STR("");
    }

    auto FString::Clear() -> void
    {
        Data.SetNum(0);
        Data.SetMax(0);
        Data.SetDataPtr(nullptr);
    }

    FStringOut::~FStringOut() = default;
}