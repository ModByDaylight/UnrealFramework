#ifndef RC_UNREAL_UNREAL_VERSION_HPP
#define RC_UNREAL_UNREAL_VERSION_HPP

#include <Unreal/Common.hpp>
#include <Unreal/PrimitiveTypes.hpp>

namespace RC::Unreal
{
    class RC_UE_API Version {
    public:
        static constexpr int32 Major = UNREAL_VERSION / 1000;
        static constexpr int32 Minor = (UNREAL_VERSION % 1000) / 10;
        static constexpr int32 Patch = (UNREAL_VERSION % 1000) % 10;

        constexpr static bool IsEqual(int32_t major_p, int32_t minor_p) {
            return (Major == major_p && Minor == minor_p);
        }

        constexpr static bool IsAtLeast(int32_t major_p, int32_t minor_p) {
            return ((Major > major_p) || (Major == major_p && Minor >= minor_p));
        }

        constexpr static bool IsAtMost(int32_t major_p, int32_t minor_p) {
            return ((Major < major_p) || (Major == major_p && Minor <= minor_p));
        }

        constexpr static bool IsBelow(int32_t major_p, int32_t minor_p) {
            return ((Major < major_p) || (Major == major_p && Minor < minor_p));
        }

        constexpr static bool IsAbove(int32_t major_p, int32_t minor_p) {
            return ((Major > major_p) || (Major == major_p && Minor > minor_p));
        }
    };
}


#endif //RC_UNREAL_UNREAL_VERSION_HPP
