#ifndef RC_UNREAL_TYPE_CHECKER_HPP
#define RC_UNREAL_TYPE_CHECKER_HPP

#include <unordered_map>
#include <functional>

#include <Unreal/Common.hpp>
#include <Unreal/NameTypes.hpp>

namespace RC::Unreal
{
    enum class InternalType
    {
        Array,
        Map,
    };

    class RC_UE_API TypeChecker
    {
    private:
        constexpr static int32_t ffield_class_offset = 0x8;
        constexpr static int32_t super_class_offset = 0x20;

    public:
        static inline std::unordered_map<std::wstring, FName> m_core_object_names;

        // Be careful with storing pointers, they may be relocated at any given point so make sure before you store them
        static inline std::unordered_map<std::wstring, void*> m_core_object_pointers;

        // Stores function pointers to all 'to_string' functions for UObject and XProperty.
        // The key is UClass* (or FFieldClass* in 4.25+)
        using ObjectToStringDecl = std::function<void(void*, std::wstring&)>;
        static inline std::unordered_map<void*, ObjectToStringDecl> m_object_to_string_functions;

        using ObjectToStringComplexDeclCallable = const std::function<void(void*)>&;
        using ObjectToStringComplexDecl = std::function<void(void*, std::wstring&, ObjectToStringComplexDeclCallable)>;
        static inline std::unordered_map<void*, ObjectToStringComplexDecl> m_object_to_string_complex_functions;

    public:
        // Core Names
        static inline FName m_core_world_name{};
        static inline FName m_core_level_name{};
        static inline FName m_core_package_name{};
        static inline FName m_core_enum_name{};
        static inline FName m_core_object_name{};
        static inline FName m_core_engine_name{};

        // Property Names
        static inline FName m_property_name{};
        static inline FName m_boolproperty_name{};
        static inline FName m_int8property_name{};
        static inline FName m_int16property_name{};
        static inline FName m_intproperty_name{};
        static inline FName m_int64property_name{};
        static inline FName m_byteproperty_name{};
        static inline FName m_uint16property_name{};
        static inline FName m_uint32property_name{};
        static inline FName m_uint64property_name{};
        static inline FName m_floatproperty_name{};
        static inline FName m_doubleproperty_name{};
        static inline FName m_objectproperty_name{};
        static inline FName m_classproperty_name{};
        static inline FName m_strproperty_name{};
        static inline FName m_textproperty_name{};
        static inline FName m_structproperty_name{};
        static inline FName m_enumproperty_name{};
        static inline FName m_arrayproperty_name{};
        static inline FName m_nameproperty_name{};
        static inline FName m_scriptdelegateproperty_name{};
        static inline FName m_multicastinlinescriptdelegateproperty_name{};
        static inline FName m_multicastsparsescriptdelegateproperty_name{};
        static inline FName m_weakobjectproperty_name{};
        static inline FName m_mapproperty_name{};

        static inline FName m_function_name{};

    public:
        auto static get_world_name() -> FName;
        auto static get_level_name() -> FName;

    public:
        auto static store_all_object_names() -> void;
        auto static store_all_object_types() -> bool;

        auto static get_fname(const std::wstring& object_name) -> FName;

        auto static is_ffield(const void* p_this) -> bool;

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

        template<typename Callable, typename Callable2>
        auto static inline dump_all_stored_object_types(Callable callable, Callable2 callable2) -> void
        {
            for (auto&[object_string, object_fname] : m_core_object_names)
            {
                callable(object_fname.get_comparison_index(), object_fname.get_number());
            }

            for (auto&[object_string, object_ptr] : m_core_object_pointers)
            {
                callable2(object_string, object_ptr);
            }
        }
    };
}


#endif //RC_UNREAL_TYPE_CHECKER_HPP
