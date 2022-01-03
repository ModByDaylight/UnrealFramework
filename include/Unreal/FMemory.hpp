#ifndef RC_UNREAL_FMEMORY_HPP
#define RC_UNREAL_FMEMORY_HPP

#include <Function/Function.hpp>

#include <Unreal/Common.hpp>

namespace RC::Unreal
{
    class RC_API FMemory
    {
    public:
        static inline Function<void(void*)> free{};
    };
}

#endif //RC_UNREAL_FMEMORY_HPP
