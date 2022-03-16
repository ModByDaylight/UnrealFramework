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
    struct FStaticConstructObjectParameters;

    namespace Hook
    {
        enum class HookType
        {
            Pre,
            Post,
        };

        using ProcessEventCallback = void (*)(UObject* context, UFunction* function, void* parms);
        using StaticConstructObjectPreCallback = UObject* (*)(const FStaticConstructObjectParameters& params);
        using StaticConstructObjectPostCallback = UObject* (*)(const FStaticConstructObjectParameters& params, UObject* constructed_object);
        using ProcessConsoleExecCallback = bool (*)(UObject* Context, const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor);

        struct RC_UE_API StaticStorage
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
            static inline std::unique_ptr<::PLH::x64Detour> ProcessConsoleExecDetour;
            static inline std::vector<StaticConstructObjectPreCallback> static_construct_object_pre_callbacks;
            static inline std::vector<StaticConstructObjectPostCallback> static_construct_object_post_callbacks;
            static inline std::vector<ProcessEventCallback> process_event_pre_callbacks;
            static inline std::vector<ProcessEventCallback> process_event_post_callbacks;
            static inline std::vector<ProcessConsoleExecCallback> ProcessConsoleExecCallbacks;
        };

        auto RC_UE_API add_required_object(const std::wstring& object_full_typeless_name) -> void;

        // Registers a callback to be called whenever 'StaticConstructObject' is called
        // Callbacks may alter the return value of 'StaticConstructObject', and the last one to alter it is the one that takes effect
        // Alterations to parameters in the 'pre' callback are applied prior to object construction
        // Alterations to parameters in the 'post' callback have no effect
        auto RC_UE_API register_static_construct_object_pre_callback(StaticConstructObjectPreCallback) -> void;
        auto RC_UE_API register_static_construct_object_post_callback(StaticConstructObjectPostCallback) -> void;

        // Registers a callback to be called whenever 'ProcessEvent' is called
        // Callbacks may not alter the return value 'ProcessEvent', because there is no return value
        // Alterations to parameters in the 'pre' callback are applied prior to object construction
        // Alterations to parameters in the 'post' callback have no effect
        auto RC_UE_API register_process_event_pre_callback(ProcessEventCallback) -> void;
        auto RC_UE_API register_process_event_post_callback(ProcessEventCallback) -> void;

        // Registers a callback to be called whenever 'UGameViewportClient::ProcessConsoleExec' is executed
        auto RC_UE_API RegisterProcessConsoleExecCallback(ProcessConsoleExecCallback) -> void;
    }

    auto RC_UE_API hook_static_construct_object() -> void;
    auto RC_UE_API hook_process_event() -> void;
    auto RC_UE_API HookProcessConsoleExec() -> void;


}

#endif //RC_UNREAL_HOOKS_HPP
