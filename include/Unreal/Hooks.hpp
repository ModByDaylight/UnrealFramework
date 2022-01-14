#ifndef RC_UNREAL_HOOKS_HPP
#define RC_UNREAL_HOOKS_HPP

#include <vector>
#include <unordered_map>
#include <memory>

#include <polyhook2/Detour/x64Detour.hpp>

#include <Unreal/Common.hpp>
#include <Unreal/UFunctionStructs.hpp>
#include <Unreal/FFrame.hpp>

namespace RC::LuaMadeSimple
{
    class Lua;
}

namespace RC::Unreal
{
    class UClass;
    class UFunction;

    namespace Hook
    {
        enum class HookType
        {
            Pre,
            Post,
        };

        using ProcessEventCallback = void(*)(UObject* context, UFunction*, void* parms);

        struct RC_API LuaCallbackData
        {
            const LuaMadeSimple::Lua& lua;
            UClass* instance_of_class;
            int32_t registry_index;
        };

        struct RC_API StaticStorage
        {
            struct RequiredObject
            {
                std::wstring object_name{};
                bool object_constructed{false};
            };

            static inline std::vector<RequiredObject> required_objects_for_init{};
            static inline int32_t num_required_objects_constructed{};
            static inline bool all_required_objects_constructed{false};
            static inline std::unique_ptr<::PLH::x64Detour> static_construct_object_detour;
            static inline std::unique_ptr<::PLH::x64Detour> process_event_detour;
            static inline std::vector<LuaCallbackData> static_construct_object_lua_callbacks;
            static inline std::vector<ProcessEventCallback> process_event_pre_callbacks;
            static inline std::vector<ProcessEventCallback> process_event_post_callbacks;
        };

        auto RC_API add_required_object(const std::wstring& object_full_typeless_name) -> void;
        auto RC_API register_process_event_callback(ProcessEventCallback, HookType) -> void;
    }

    using HookedUFunctionMap = std::unordered_map<const UFunction*, UnrealScriptFunctionData>;
    auto RC_API get_hooked_functions_map() -> HookedUFunctionMap&;
    auto RC_API unreal_script_function_hook(UObject* context, FFrame& the_stack, void* result_decl) -> void;

    auto RC_API hook_static_construct_object() -> void;
    auto RC_API hook_process_event() -> void;


}

#endif //RC_UNREAL_HOOKS_HPP
