#ifndef RC_UNREAL_UFUNCTIONSTRUCTS_HPP
#define RC_UNREAL_UFUNCTIONSTRUCTS_HPP

#include <functional>
#include <Unreal/Common.hpp>
#include <Unreal/FFrame.hpp>
#include <map>

namespace RC::Unreal
{
    class UObject;

    using UnrealScriptFunction = void(*)(UObject* Context, FFrame& TheStack, void* RESULT_DECL);

    class RC_UE_API UnrealScriptFunctionCallableContext
    {
    public:
        UObject* Context;
        FFrame& TheStack;
        void* RESULT_DECL;

        UnrealScriptFunctionCallableContext(UObject* Context, FFrame& TheStack, void* RESULT_DECL);

        template<typename ReturnType>
        auto SetReturnValue(ReturnType NewValue) -> void
        {
            *static_cast<ReturnType*>(RESULT_DECL) = NewValue;
        }

        template<typename ParamStruct>
        auto GetParams() -> ParamStruct&
        {
            return *static_cast<ParamStruct*>(static_cast<void*>(TheStack.Locals));
        }
    };
    using UnrealScriptFunctionCallable = std::function<void(UnrealScriptFunctionCallableContext& Context, void* CustomData)>;
    using CallbackId = int32_t;

    struct UnrealScriptCallbackData
    {
        UnrealScriptFunctionCallable Callable;
        void* CustomData;
    };

    class RC_UE_API UnrealScriptFunctionData
    {
    private:
        UnrealScriptFunction OriginalFunc;
        CallbackId HookIndexCounter;
        std::map<int32_t, UnrealScriptCallbackData> PreCallbacks;
        std::map<int32_t, UnrealScriptCallbackData> PostCallbacks;
    public:
        UnrealScriptFunctionData(UnrealScriptFunction OriginalFuncPtr);
        inline auto GetOriginalFuncPtr() const -> UnrealScriptFunction { return OriginalFunc; }

        CallbackId AddPreCallback(const UnrealScriptFunctionCallable& Callable, void* CustomData = nullptr);
        CallbackId AddPostCallback(const UnrealScriptFunctionCallable& Callable, void* CustomData = nullptr);
        bool RemoveCallback(CallbackId CallbackId);

        void RemoveAllCallbacks();

        void FirePreCallbacks(UnrealScriptFunctionCallableContext& Context) const;
        void FirePostCallbacks(UnrealScriptFunctionCallableContext& Context) const;
    };

    namespace Internal {
        using HookedUFunctionMap = std::unordered_map<const UFunction*, UnrealScriptFunctionData>;
        auto RC_UE_API GetHookedFunctionsMap() -> HookedUFunctionMap&;

        auto UnrealScriptFunctionHook(UObject* Context, FFrame& TheStack, void* RESULT_DECL) -> void;
    }
}

#endif //RC_UNREAL_UFUNCTIONSTRUCTS_HPP
