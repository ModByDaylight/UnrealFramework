#ifndef UE4SS_UNREALSTRINGCONVERSIONS_H
#define UE4SS_UNREALSTRINGCONVERSIONS_H

#include <Unreal/Common.hpp>
#include <cstdlib>

inline void lex_from_string(int8_t& out_value, 		const wchar_t* buffer)	{ out_value = (int8_t) _wtoi(buffer);	}
inline void lex_from_string(int16_t& out_value,		const wchar_t* buffer)	{ out_value = (int16_t) _wtoi(buffer);	}
inline void lex_from_string(int32_t& out_value,		const wchar_t* buffer)	{ out_value = (int32_t) _wtoi(buffer);	}
inline void lex_from_string(int64_t& out_value,		const wchar_t* buffer)	{ out_value = _wtoi64(buffer);	}
inline void lex_from_string(uint8_t& out_value,		const wchar_t* buffer)	{ out_value = (uint8_t) _wtoi(buffer);	}
inline void lex_from_string(uint16_t& out_value, 	    const wchar_t* buffer)	{ out_value = (uint16_t) _wtoi(buffer); }
inline void lex_from_string(uint32_t& out_value, 	    const wchar_t* buffer)	{ out_value = (uint32_t) _wtoi64(buffer); }
inline void lex_from_string(uint64_t& out_value, 	    const wchar_t* buffer)	{ out_value = _wcstoui64(buffer, nullptr, 0); }
inline void lex_from_string(float& out_value,		    const wchar_t* buffer)	{ out_value = (float) _wtof(buffer); }
inline void lex_from_string(double& out_value, 	    const wchar_t* buffer)	{ out_value = _wtof(buffer); }
inline void lex_from_string(bool& out_value, 		    const wchar_t* buffer)	{ out_value = !!_wtoi(buffer); }
inline void lex_from_string(std::wstring& out_value, 	const wchar_t* buffer)	{ out_value = buffer; }

template<typename T>
inline typename std::enable_if_t<std::is_arithmetic_v<T>, std::wstring> lex_to_string(const T& value)
{
    return std::to_wstring(value);
}

inline std::wstring lex_to_string(bool value)
{
    return value ? STR("1") : STR("0");
}

inline static int32_t hex_digit(wchar_t c)
{
    int32_t result = 0;

    if (c >= '0' && c <= '9')
    {
        result = c - '0';
    }
    else if (c >= 'a' && c <= 'f')
    {
        result = c + 10 - 'a';
    }
    else if (c >= 'A' && c <= 'F')
    {
        result = c + 10 - 'A';
    }
    else
    {
        result = 0;
    }
    return result;
}

#endif //UE4SS_UNREALSTRINGCONVERSIONS_H
