#ifndef RC_UNREAL_UNREAL_VERSION_HPP
#define RC_UNREAL_UNREAL_VERSION_HPP

#include <cassert>
#include <utility>

#include <Helpers/ASM.hpp>
#include <Helpers/String.hpp>

#include <Unreal/UObject.hpp>
#include <Unreal/UObjectGlobals.hpp>

#define UNREAL_VERSION_RUN_TESTS 1

namespace RC::Unreal
{
    struct RC_API VersionStatus
    {
        enum StatusCode
        {
            SUCCESS,
            FAILURE,
        };

        std::wstring error_message{};
        StatusCode status_code{};

        VersionStatus() = default;
        VersionStatus(StatusCode status_code_param, std::wstring error_message_param) : status_code(status_code_param), error_message(std::move(error_message_param)) {}
    };

    class RC_API Version
    {
    public:
        static inline int32_t major{-1};
        static inline int32_t minor{-1};

        enum class VersionType
        {
            NEW,
            OLD
        };

        auto static setup_fallback() -> void;

        auto static setup_new(void* address) -> VersionStatus
        {
            uint8_t* call_destination = Helper::ASM::follow_call(static_cast<uint8_t*>(address) + 0x5);
            if (!call_destination)
            {
                return {VersionStatus::FAILURE, STR("Could not determine Unreal Engine version: The destination for the call instruction is nullptr.\n")};
            }

            uint8_t* jmp_destination = Helper::ASM::follow_jmp(call_destination);
            if (!jmp_destination)
            {
                return {VersionStatus::FAILURE, STR("Could not determine Unreal Engine version: The jmp instruction followed by the call instruction is nullptr.\n")};
            }

            std::wstring version_string = Helper::ASM::resolve_lea<wchar_t*>(jmp_destination);
            if (!version_string.data())
            {
                return {VersionStatus::FAILURE, STR("Could not determine Unreal Engine version: The lea instruction followed by the jmp instruction is nullptr.\n")};
            }

            // The version string looks like this: "++UE4+Release-4.26-CL-14830424"
            auto exploded_version_string_p1 = explode_by_occurrence(version_string, L'-');
            if (exploded_version_string_p1.size() < 2)
            {
                return {VersionStatus::FAILURE, STR("Could not determine Unreal Engine version: The version string has an invalid or unknown format.\n")};
            }

            auto exploded_version_string_p2 = explode_by_occurrence(exploded_version_string_p1[1], L'.');
            if (exploded_version_string_p2.size() < 2)
            {
                return {VersionStatus::FAILURE, STR("Could not determine Unreal Engine version: The version string has an invalid or unknown format.\n")};
            }

            try
            {
                major = stoi(exploded_version_string_p2[0]);
                minor = stoi(exploded_version_string_p2[1]);
            }
            catch (std::out_of_range&)
            {
                return {VersionStatus::FAILURE, STR("Could not determine Unreal Engine version: stoi threw std::out_of_range.\n")};
            }
            catch (std::invalid_argument&)
            {
                return {VersionStatus::FAILURE, STR("Could not determine Unreal Engine version: stoi threw std::invalid_argument.\n")};
            }

            if (is_below(4, 12))
            {
                return {VersionStatus::FAILURE, std::format(STR("Detected unsupported Unreal Engine version: {}.{}\n"), major, minor)};
            }

            return {VersionStatus::SUCCESS, STR("")};
        }

        auto static setup_old(void* address) -> VersionStatus
        {
            major = *(static_cast<uint8_t*>(address) + 0x6);
            minor = *(static_cast<uint8_t*>(address) + 0x8);

            return {VersionStatus::SUCCESS, STR("")};
        }

        auto static setup(void* address, VersionType type) -> VersionStatus
        {
#if UNREAL_VERSION_RUN_TESTS == 1
            run_tests();
#endif

            VersionStatus version_status;
            if (type == VersionType::NEW)
            {
                version_status = setup_new(address);
            }
            else if (type == VersionType::OLD)
            {
                version_status = setup_old(address);
            }

            return version_status;
        }

        [[nodiscard]]
        auto static is_equal(int32_t major_p, int32_t minor_p) -> bool { return (major == major_p && minor == minor_p); }

        [[nodiscard]]
        auto static is_atleast(int32_t major_p, int32_t minor_p) -> bool { return ((major > major_p) || (major == major_p && minor >= minor_p)); }

        [[nodiscard]]
        auto static is_atmost(int32_t major_p, int32_t minor_p) -> bool { return ((major < major_p) || (major == major_p && minor <= minor_p)); }

        [[nodiscard]]
        auto static is_below(int32_t major_p, int32_t minor_p) -> bool { return ((major < major_p) || (major == major_p && minor < minor_p)); }

        [[nodiscard]]
        auto static is_above(int32_t major_p, int32_t minor_p) -> bool { return ((major > major_p) || (major == major_p && minor > minor_p)); }

#if UNREAL_VERSION_RUN_TESTS == 1
        auto static run_tests() -> void
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
    };
}


#endif //RC_UNREAL_UNREAL_VERSION_HPP
