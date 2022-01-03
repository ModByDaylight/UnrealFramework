#ifndef RC_UNREAL_COMMON_HPP
#define RC_UNREAL_COMMON_HPP

#ifndef RC_EXPORT
#ifndef RC_API
#define RC_API
#endif
#else
#ifndef RC_API
#define RC_API __declspec(dllexport)
#endif
#endif

#endif //RC_UNREAL_COMMON_HPP
