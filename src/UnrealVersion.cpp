#include <Unreal/UnrealVersion.hpp>
#include <Unreal/UClass.hpp>

namespace RC::Unreal
{
    int32_t Version::major{-1};
    int32_t Version::minor{-1};

    VersionStatus::VersionStatus(StatusCode status_code_param, std::wstring error_message_param) : status_code(status_code_param), error_message(std::move(error_message_param)) {}

    auto Version::setup(const UnrealInitializer::Config& config, void* address) -> VersionStatus
    {
        uint16_t major_version = Helper::Casting::ptr_cast_deref<uint16_t>(address);
        uint16_t minor_version = Helper::Casting::ptr_cast_deref<uint16_t>(address, 0x2);
        uint16_t patch_version = Helper::Casting::ptr_cast_deref<uint16_t>(address, 0x4);

        if (major_version < 4 || major_version > 5)
        {
            return {VersionStatus::FAILURE, std::format(STR("Could not determine Unreal Engine version: major version was too big or too small ({})"), major_version)};
        }

        if (minor_version > 30)
        {
            return {VersionStatus::FAILURE, std::format(STR("Could not determine Unreal Engine version: minor version was too big ({})"), minor_version)};
        }

        if (patch_version > 10)
        {
            return {VersionStatus::FAILURE, std::format(STR("Could not determine Unreal Engine version: patch version was too big ({})"), patch_version)};
        }

        const File::CharType* branch = Helper::Casting::ptr_cast_deref<const File::CharType*>(address, 0x10);
        void* branch_test = Helper::Casting::ptr_cast_deref_safe<void*>(branch, 0x0, config.process_handle);
        if (!branch_test)
        {
            return {VersionStatus::FAILURE, std::format(STR("Could not determine Unreal Engine version: 'Branch' member variable FString data was nullptr"))};
        }

        auto branch_view = File::StringViewType{branch};
        if (!branch_view.starts_with(STR("++")) &&
            !branch_view.starts_with(STR("UE4")))
        {
            return {VersionStatus::FAILURE, std::format(STR("Could not determine Unreal Engine version: 'Branch' member variable FString does not start with '++'"))};
        }

        major = static_cast<int32_t>(major_version);
        minor = static_cast<int32_t>(minor_version);

#if UNREAL_VERSION_RUN_TESTS == 1
        run_tests();
#endif

        return {VersionStatus::SUCCESS, STR("")};
    }

#if UNREAL_VERSION_RUN_TESTS == 1
    auto Version::run_tests() -> void
    {
        int32_t stashed_major = major;
        int32_t stashed_minor = minor;

        major = 4;
        minor = 25;

        if (!is_equal(4, 25))
        {
            throw std::runtime_error{"[UnrealVersion] Failed test #1 - !is_equal(4, 25) || Test ran with major = 4 and minor = 25"};
        }

        if (!is_atleast(4, 24))
        {
            throw std::runtime_error{"[UnrealVersion] Failed test #2 - !is_atleast(4, 24) || Test ran with major = 4 and minor = 25"};
        }

        if (is_atmost(4, 24))
        {
            throw std::runtime_error{"[UnrealVersion] Failed test #3 - is_atmost(4, 24) || Test ran with major = 4 and minor = 25"};
        }

        if (is_below(4, 24))
        {
            throw std::runtime_error{"[UnrealVersion] Failed test #4 - is_below(4, 24) || Test ran with major = 4 and minor = 25"};
        }

        if (!is_above(4, 24))
        {
            throw std::runtime_error{"[UnrealVersion] Failed test #5 - !is_above(4, 24) || Test ran with major = 4 and minor = 25"};
        }

        major = stashed_major;
        minor = stashed_minor;
    }
#endif
}
