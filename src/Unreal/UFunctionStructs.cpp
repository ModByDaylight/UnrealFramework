#include <Unreal/UFunctionStructs.hpp>
#include <Unreal/UFunction.hpp>
#include <DynamicOutput/Output.hpp>
#include <Helpers/Format.hpp>
#include <Unreal/Hooks.hpp>

namespace RC::Unreal
{
    UnrealScriptFunctionData::UnrealScriptFunctionData(UnrealScriptFunction OriginalFuncPtr) {
        this->OriginalFunc = OriginalFuncPtr;
        this->HookIndexCounter = 1;
    }

    CallbackId UnrealScriptFunctionData::AddPreCallback(const UnrealScriptFunctionCallable& Callable, void* CustomData)
    {
        CallbackId new_callback_id = this->HookIndexCounter++;
        this->PreCallbacks.insert({new_callback_id, {Callable, CustomData}});
        return new_callback_id;
    }

    CallbackId UnrealScriptFunctionData::AddPostCallback(const UnrealScriptFunctionCallable& Callable, void* CustomData)
    {
        CallbackId NewCallbackId = this->HookIndexCounter++;
        this->PostCallbacks.insert({NewCallbackId, {Callable, CustomData}});
        return NewCallbackId;
    }

    bool UnrealScriptFunctionData::RemoveCallback(CallbackId CallbackId)
    {
        return PreCallbacks.erase(CallbackId) ||
               PostCallbacks.erase(CallbackId);
    }

    void UnrealScriptFunctionData::RemoveAllCallbacks()
    {
        PreCallbacks.clear();
        PostCallbacks.clear();
    }

    void UnrealScriptFunctionData::FirePreCallbacks(UnrealScriptFunctionCallableContext& Context) const
    {
        for (const auto& [CallbackId, CallbackData] : PreCallbacks)
        {
            CallbackData.Callable(Context, CallbackData.CustomData);
        }
    }

    void UnrealScriptFunctionData::FirePostCallbacks(UnrealScriptFunctionCallableContext& Context) const
    {
        for (const auto& [CallbackId, CallbackData]  : PostCallbacks)
        {
            CallbackData.Callable(Context, CallbackData.CustomData);
        }
    }

    UnrealScriptFunctionCallableContext::UnrealScriptFunctionCallableContext(UObject* Context, FFrame& TheStack, void* RESULT_DECL)
            : Context(Context),
              TheStack(TheStack),
              RESULT_DECL(RESULT_DECL) {}

    static Internal::HookedUFunctionMap GHookedScriptFunctions{};

    auto Internal::GetHookedFunctionsMap() -> HookedUFunctionMap&
    {
        return GHookedScriptFunctions;
    }

    auto Internal::UnrealScriptFunctionHook(UObject* Context, FFrame& TheStack, void* RESULT_DECL) -> void
    {
        try
        {
            HookedUFunctionMap& FunctionMap = GetHookedFunctionsMap();
            const auto Iterator = FunctionMap.find(TheStack.CurrentNativeFunction);

            if (Iterator == FunctionMap.end())
            {
                throw std::runtime_error("Failed to find the function map entry for hooked function");
            }

            UnrealScriptFunctionCallableContext FuncContext(Context, TheStack, RESULT_DECL);

            Iterator->second.FirePreCallbacks(FuncContext);
            Iterator->second.GetOriginalFuncPtr()(Context, TheStack, RESULT_DECL);
            Iterator->second.FirePostCallbacks(FuncContext);
        }
        catch (std::exception& e)
        {
            Output::send(STR("{}\n"), RC::fmt(L"Error executing hooked function %s: %S", TheStack.CurrentNativeFunction->GetPathName().c_str(), e.what()));
        }
    }
}
