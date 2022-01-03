#ifndef RC_UNREAL_UOBJECTBASEUTILITY_HPP
#define RC_UNREAL_UOBJECTBASEUTILITY_HPP

#include <cstdint>
#include <cstdio>

#include <Function/Function.hpp>

#include <Unreal/Common.hpp>

namespace RC::Unreal
{
    class UObject;
    class FStringOut;

    class RC_API UObjectBaseUtility
    {
    public:
        auto static get_path_name_recursive(const UObject* object, const void* stop_outer, std::wstring& out_path) -> void;
        auto static get_path_name(const UObject* object, const void* stop_outer, std::wstring& out_path) -> void;
        auto static get_full_name(const UObject* object, std::wstring& out_name, const void* stop_outer = nullptr) -> void;

    public:
        // Internal game function implementations
        auto get_full_name(const void* stop_outer = nullptr) -> std::wstring;
        auto get_full_name(std::wstring& out_name, const void* stop_outer = nullptr) -> void;
    };
}


#endif //RC_UNREAL_UOBJECTBASEUTILITY_HPP
