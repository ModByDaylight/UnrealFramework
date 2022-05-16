#ifndef RC_UNREAL_COMMON_HPP
#define RC_UNREAL_COMMON_HPP

#define FORCENOINLINE __declspec(noinline)
#define FORCEINLINE __forceinline
#define RESTRICT __restrict

#define check(Expression)
#define checkSlow(Expression)
#define checkf(Expression, Message, ...)
#define checkfSlow(Expression, Message, ...)

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
