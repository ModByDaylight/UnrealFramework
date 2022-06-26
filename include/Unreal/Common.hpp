#ifndef RC_UNREAL_COMMON_HPP
#define RC_UNREAL_COMMON_HPP

#include <cassert>
#include "Helpers/Format.hpp"

#define DO_GUARD_SLOW 1
#define DO_CHECK 1

#define FORCENOINLINE __declspec(noinline)
#define FORCEINLINE __forceinline
#define RESTRICT __restrict

#ifdef DO_GUARD_SLOW
#define checkSlow(Expression) check(Expression)
#define checkfSlow(Expression, Message, ...) checkf(Expression, Message, ## __VA_ARGS__);
#else
#define checkSlow(Expression) (Expression);
#define checkfSlow(Expression, Message, ...) (Expression);
#endif

#if !defined(NDEBUG) && defined(DO_CHECK)
#define check(Expression) \
    if (!(Expression)) { \
        _wassert(STR(#Expression), STR("") __FILE__, __LINE__); \
    }

#define checkf(Expression, Message, ...) \
    if (!(Expression)) { \
        _wassert(RC::fmt(Message, ## __VA_ARGS__).c_str(), STR("") __FILE__, __LINE__); \
    }

#define ensure(Expression) \
    ((Expression) || [](){ \
        _wassert(STR(#Expression), STR("") __FILE__, __LINE__); \
        return false; \
    }())

#define ensuref(Expression, Message, ...) \
    ((Expression) || [](){ \
        _wassert(RC::fmt(Message, ## __VA_ARGS__).c_str(), STR("") __FILE__, __LINE__); \
        return false; \
    }())

#else
#define check(Expression) (Expression);
#define checkf(Expression, Message, ...) (Expression);
#define ensure(Expression) (Expression);
#define ensuref(Expression, Message, ...) (Expression);
#endif

#ifndef UNREAL_VERSION
    #error "UNREAL_VERSION is not defined by the Build System"
#endif

#ifndef RC_UNREAL_BUILD_STATIC
    #ifdef RC_UNREAL_EXPORTS
        #define RC_UE_API __declspec(dllexport)
    #else
        #define RC_UE_API __declspec(dllimport)
    #endif
#else
    #define RC_UE_API
#endif

#define NS_RC_UE_START namespace RC::Unreal {
#define NS_RC_UE_END };
#define USING_NS_RC_UE using namespace RC::Unreal;

#endif //RC_UNREAL_COMMON_HPP
