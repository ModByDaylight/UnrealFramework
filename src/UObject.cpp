#include <Unreal/TypeChecker.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/VersionedContainer/Container.hpp>
#include <Unreal/UPackage.hpp>
#include <Unreal/TMap.hpp>
#include <Unreal/UFunction.hpp>

namespace RC::Unreal
{
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UObject);

    using MemberOffsets = ::RC::Unreal::StaticOffsetFinder::MemberOffsets;

    auto UObjectBase::register_dependencies() -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObjectBase, RegisterDependencies, void)
    }

    auto UObjectBase::deferred_register(class UClass* UClassStaticClass, const File::CharType* PackageName, const File::CharType* Name) -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObjectBase, DeferredRegister, void, PARAMS(const UClass*, const File::CharType*, const File::CharType*), ARGS(UClassStaticClass, PackageName, Name))
    }

    auto UObjectBaseUtility::CanBeClusterRoot() const -> bool
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObjectBaseUtility, CanBeClusterRoot, bool)
    }

    auto UObjectBaseUtility::CanBeInCluster() const -> bool
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObjectBaseUtility, CanBeInCluster, bool)
    }

    auto UObjectBaseUtility::CreateCluster() -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObjectBaseUtility, CreateCluster, void)
    }

    auto UObjectBaseUtility::OnClusterMarkedAsPendingKill() -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObjectBaseUtility, OnClusterMarkedAsPendingKill, void)
    }

    auto UObject::get_uclass() const -> UClass*
    {
        return Helper::Casting::offset_deref<UClass*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UObject_ClassPrivate));
    }

    auto UObject::get_outer() -> UObject*
    {
        return Helper::Casting::offset_deref<UObject*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UObject_OuterPrivate));
    }

    auto UObject::get_fname() -> FName
    {
        return Helper::Casting::offset_deref<FName>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UObject_NamePrivate));
    }

    auto UObject::set_flags_to(EObjectFlags new_flags) -> void
    {
        Container::m_unreal_object_base->UObject_set_flags_to(this, new_flags);
    }

    auto UObject::get_internal_index() -> uint32_t
    {
        return Container::m_unreal_object_base->UObject_get_internal_index(this);
    }

    auto UObject::get_object_item() -> FUObjectItem*
    {
        return static_cast<FUObjectItem*>(Container::m_unreal_vc_base->UObjectArray_index_to_object(get_internal_index()));
    }

    FString UObject::GetDetailedInfoInternal() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, GetDetailedInfoInternal, FString)
    }

    void UObject::PostInitProperties()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, PostInitProperties, void)
    }

    void UObject::PostCDOContruct()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, PostCDOContruct, void)
    }

    bool UObject::PreSaveRoot(const TCHAR* Filename)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, PreSaveRoot, bool, PARAMS(const TCHAR*), ARGS(Filename))
    }

    void UObject::PostSaveRoot(bool bCleanupIsRequired)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, PostSaveRoot, void, PARAMS(bool), ARGS(bCleanupIsRequired))
    }

    void UObject::PreSave(const ITargetPlatform* TargetPlatform)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, PreSave, void, PARAMS(const ITargetPlatform*), ARGS(TargetPlatform))
    }

    bool UObject::IsReadyForAsyncPostLoad() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, IsReadyForAsyncPostLoad, bool)
    }

    void UObject::PostLoad()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, PostLoad, void)
    }

    void UObject::PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, PostLoadSubobjects, void, PARAMS(FObjectInstancingGraph*), ARGS(OuterInstanceGraph))
    }

    void UObject::BeginDestroy()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, BeginDestroy, void)
    }

    bool UObject::IsReadyForFinishDestroy()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, IsReadyForFinishDestroy, bool)
    }

    void UObject::FinishDestroy()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, FinishDestroy, void)
    }

    void UObject::Serialize(FArchive& Ar)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, Serialize, void, PARAMS(FArchive&), ARGS(Ar))
    }

    void UObject::ShutdownAfterError()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, ShutdownAfterError, void)
    }

    void UObject::PostInterpChange(FProperty* PropertyThatChanged)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, PostInterpChange, void, PARAMS(FProperty*), ARGS(PropertyThatChanged))
    }

    void UObject::PostRename(UObject* OldOuter, const FName OldName)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, PostRename, void, PARAMS(UObject*, const FName), ARGS(OldOuter, OldName))
    }

    void UObject::PreDuplicate(FObjectDuplicationParameters& DupParams)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, PreDuplicate, void, PARAMS(FObjectDuplicationParameters&), ARGS(DupParams))
    }

    void UObject::PostDuplicate(bool bDuplicateForPIE)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, PostDuplicate, void, PARAMS(bool), ARGS(bDuplicateForPIE))
    }

    bool UObject::NeedsLoadForClient() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, NeedsLoadForClient, bool)
    }

    bool UObject::NeedsLoadForServer() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, NeedsLoadForServer, bool)
    }

    bool UObject::NeedsLoadForTargetPlatform(const ITargetPlatform* TargetPlatform) const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, NeedsLoadForTargetPlatform, bool, PARAMS(const ITargetPlatform*), ARGS(TargetPlatform))
    }

    bool UObject::NeedsLoadForEditorGame() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, NeedsLoadForEditorGame, bool)
    }

    bool UObject::IsEditorOnly() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, IsEditorOnly, bool)
    }

    bool UObject::IsPostLoadThreadSafe() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, IsPostLoadThreadSafe, bool)
    }

    bool UObject::IsDestructionThreadSafe() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, IsDestructionThreadSafe, bool)
    }

    void UObject::GetPreloadDependencies(TArray<UObject*>& OutDeps)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, GetPreloadDependencies, void, PARAMS(TArray<UObject*>&), ARGS(OutDeps))
    }

    void UObject::GetPrestreamPackages(TArray<UObject*>& OutPrestream)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, GetPrestreamPackages, void, PARAMS(TArray<UObject*>&), ARGS(OutPrestream))
    }

    void UObject::ExportCustomProperties(FOutputDevice& Out, uint32 Indent)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, ExportCustomProperties, void, PARAMS(FOutputDevice&, uint32), ARGS(Out, Indent))
    }

    void UObject::ImportCustomProperties(const TCHAR* SourceText, FFeedbackContext* Warn)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, ImportCustomProperties, void, PARAMS(const TCHAR*, FFeedbackContext*), ARGS(SourceText, Warn))
    }

    void UObject::PostEditImport()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, PostEditImport, void)
    }

    void UObject::PostReloadConfig(FProperty* PropertyThatWasLoaded)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, PostReloadConfig, void, PARAMS(FProperty*), ARGS(PropertyThatWasLoaded))
    }

    bool UObject::Rename(const TCHAR* NewName, UObject* NewOuter, ERenameFlags Flags)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject,
                                         Rename,
                                         bool,
                                         PARAMS(const TCHAR*, UObject*, ERenameFlags),
                                         ARGS(NewName, NewOuter, Flags))
    }

    FString UObject::GetDesc()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, GetDesc, FString)
    }

    UScriptStruct* UObject::GetSparseClassDataStruct() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, GetSparseClassDataStruct, UScriptStruct*)
    }

    UWorld* UObject::GetWorld() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, GetWorld, UWorld*)
    }

    bool UObject::GetNativePropertyValues(TMap<FString, FString>& out_PropertyValues, uint32 ExportFlags) const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject,
                                         GetNativePropertyValues,
                                         bool,
                                         PARAMS(TMap<FString, FString>&, uint32),
                                         ARGS(out_PropertyValues, ExportFlags))
    }

    void UObject::GetResourceSizeEx(FResourceSizeEx& CumulativeResourceSize)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, GetResourceSizeEx, void, PARAMS(FResourceSizeEx&), ARGS(CumulativeResourceSize))
    }

    FName UObject::GetExporterName()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, GetExporterName, FName)
    }

    FRestoreForUObjectOverwrite* UObject::GetRestoreForUObjectOverwrite()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, GetRestoreForUObjectOverwrite, FRestoreForUObjectOverwrite*)
    }

    bool UObject::AreNativePropertiesIdenticalTo(UObject* Other) const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, AreNativePropertiesIdenticalTo, bool, PARAMS(UObject*), ARGS(Other))
    }

    void UObject::GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, GetAssetRegistryTags, void, PARAMS(TArray<FAssetRegistryTag>&), ARGS(OutTags))
    }

    bool UObject::IsAsset() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, IsAsset, bool)
    }

    FPrimaryAssetId UObject::GetPrimaryAssetId() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, GetPrimaryAssetId, FPrimaryAssetId)
    }

    bool UObject::IsLocalizedResource() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, IsLocalizedResource, bool)
    }

    bool UObject::IsSafeForRootSet() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, IsSafeForRootSet, bool)
    }

    void UObject::TagSubobjects(EObjectFlags NewFlags)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, TagSubobjects, void, PARAMS(EObjectFlags), ARGS(NewFlags))
    }

    void UObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, GetLifetimeReplicatedProps, void, PARAMS(TArray<FLifetimeProperty>&), ARGS(OutLifetimeProps))
    }

    bool UObject::IsNameStableForNetworking() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, IsNameStableForNetworking, bool)
    }

    bool UObject::IsFullNameStableForNetworking() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, IsFullNameStableForNetworking, bool)
    }

    bool UObject::IsSupportedForNetworking() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, IsSupportedForNetworking, bool)
    }

    void UObject::GetSubobjectsWithStableNamesForNetworking(TArray<UObject*>& ObjList)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject,
                                         GetSubobjectsWithStableNamesForNetworking,
                                         void,
                                         PARAMS(TArray<UObject*>&),
                                         ARGS(ObjList))
    }

    void UObject::PreNetReceive()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, PreNetReceive, void)
    }

    void UObject::PostNetReceive()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, PostNetReceive, void)
    }

    void UObject::PostRepNotifies()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, PostRepNotifies, void)
    }

    void UObject::PreDestroyFromReplication()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, PreDestroyFromReplication, void)
    }

    void UObject::BuildSubobjectMapping(UObject* OtherObject, TMap<UObject*, UObject*>& ObjectMapping) const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject,
                                         BuildSubobjectMapping,
                                         void,
                                         PARAMS(UObject*, TMap<UObject*, UObject*>&),
                                         ARGS(OtherObject, ObjectMapping))
    }

    const TCHAR* UObject::GetConfigOverridePlatform() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, GetConfigOverridePlatform, TCHAR*)
    }

    void UObject::OverridePerObjectConfigSection(FString& SectionName)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, OverridePerObjectConfigSection, void, PARAMS(FString&), ARGS(SectionName))
    }

    void UObject::ProcessEvent(UFunction* Function, void* Parms)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, ProcessEvent, void, PARAMS(UFunction*, void*), ARGS(Function, Parms))
    }

    int32 UObject::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, GetFunctionCallspace, int32, PARAMS(UFunction*, FFrame*), ARGS(Function, Stack))
    }

    bool UObject::CallRemoteFunction(UFunction* Function, void* Parms, struct FOutParmRec* OutParms, FFrame* Stack)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject,
                                         CallRemoteFunction,
                                         bool,
                                         PARAMS(UFunction*, void*, FOutParmRec*, FFrame*),
                                         ARGS(Function, Parms, OutParms, Stack))
    }

    bool UObject::ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject,
                                         ProcessConsoleExec,
                                         bool,
                                         PARAMS(const TCHAR*, FOutputDevice&, UObject*),
                                         ARGS(Cmd, Ar, Executor))
    }

    UClass* UObject::RegenerateClass(UClass* ClassToRegenerate, UObject* PreviousCDO)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, RegenerateClass, UClass*, PARAMS(UClass*, UObject*), ARGS(ClassToRegenerate, PreviousCDO))
    }

    void UObject::MarkAsEditorOnlySubobject()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, MarkAsEditorOnlySubobject, void)
    }

    bool UObject::CheckDefaultSubobjectsInternal() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, CheckDefaultSubobjectsInternal, bool)
    }

    void UObject::ValidateGeneratedRepEnums(const TArray<struct FRepRecord>& ClassReps) const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, ValidateGeneratedRepEnums, void, PARAMS(const TArray<FRepRecord>&), ARGS(ClassReps))
    }

    void UObject::SetNetPushIdDynamic(const int32 NewNetPushId)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UObject, SetNetPushIdDynamic, void, PARAMS(const int32), ARGS(NewNetPushId))
    }

    int32 UObject::GetNetPushIdDynamic() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UObject, GetNetPushIdDynamic, int32)
    }

    auto UObject::process_event(UFunction* function, void* params) -> void
    {
        process_event_internal(this, function, params);
    }

    auto UObject::get_outermost() -> UObject*
    {
        UObject* current_object = this;
        while (current_object->get_outer())
        {
            current_object = current_object->get_outer();
        }
        return current_object;
    }

    auto UObject::get_typed_outer(UClass* outer_type) -> UObject*
    {
        UObject* current_outer = get_outer();
        while (current_outer != nullptr)
        {
            if (current_outer->is_a(outer_type))
            {
                return current_outer;
            }
            current_outer = current_outer->get_outer();
        }
        return nullptr;
    }

    auto UObject::is_a(UClass* uclass) -> bool {
        return get_uclass()->is_child_of(uclass);
    }

    auto UObject::get_path_name(UObject *stop_outer) -> std::wstring
    {
        std::wstring result_name;
        get_path_name(stop_outer, result_name);
        return result_name;
    }

    auto UObject::get_path_name(UObject* stop_outer, std::wstring& result_string) -> void
    {
        if(this != stop_outer && this != nullptr)
        {
            UObject* outer = get_outer();

            if (outer && outer != stop_outer)
            {
                outer->get_path_name(stop_outer, result_string);

                // SUBOBJECT_DELIMITER_CHAR is used to indicate that this object's outer is not a UPackage
                if (outer->get_uclass() != UPackage::static_class()
                    && outer->get_outer()->get_uclass() == UPackage::static_class())
                {
                    result_string.push_back(SUBOBJECT_DELIMITER_CHAR);
                }
                else
                {
                    result_string.append(STR("."));
                }
            }
            result_string.append(get_name());
        }
        else
        {
            result_string.append(STR("None"));
        }
    }

    auto UObject::get_full_name(UObject* stop_outer) -> std::wstring
    {
        if (this == nullptr)
        {
            return STR("None");
        }
        else
        {
            std::wstring out_name;
            out_name.append(get_uclass()->get_name() + STR(" "));
            get_path_name(stop_outer, out_name);
            return out_name;
        }
    }
}