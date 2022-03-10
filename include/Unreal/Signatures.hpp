#ifndef RC_UNREAL_SIGNATURES_HPP
#define RC_UNREAL_SIGNATURES_HPP

#include <string>
#include <unordered_set>

#include <Unreal/Common.hpp>
#include <Unreal/UnrealInitializer.hpp>

namespace RC::Unreal::Signatures
{
    enum ScanStatus
    {
        Failed,
        Succeeded,
    };

    struct RC_UE_API ScanError
    {
        std::string message;
        bool is_fatal{true};

        ScanError(std::string_view error) : message(error) {}
    };

    struct RC_UE_API ScanResult
    {
        std::vector<std::wstring> success_messages{};
        std::unordered_set<std::wstring> info_messages{};
        std::vector<ScanError> errors;
        ScanStatus scan_status{ScanStatus::Failed};
    };

    auto RC_UE_API scan_for_game_functions_and_data(const UnrealInitializer::Config&) -> ScanResult;
    auto RC_UE_API scan_for_guobjectarray(const UnrealInitializer::Config&) -> ScanResult;

    auto RC_UE_API scan_for_game_functions_and_data_impl(const UnrealInitializer::Config&) -> ScanResult;
    auto RC_UE_API scan_for_guobjectarray_impl(const UnrealInitializer::Config&) -> ScanResult;
}


#endif //RC_UNREAL_SIGNATURES_HPP
