#ifndef RC_UNREAL_UNREAL_VERSION_HPP
#define RC_UNREAL_UNREAL_VERSION_HPP

#include <cassert>
#include <utility>
#include <format>

#include <Helpers/ASM.hpp>
#include <Helpers/String.hpp>
#include <File/File.hpp>

#include <Unreal/UObjectGlobals.hpp>

#define UNREAL_VERSION_RUN_TESTS 1

namespace RC::Unreal
{
    namespace UnrealInitializer
    {
        struct Config;
    }

    struct RC_UE_API VersionStatus
    {
        enum StatusCode
        {
            SUCCESS,
            FAILURE,
        };

        std::wstring error_message{};
        StatusCode status_code{};

        VersionStatus() = default;
        VersionStatus(StatusCode status_code_param, std::wstring error_message_param);
    };

    class RC_UE_API Version
    {
    public:
        static int32_t major;
        static int32_t minor;

        auto static setup(const UnrealInitializer::Config& config, void* address) -> VersionStatus;

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
        auto static run_tests() -> void;
#endif
    };
}


#endif //RC_UNREAL_UNREAL_VERSION_HPP
