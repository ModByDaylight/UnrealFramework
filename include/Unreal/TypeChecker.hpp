#ifndef RC_UNREAL_TYPE_CHECKER_HPP
#define RC_UNREAL_TYPE_CHECKER_HPP

#include <unordered_map>
#include <functional>

#include <Unreal/Common.hpp>

namespace RC::Unreal
{
    struct FName;

    extern FName GWorldName;
    extern FName GLevelName;
    extern FName GPackageName;
    extern FName GPropertyName;
    extern FName GFunctionName;

    enum class InternalType
    {
        Array,
        Map,
    };

    class RC_UE_API TypeChecker
    {
    private:
        constexpr static int32_t FFieldClassOffset = 0x8;

    public:
        // Stores function pointers to all 'to_string' functions for UObject and FProperty.
        // The key is UClass* (or FFieldClass* in 4.25+)
        using ObjectToStringDecl = std::function<void(void*, std::wstring&)>;
        static std::unordered_map<void*, ObjectToStringDecl> m_object_to_string_functions;

        using ObjectToStringComplexDeclCallable = const std::function<void(void*)>&;
        using ObjectToStringComplexDecl = std::function<void(void*, std::wstring&, ObjectToStringComplexDeclCallable)>;
        static std::unordered_map<void*, ObjectToStringComplexDecl> m_object_to_string_complex_functions;

    public:
        // Property Names


    public:
        auto static get_world_name() -> FName;
        auto static get_level_name() -> FName;

    public:
        auto static store_all_object_names() -> void;
        auto static store_all_object_types() -> bool;

        // Returns true if the UObject inherits from the Property class
        // DO NOT RUN IN 4.25+!
        auto static is_property(class UObject* object) -> bool;
        auto static is_property(class FField* object) -> bool;

        auto static to_string_exists(void* uclass) -> bool
        {
            return m_object_to_string_functions.contains(uclass);
        }

        auto static to_string_complex_exists(void* uclass) -> bool
        {
            return m_object_to_string_complex_functions.contains(uclass);
        }

        auto static get_to_string(void* uclass)
        {
            return m_object_to_string_functions[uclass];
        }

        auto static get_to_string_complex(void* uclass)
        {
            return m_object_to_string_complex_functions[uclass];
        }
    };
}


#endif //RC_UNREAL_TYPE_CHECKER_HPP
