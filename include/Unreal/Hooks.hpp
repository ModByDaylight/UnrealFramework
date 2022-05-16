#ifndef RC_UNREAL_HOOKS_HPP
#define RC_UNREAL_HOOKS_HPP

#include <vector>
#include <unordered_map>
#include <memory>

#include <Unreal/Common.hpp>
#include <Unreal/UFunctionStructs.hpp>
#include <Unreal/FFrame.hpp>

namespace PLH
{
    class x64Detour;
}

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

        using ProcessEventCallback = void(*)(UObject* Context, UFunction* Function, void* Parms);
        using StaticConstructObjectPreCallback = UObject* (*)(const FStaticConstructObjectParameters& Params);
        using StaticConstructObjectPostCallback = UObject* (*)(const FStaticConstructObjectParameters& Params, UObject* ConstructedObject);
        using ProcessConsoleExecCallback = bool (*)(UObject* Context, const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor);

        struct RC_UE_API StaticStorage
        {
            struct RequiredObject
            {
                std::wstring ObjectName{};
                bool ObjectConstructed{false};
            };

            static std::vector<RequiredObject> RequiredObjectsForInit;
            static int32_t NumRequiredObjectsConstructed;
            static bool bAllRequiredObjectsConstructed;

            static std::unique_ptr<::PLH::x64Detour> StaticConstructObjectDetour;
            static std::unique_ptr<::PLH::x64Detour> ProcessEventDetour;
            static std::unique_ptr<::PLH::x64Detour> ProcessConsoleExecDetour;
            static std::unique_ptr<::PLH::x64Detour> UStructLinkDetour;

            static std::vector<StaticConstructObjectPreCallback> StaticConstructObjectPreCallbacks;
            static std::vector<StaticConstructObjectPostCallback> StaticConstructObjectPostCallbacks;
            static std::vector<ProcessEventCallback> ProcessEventPreCallbacks;
            static std::vector<ProcessEventCallback> ProcessEventPostCallbacks;
            static std::vector<ProcessConsoleExecCallback> ProcessConsoleExecCallbacks;
        };

        auto RC_UE_API AddRequiredObject(const std::wstring& ObjectFullTypelessName) -> void;

        // Registers a callback to be called whenever 'StaticConstructObject' is called
        // Callbacks may alter the return value of 'StaticConstructObject', and the last one to alter it is the one that takes effect
        // Alterations to parameters in the 'pre' callback are applied prior to object construction
        // Alterations to parameters in the 'post' callback have no effect
        auto RC_UE_API RegisterStaticConstructObjectPreCallback(StaticConstructObjectPreCallback) -> void;
        auto RC_UE_API RegisterStaticConstructObjectPostCallback(StaticConstructObjectPostCallback) -> void;

        // Registers a callback to be called whenever 'ProcessEvent' is called
        // Callbacks may not alter the return value 'ProcessEvent', because there is no return value
        // Alterations to parameters in the 'pre' callback are applied prior to object construction
        // Alterations to parameters in the 'post' callback have no effect
        auto RC_UE_API RegisterProcessEventPreCallback(ProcessEventCallback) -> void;
        auto RC_UE_API RegisterProcessEventPostCallback(ProcessEventCallback) -> void;

        // Registers a callback to be called whenever 'UGameViewportClient::ProcessConsoleExec' is executed
        auto RC_UE_API RegisterProcessConsoleExecCallback(ProcessConsoleExecCallback) -> void;
    }

    auto RC_UE_API HookStaticConstructObject() -> void;
    auto RC_UE_API HookProcessEvent() -> void;
    auto RC_UE_API HookProcessConsoleExec() -> void;
    auto RC_UE_API HookUStructLink() -> void;


}

#endif //RC_UNREAL_HOOKS_HPP
