#ifndef RC_UNREAL_COMMON_HPP
#define RC_UNREAL_COMMON_HPP

#define FORCENOINLINE __declspec(noinline)
#define FORCEINLINE __forceinline
#define RESTRICT __restrict

#define NS_RC_UE_START namespace RC::Unreal {
#define NS_RC_UE_END };

#ifndef RC_EXPORT
#ifndef RC_UNREAL_BUILD_STATIC
#ifndef RC_UE_API
#define RC_UE_API __declspec(dllimport)
#endif
#else
#ifndef RC_UE_API
#define RC_UE_API
#endif
#endif
#else
#ifndef RC_UE_API
#define RC_UE_API __declspec(dllexport)
#endif
#endif

#endif //RC_UNREAL_COMMON_HPP
