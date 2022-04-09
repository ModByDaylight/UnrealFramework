#ifndef RC_SINGLE_PASS_SIG_SCANNER_COMMON_HPP
#define RC_SINGLE_PASS_SIG_SCANNER_COMMON_HPP

#ifndef RC_SIG_SCANNER_EXPORTS
#ifndef RC_SIG_SCANNER_BUILD_STATIC
#ifndef RC_SIG_SCANNER_API
#define RC_SIG_SCANNER_API __declspec(dllimport)
#endif
#else
#define RC_SIG_SCANNER_API
#endif
#else
#ifndef RC_SIG_SCANNER_API
#define RC_SIG_SCANNER_API __declspec(dllexport)
#endif
#endif

#endif //RC_SINGLE_PASS_SIG_SCANNER_COMMON_HPP
