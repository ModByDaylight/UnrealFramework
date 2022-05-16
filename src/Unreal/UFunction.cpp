#include <Unreal/UFunction.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/FProperty.hpp>
#include <Helpers/Casting.hpp>

namespace RC::Unreal
{
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UFunction);

    using MemberOffsets = ::RC::Unreal::StaticOffsetFinder::MemberOffsets;

    auto UFunction::GetFunctionFlags() -> EFunctionFlags
    {
        // Works from 4.18 to 4.27 (confirmed)
        // Before 4.18, there was 'uint16 RepOffset;' between 'NumParms' and 'FunctionFlags'
        // For reference, 'NumParms' is 8 bits, and 'FunctionFlags' is 32 bits
        return Helper::Casting::offset_deref<EFunctionFlags>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UFunction_NumParms) - 4);
    }

    auto UFunction::GetFuncPtr() -> UnrealScriptFunction
    {
        return Helper::Casting::offset_deref<UnrealScriptFunction>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UFunction_Func));
    }

    auto UFunction::SetFuncPtr(UnrealScriptFunction NewFuncPtr) -> void
    {
        UnrealScriptFunction* Func = static_cast<UnrealScriptFunction*>(static_cast<void*>(this + StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UFunction_Func)));
        *Func = NewFuncPtr;
    }

    auto UFunction::GetNumParms() -> uint8_t
    {
        uint8_t NumParameters = 0;

        this->ForEachProperty([&](FProperty* Param) {
            if (Param->HasAnyPropertyFlags(Unreal::CPF_Parm))
            {
                NumParameters++;
            }
            return LoopAction::Continue;
        });
        return NumParameters;
    }

    uint16 UFunction::GetReturnValueOffset()
    {
        return Helper::Casting::ptr_cast_deref<uint16>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UFunction_ReturnValueOffset));
    }

    auto UFunction::GetParmsSize() -> int32_t
    {
        int32_t ParamsSize = 0;

        this->ForEachProperty([&](FProperty* Param) {
            if (Param->HasAnyPropertyFlags(Unreal::CPF_Parm))
            {
                ParamsSize = Param->GetOffset_Internal() + Param->GetSize();
            }
            return LoopAction::Continue;
        });
        return ParamsSize;
    }

    auto UFunction::GetReturnProperty() -> FProperty*
    {
        FProperty* ReturnProperty = nullptr;

        this->ForEachProperty([&](FProperty* Param) {
            if (Param->HasAnyPropertyFlags(Unreal::CPF_ReturnParm))
            {
                ReturnProperty = Param;
                return LoopAction::Break;
            }
            return LoopAction::Continue;
        });
        return ReturnProperty;
    }

    auto UFunction::GetFunctionHookData() -> UnrealScriptFunctionData&
    {
        Internal::HookedUFunctionMap& HookedFunctions = Internal::GetHookedFunctionsMap();
        auto Iterator = HookedFunctions.find(this);

        if (Iterator == HookedFunctions.end())
        {
            Iterator = HookedFunctions.insert({this, UnrealScriptFunctionData(GetFuncPtr())}).first;
            SetFuncPtr(&Internal::UnrealScriptFunctionHook);
        }
        return Iterator->second;
    }

    auto UFunction::RegisterPreHook(const UnrealScriptFunctionCallable& PreCallback, void* CustomData) -> CallbackId
    {
        UnrealScriptFunctionData& FunctionData = GetFunctionHookData();
        return FunctionData.AddPreCallback(PreCallback, CustomData);
    }

    auto UFunction::RegisterPostHook(const UnrealScriptFunctionCallable& PostCallback, void* CustomData) -> CallbackId
    {
        UnrealScriptFunctionData& FunctionData = GetFunctionHookData();
        return FunctionData.AddPostCallback(PostCallback, CustomData);
    }

    auto UFunction::UnregisterHook(CallbackId CallbackId) -> bool
    {
        UnrealScriptFunctionData& FunctionData = GetFunctionHookData();
        return FunctionData.RemoveCallback(CallbackId);
    }

    auto UFunction::UnregisterAllHooks() -> void
    {
        Internal::HookedUFunctionMap& HookedFunctions = Internal::GetHookedFunctionsMap();
        auto Iterator = HookedFunctions.find(this);

        if (Iterator != HookedFunctions.end())
        {
            Iterator->second.RemoveAllCallbacks();
            SetFuncPtr(Iterator->second.GetOriginalFuncPtr());
            HookedFunctions.erase(this);
        }
    }
}
