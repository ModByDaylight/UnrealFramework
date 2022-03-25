#include <Unreal/Hooks.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include <Unreal/UnrealInitializer.hpp>
#include <Unreal/FFrame.hpp>
#include <Unreal/UObjectGlobals.hpp>
#include <Unreal/UnrealVersion.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UAssetRegistry.hpp>
#include <Unreal/UGameViewportClient.hpp>

//#include <polyhook2/CapstoneDisassembler.hpp>
#include <polyhook2/ZydisDisassembler.hpp>

namespace RC::Unreal
{
    std::vector<Hook::StaticStorage::RequiredObject> Hook::StaticStorage::RequiredObjectsForInit{};
    int32_t Hook::StaticStorage::NumRequiredObjectsConstructed{};
    bool Hook::StaticStorage::bAllRequiredObjectsConstructed{false};
    std::unique_ptr<::PLH::x64Detour> Hook::StaticStorage::StaticConstructObjectDetour{};
    std::unique_ptr<::PLH::x64Detour> Hook::StaticStorage::ProcessEventDetour{};
    std::unique_ptr<::PLH::x64Detour> Hook::StaticStorage::ProcessConsoleExecDetour{};
    std::vector<Hook::StaticConstructObjectPreCallback> Hook::StaticStorage::StaticConstructObjectPreCallbacks{};
    std::vector<Hook::StaticConstructObjectPostCallback> Hook::StaticStorage::StaticConstructObjectPostCallbacks{};
    std::vector<Hook::ProcessEventCallback> Hook::StaticStorage::ProcessEventPreCallbacks{};
    std::vector<Hook::ProcessEventCallback> Hook::StaticStorage::ProcessEventPostCallbacks{};
    std::vector<Hook::ProcessConsoleExecCallback> Hook::StaticStorage::ProcessConsoleExecCallbacks{};

    uint64_t HookTrampolineProcessEvent = NULL;
    uint64_t HookTrampolineStaticConstructObject = NULL;
    uint64_t HookTrampolineProcessConsoleExec = NULL;

    auto Hook::AddRequiredObject(const std::wstring& ObjectFullTypelessName) -> void
    {
        if (!UObjectGlobals::StaticFindObject(nullptr, nullptr, ObjectFullTypelessName))
        {
            Output::send(STR("Need to construct: {}\n"), ObjectFullTypelessName);
            StaticStorage::RequiredObjectsForInit.emplace_back(Hook::StaticStorage::RequiredObject{ObjectFullTypelessName});
            StaticStorage::bAllRequiredObjectsConstructed = false;
        }
        else
        {
            Output::send(STR("Already constructed: {}\n"), ObjectFullTypelessName);
            if (StaticStorage::RequiredObjectsForInit.size() == 0)
            {
                StaticStorage::bAllRequiredObjectsConstructed = true;
            }
        }
    }

    auto Hook::RegisterStaticConstructObjectPreCallback(StaticConstructObjectPreCallback callback) -> void
    {
        StaticStorage::StaticConstructObjectPreCallbacks.emplace_back(callback);
    }

    auto Hook::RegisterStaticConstructObjectPostCallback(StaticConstructObjectPostCallback callback) -> void
    {
        StaticStorage::StaticConstructObjectPostCallbacks.emplace_back(callback);
    }

    auto Hook::RegisterProcessEventPreCallback(ProcessEventCallback callback) -> void
    {
        StaticStorage::ProcessEventPreCallbacks.emplace_back(callback);
    }

    auto Hook::RegisterProcessEventPostCallback(ProcessEventCallback callback) -> void
    {
        StaticStorage::ProcessEventPostCallbacks.emplace_back(callback);
    }

    auto RC_UE_API Hook::RegisterProcessConsoleExecCallback(ProcessConsoleExecCallback callback) -> void
    {
        StaticStorage::ProcessConsoleExecCallbacks.emplace_back(callback);
    }

    auto HookedStaticConstructObjectPre(const FStaticConstructObjectParameters& Params) -> UObject*
    {
        UObject* AlteredReturnValue{};
        for (const auto& Callback : Hook::StaticStorage::StaticConstructObjectPreCallbacks)
        {
            AlteredReturnValue = Callback(Params);
        }
        return AlteredReturnValue;
    }

    auto HookedStaticConstructObjectPost(const FStaticConstructObjectParameters& Params, UObject* ConstructedObject) -> UObject*
    {
        if (!Hook::StaticStorage::bAllRequiredObjectsConstructed)
        {
            for (auto& RequiredObject : Hook::StaticStorage::RequiredObjectsForInit)
            {
                if (Hook::StaticStorage::NumRequiredObjectsConstructed >= Hook::StaticStorage::RequiredObjectsForInit.size())
                {
                    Hook::StaticStorage::bAllRequiredObjectsConstructed = true;
                    break;
                }

                if (RequiredObject.ObjectConstructed) { continue; }

                UObject* required_object_ptr = UObjectGlobals::StaticFindObject(nullptr, nullptr, RequiredObject.ObjectName);
                if (required_object_ptr)
                {
                    RequiredObject.ObjectConstructed = true;
                    ++Hook::StaticStorage::NumRequiredObjectsConstructed;
                    Output::send(STR("Constructed [{} / {}]: {}\n"), Hook::StaticStorage::NumRequiredObjectsConstructed, Hook::StaticStorage::RequiredObjectsForInit.size(), RequiredObject.ObjectName);
                }
            }
        }

        UObject* AlteredReturnValue{};
        for (const auto& Callback : Hook::StaticStorage::StaticConstructObjectPostCallbacks)
        {
            AlteredReturnValue = Callback(Params, ConstructedObject);
        }
        return AlteredReturnValue;
    }

    auto HookedStaticConstructObjectDeprecated(StaticConstructObject_Internal_Params_Deprecated) -> UObject*
    {
        const FStaticConstructObjectParameters Params{
            .Class = InClass_,
            .Outer = InOuter_,
            .Name = InName_,
            .SetFlags = InFlags_,
            .InternalSetFlags = InternalSetFlags_,
            .bCopyTransientsFromClassDefaults = bCopyTransientsFromClassDefaults_,
            .bAssumeTemplateIsArchetype = bAssumeTemplateIsArchetype_,
            .Template = InTemplate_,
            .InstanceGraph = InInstanceGraph_,
            .ExternalPackage = static_cast<class UPackage*>(ExternalPackage_),
            .SubobjectOverrides = nullptr
        };

        UObject* AlteredReturnValue = HookedStaticConstructObjectPre(Params);
        UObject* ConstructedObject = PLH::FnCast(HookTrampolineStaticConstructObject, UObjectGlobals::GlobalState::StaticConstructObjectInternalDeprecated.get_function_pointer())(
                InClass_,
                InOuter_,
                InName_,
                InFlags_,
                InternalSetFlags_,
                InTemplate_,
                bCopyTransientsFromClassDefaults_,
                InInstanceGraph_,
                bAssumeTemplateIsArchetype_,
                ExternalPackage_
        );
        AlteredReturnValue = HookedStaticConstructObjectPost(Params, ConstructedObject);

        return AlteredReturnValue ? AlteredReturnValue : ConstructedObject;
    }

    auto HookedStaticConstructObject(const FStaticConstructObjectParameters& Params) -> UObject*
    {
        UObject* AlteredReturnValue = HookedStaticConstructObjectPre(Params);
        UObject* ConstructedObject = PLH::FnCast(HookTrampolineStaticConstructObject, UObjectGlobals::GlobalState::StaticConstructObjectInternal.get_function_pointer())(
                Params
        );
        AlteredReturnValue = HookedStaticConstructObjectPost(Params, ConstructedObject);

        return AlteredReturnValue ? AlteredReturnValue : ConstructedObject;
    }

    auto HookedProcessEvent(UObject* Context, UFunction* Function, void* Parms) -> void
    {
        for (const auto& Callback : Hook::StaticStorage::ProcessEventPreCallbacks)
        {
            Callback(Context, Function, Parms);
        }

        PLH::FnCast(HookTrampolineProcessEvent, UObject::ProcessEventInternal.get_function_pointer())(Context, Function, Parms);

        for (const auto& Callback : Hook::StaticStorage::ProcessEventPostCallbacks)
        {
            Callback(Context, Function, Parms);
        }

        if (UAssetRegistry::ShouldLoadAllAssets() && !UAssetRegistry::AssetsAreLoading())
        {
            UAssetRegistry::LoadAllAssetsThread();

            // Loading assets is an asynchronous operation
            // It's very important to not reset this bool until after the assets have been loaded
            // This is what keeps the load-requester waiting until all the assets have been loaded
            UAssetRegistry::SetShouldLoadAllAssets(false);
        }
    }

    auto HookedProcessConsoleExec(UObject* Context, const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor) -> bool
    {
        bool return_value = PLH::FnCast(HookTrampolineProcessConsoleExec, UObject::ProcessConsoleExecInternal.get_function_pointer())(Context, Cmd, Ar, Executor);

        if (Cast<UGameViewportClient>(Context))
        {
            for (const auto& callback : Hook::StaticStorage::ProcessConsoleExecCallbacks)
            {
                return_value = callback(Context, Cmd, Ar, Executor);
            }
        }

        return return_value;
    }

    auto HookStaticConstructObject() -> void
    {
        PLH::ZydisDisassembler Dis(PLH::Mode::x64);

        if (Version::IsAtMost(4, 25))
        {
            Hook::StaticStorage::StaticConstructObjectDetour = std::make_unique<PLH::x64Detour>(
                    static_cast<char*>(UObjectGlobals::GlobalState::StaticConstructObjectInternalDeprecated.get_function_address()),
                    std::bit_cast<char*>(&HookedStaticConstructObjectDeprecated),
                    &HookTrampolineStaticConstructObject,
                    Dis);
        }
        else
        {
            Hook::StaticStorage::StaticConstructObjectDetour = std::make_unique<PLH::x64Detour>(
                    static_cast<char*>(UObjectGlobals::GlobalState::StaticConstructObjectInternal.get_function_address()),
                    std::bit_cast<char*>(&HookedStaticConstructObject),
                    &HookTrampolineStaticConstructObject,
                    Dis);
        }

        Hook::StaticStorage::StaticConstructObjectDetour->hook();
    }

    auto HookProcessEvent() -> void
    {
        PLH::ZydisDisassembler Dis(PLH::Mode::x64);
        Hook::StaticStorage::ProcessEventDetour = std::make_unique<PLH::x64Detour>(
                static_cast<char*>(UObject::ProcessEventInternal.get_function_address()),
                std::bit_cast<char*>(&HookedProcessEvent),
                &HookTrampolineProcessEvent,
                Dis);

        Hook::StaticStorage::ProcessEventDetour->hook();
    }

    auto RC_UE_API HookProcessConsoleExec() -> void
    {
        PLH::ZydisDisassembler Dis(PLH::Mode::x64);
        Hook::StaticStorage::ProcessConsoleExecDetour = std::make_unique<PLH::x64Detour>(
                static_cast<char*>(UObject::ProcessConsoleExecInternal.get_function_address()),
                std::bit_cast<char*>(&HookedProcessConsoleExec),
                &HookTrampolineProcessConsoleExec,
                Dis);

        Hook::StaticStorage::ProcessConsoleExecDetour->hook();
    }
}
