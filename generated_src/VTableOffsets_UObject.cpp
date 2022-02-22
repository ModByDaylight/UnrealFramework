#include <cstdint>

#include <Unreal/UObject.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/FMemory.hpp>
#include <Unreal/Property/FMulticastDelegateProperty.hpp>
#include <Unreal/Property/FNumericProperty.hpp>
#include <Unreal/Property/FObjectProperty.hpp>
#include <Unreal/FProperty.hpp>

namespace RC::Unreal
{
    uint32_t UObject::VTableOffsets::PostSaveRoot = 0;
    uint32_t UObject::VTableOffsets::GetDetailedInfoInternal = 0;
    uint32_t UObject::VTableOffsets::MarkAsEditorOnlySubobject = 0;
    uint32_t UObject::VTableOffsets::NeedsLoadForEditorGame = 0;
    uint32_t UObject::VTableOffsets::PostInitProperties = 0;
    uint32_t UObject::VTableOffsets::Rename = 0;
    uint32_t UObject::VTableOffsets::Modify = 0;
    uint32_t UObject::VTableOffsets::PreSaveRoot = 0;
    uint32_t UObject::VTableOffsets::PostRename = 0;
    uint32_t UObject::VTableOffsets::ExportCustomProperties = 0;
    uint32_t UObject::VTableOffsets::Serialize = 0;
    uint32_t UObject::VTableOffsets::PostLoad = 0;
    uint32_t UObject::VTableOffsets::GetPreloadDependencies = 0;
    uint32_t UObject::VTableOffsets::PreSave = 0;
    uint32_t UObject::VTableOffsets::UObject_Destructor = 0;
    uint32_t UObject::VTableOffsets::PostLoadSubobjects = 0;
    uint32_t UObject::VTableOffsets::BeginDestroy = 0;
    uint32_t UObject::VTableOffsets::BuildSubobjectMapping = 0;
    uint32_t UObject::VTableOffsets::PostReloadConfig = 0;
    uint32_t UObject::VTableOffsets::IsReadyForFinishDestroy = 0;
    uint32_t UObject::VTableOffsets::FinishDestroy = 0;
    uint32_t UObject::VTableOffsets::ShutdownAfterError = 0;
    uint32_t UObject::VTableOffsets::GetFunctionCallspace = 0;
    uint32_t UObject::VTableOffsets::GetAssetRegistryTags = 0;
    uint32_t UObject::VTableOffsets::PostInterpChange = 0;
    uint32_t UObject::VTableOffsets::ProcessConsoleExec = 0;
    uint32_t UObject::VTableOffsets::PostDuplicate = 0;
    uint32_t UObject::VTableOffsets::NeedsLoadForClient = 0;
    uint32_t UObject::VTableOffsets::NeedsLoadForServer = 0;
    uint32_t UObject::VTableOffsets::IsPostLoadThreadSafe = 0;
    uint32_t UObject::VTableOffsets::IsLocalizedResource = 0;
    uint32_t UObject::VTableOffsets::ImportCustomProperties = 0;
    uint32_t UObject::VTableOffsets::GetResourceSize = 0;
    uint32_t UObject::VTableOffsets::PostEditImport = 0;
    uint32_t UObject::VTableOffsets::ProcessEvent = 0;
    uint32_t UObject::VTableOffsets::GetDesc = 0;
    uint32_t UObject::VTableOffsets::GetPrestreamPackages = 0;
    uint32_t UObject::VTableOffsets::GetWorld = 0;
    uint32_t UObject::VTableOffsets::__vecDelDtor = 0;
    uint32_t UObject::VTableOffsets::GetNativePropertyValues = 0;
    uint32_t UObject::VTableOffsets::RegenerateClass = 0;
    uint32_t UObject::VTableOffsets::IsAsset = 0;
    uint32_t UObject::VTableOffsets::GetExporterName = 0;
    uint32_t UObject::VTableOffsets::GetLifetimeReplicatedProps = 0;
    uint32_t UObject::VTableOffsets::GetRestoreForUObjectOverwrite = 0;
    uint32_t UObject::VTableOffsets::AreNativePropertiesIdenticalTo = 0;
    uint32_t UObject::VTableOffsets::IsSafeForRootSet = 0;
    uint32_t UObject::VTableOffsets::CheckDefaultSubobjectsInternal = 0;
    uint32_t UObject::VTableOffsets::TagSubobjects = 0;
    uint32_t UObject::VTableOffsets::IsNameStableForNetworking = 0;
    uint32_t UObject::VTableOffsets::IsFullNameStableForNetworking = 0;
    uint32_t UObject::VTableOffsets::IsSupportedForNetworking = 0;
    uint32_t UObject::VTableOffsets::GetSubobjectsWithStableNamesForNetworking = 0;
    uint32_t UObject::VTableOffsets::PreNetReceive = 0;
    uint32_t UObject::VTableOffsets::PostNetReceive = 0;
    uint32_t UObject::VTableOffsets::PostRepNotifies = 0;
    uint32_t UObject::VTableOffsets::GetPrimaryAssetId = 0;
    uint32_t UObject::VTableOffsets::IsEditorOnly = 0;
    uint32_t UObject::VTableOffsets::PreDestroyFromReplication = 0;
    uint32_t UObject::VTableOffsets::GetResourceSizeEx = 0;
    uint32_t UObject::VTableOffsets::CallRemoteFunction = 0;
    uint32_t UObject::VTableOffsets::PostCDOContruct = 0;
    uint32_t UObject::VTableOffsets::GetConfigOverridePlatform = 0;
    uint32_t UObject::VTableOffsets::NeedsLoadForTargetPlatform = 0;
    uint32_t UObject::VTableOffsets::OverridePerObjectConfigSection = 0;
    uint32_t UObject::VTableOffsets::IsReadyForAsyncPostLoad = 0;
    uint32_t UObject::VTableOffsets::IsDestructionThreadSafe = 0;
    uint32_t UObject::VTableOffsets::PreDuplicate = 0;
    uint32_t UObject::VTableOffsets::GetSparseClassDataStruct = 0;
    uint32_t UObject::VTableOffsets::ValidateGeneratedRepEnums = 0;
    uint32_t UObject::VTableOffsets::SetNetPushIdDynamic = 0;
    uint32_t UObject::VTableOffsets::GetNetPushIdDynamic = 0;
    uint32_t UObject::VTableOffsets::PostReinitProperties = 0;
    uint32_t UObject::VTableOffsets::ResolveSubobject = 0;
    uint32_t UObject::VTableOffsets::HasNonEditorOnlyReferences = 0;
    uint32_t UObject::VTableOffsets::GetExternalActorExtendedAssetRegistryTags = 0;
    uint32_t UObject::VTableOffsets::OverrideConfigSection = 0;
}
