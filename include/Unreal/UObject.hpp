#ifndef RC_UNREAL_UOBJECT_HPP
#define RC_UNREAL_UOBJECT_HPP

#include <stdexcept>

#include <File/File.hpp>
#include <Function/Function.hpp>
#include <Unreal/Common.hpp>
#include <Unreal/VersionedContainer/Container.hpp>
#include <Unreal/NameTypes.hpp>
#include <Unreal/FString.hpp>
#include <Unreal/UnrealFlags.hpp>
#include <Unreal/TypeChecker.hpp>
#include <Unreal/PrimitiveTypes.hpp>
#include <Unreal/UObjectMacros.hpp>

#define SUBOBJECT_DELIMITER_CHAR ':'

namespace RC::Unreal
{
    class UObject;
    class FArchive;
    struct FFrame;

    namespace UnrealInitializer
    {
        struct CacheInfo;
        auto CreateCache(CacheInfo&) -> void;
        auto LoadCache(CacheInfo&) -> void;
    }

    // Placeholder types for virtual functions
    // TODO: Remove when they are implemented
    using FRestoreForUObjectOverwrite = void*;
    using FObjectDuplicationParameters = void*;
    using FFeedbackContext = void*;
    using FResourceSizeEx = void*;
    using FAssetRegistryTag = void*;
    using FPrimaryAssetId = void*;

    template<typename Key, typename Value>
    class TMap;

    class RC_UE_API UObjectBase
    {
    public:
#include <VTableOffsets_UObjectBase.hpp>

    public:
        // Wrappers for virtual engine functions
        auto RegisterDependencies() -> void;
        auto DeferredRegister(class UClass* UClassStaticClass, const File::CharType* PackageName, const File::CharType* Name) -> void;
    };

    class RC_UE_API UObjectBaseUtility : public UObjectBase
    {
    public:
#include <VTableOffsets_UObjectBaseUtility.hpp>

    public:
        // Wrappers for virtual engine functions
        auto CanBeClusterRoot() const -> bool;
        auto CanBeInCluster() const -> bool;
        auto CreateCluster() -> void;
        auto OnClusterMarkedAsPendingKill() -> void;
    };

    class RC_UE_API UObject
    {
        DECLARE_EXTERNAL_OBJECT_CLASS(UObject, CoreUObject);

    public:
        using ProcessEventSignature = void(UObject* Context, class UFunction* Function, void* Params);
        static Function<ProcessEventSignature> ProcessEventInternal;

        using ProcessConsoleExecSignature = bool(UObject* Context, const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor);
        static Function<ProcessConsoleExecSignature> ProcessConsoleExecInternal;

    protected:
        friend void UnrealInitializer::CreateCache(UnrealInitializer::CacheInfo&);
        friend void UnrealInitializer::LoadCache(UnrealInitializer::CacheInfo&);
        friend void HookProcessEvent();
        friend struct FWeakObjectPtr;
        friend class AActor;
        friend class UAssetRegistry;

        auto GetInternalIndex() -> uint32_t;
        auto GetObjectItem() -> struct FUObjectItem*;

    public:
#include <VTableOffsets_UObject.hpp>

        // Wrappers for virtual engine functions
        FString GetDetailedInfoInternal() const;
        void PostInitProperties();
        void PostCDOContruct();
        bool PreSaveRoot(const TCHAR* Filename);
        void PostSaveRoot(bool bCleanupIsRequired);
        void PreSave(const class ITargetPlatform* TargetPlatform);
        bool IsReadyForAsyncPostLoad() const;
        void PostLoad();
        void PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph);
        void BeginDestroy();
        bool IsReadyForFinishDestroy();
        void FinishDestroy();
        void Serialize(FArchive& Ar);
        // Implement this when the vtable offset generator supports multiple functions with the same name
        //void Serialize(FStructuredArchive::FRecord Record);
        void ShutdownAfterError();
        void PostInterpChange(class FProperty* PropertyThatChanged);
        void PostRename(UObject* OldOuter, const FName OldName);
        void PreDuplicate(FObjectDuplicationParameters& DupParams);
        void PostDuplicate(bool bDuplicateForPIE);
        // Implement this when the vtable offset generator supports multiple functions with the same name
        //void PostDuplicate(EDuplicateMode::Type DuplicateMode);
        bool NeedsLoadForClient() const;
        bool NeedsLoadForServer() const;
        bool NeedsLoadForTargetPlatform(const class ITargetPlatform* TargetPlatform) const;
        bool NeedsLoadForEditorGame() const;
        bool IsEditorOnly() const;
        bool IsPostLoadThreadSafe() const;
        bool IsDestructionThreadSafe() const;
        void GetPreloadDependencies(TArray<UObject*>& OutDeps);
        void GetPrestreamPackages(TArray<UObject*>& OutPrestream);
        void ExportCustomProperties(FOutputDevice& Out, uint32 Indent);
        void ImportCustomProperties(const TCHAR* SourceText, FFeedbackContext* Warn);
        void PostEditImport();
        void PostReloadConfig(class FProperty* PropertyThatWasLoaded);
        bool Rename(const TCHAR* NewName=nullptr, UObject* NewOuter=nullptr, ERenameFlags Flags=REN_None);
        FString GetDesc();
        class UScriptStruct* GetSparseClassDataStruct() const;
        class UWorld* GetWorld() const;
        bool GetNativePropertyValues(TMap<FString, FString>& out_PropertyValues, uint32 ExportFlags=0) const;
        void GetResourceSizeEx(FResourceSizeEx& CumulativeResourceSize);
        FName GetExporterName();
        FRestoreForUObjectOverwrite* GetRestoreForUObjectOverwrite();
        bool AreNativePropertiesIdenticalTo(UObject* Other) const;
        void GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const;
        bool IsAsset() const;
        FPrimaryAssetId GetPrimaryAssetId() const;
        bool IsLocalizedResource() const;
        bool IsSafeForRootSet() const;
        void TagSubobjects(EObjectFlags NewFlags);
        void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const;
        bool IsNameStableForNetworking() const;
        bool IsFullNameStableForNetworking() const;
        bool IsSupportedForNetworking() const;
        void GetSubobjectsWithStableNamesForNetworking(TArray<UObject*>& ObjList);
        void PreNetReceive();
        void PostNetReceive();
        void PostRepNotifies();
        void PreDestroyFromReplication();
        void BuildSubobjectMapping(UObject* OtherObject, TMap<UObject*, UObject*>& ObjectMapping) const;
        const TCHAR* GetConfigOverridePlatform() const;
        void OverridePerObjectConfigSection(FString& SectionName);
        void ProcessEvent(UFunction* Function, void* Parms);
        int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack);
        bool CallRemoteFunction(UFunction* Function, void* Parms, struct FOutParmRec* OutParms, FFrame* Stack);
        bool ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor);
        UClass* RegenerateClass(UClass* ClassToRegenerate, UObject* PreviousCDO);
        void MarkAsEditorOnlySubobject();
        bool CheckDefaultSubobjectsInternal() const;
        void ValidateGeneratedRepEnums(const TArray<struct FRepRecord>& ClassReps) const;
        void SetNetPushIdDynamic(const int32 NewNetPushId);
        int32 GetNetPushIdDynamic() const;

    public:
        /**
         * Returns the Class of the object
         */
        auto GetClass() const -> UClass*;

        /**
         * Returns the object acting as the outer of this object
         * Every object must have a valid outer, with the exclusion of UPackage objects
         */
        auto GetOuter() -> UObject*;

        /**
         * Returns name of the object as the instance of FName
         * Names of the objects are unique inside their relevant outers
         */
        auto GetFName() -> FName;

        /**
         * Updates the flags currently set on the object to the provided ones
         * This function overwrites the flags completely, use set_flags or clear_flags to
         * add or remove flags instead
         */
        auto SetFlagsTo(EObjectFlags NewFlags) -> void;

        /**
         * Returns the name of this object, as string
         */
        inline auto GetName() -> std::wstring
        {
            return GetFName().ToString();
        }

        /**
         * Checks whenever this object is an instance of the specified class
         */
        auto IsA(UClass* Class) -> bool;

        /**
         * Adds new flags to the object
         */
        inline auto SetFlags(EObjectFlags NewFlags) -> void
        {
            Container::UnrealObjectVC->UObject_set_flags(this, NewFlags);
        }

        /**
         * Removes the provided flags from the object
         */
        inline auto ClearFlags(EObjectFlags ClearFlags) -> void
        {
            Container::UnrealObjectVC->UObject_clear_flags(this, ClearFlags);
        }

        /**
         * Checks whenever the object has any of the provided flags set
         */
        inline auto HasAnyFlags(EObjectFlags FlagsToCheck) -> bool
        {
            return Container::UnrealObjectVC->UObject_has_any_flag(this, FlagsToCheck);
        }

        /**
         * Checks whenever the object has all of the flags specified
         */
        auto HasAllFlags(EObjectFlags FlagsToCheck) -> bool
        {
            return Container::UnrealObjectVC->UObject_has_all_flags(this, FlagsToCheck);
        }

        bool HasAnyInternalFlags(EInternalObjectFlags FlagsToCheck)
        {
            return Container::UnrealVC->FUObjectItem_HasAnyFlags(Container::UnrealVC->UObjectArray_index_to_object(GetInternalIndex()), FlagsToCheck);
        }

        bool IsUnreachable()
        {
            return Container::UnrealVC->FUObjectItem_is_object_unreachable(Container::UnrealVC->UObjectArray_index_to_object(GetInternalIndex()));
        }

        /**
         * Templated version of the IsA(UClass*) function
         */
        template<UObjectDerivative T>
        inline auto IsA() -> bool
        {
            return IsA(T::StaticClass());
        }

        /**
         * Returns the outermost object for this object, normally the returned
         * object will always represent the UPackage instance
         */
        auto GetOutermost() -> UObject*;

        /**
         * Returns the first outer of the object that is a subclass of the provided type
         */
        auto GetTypedOuter(UClass* OuterType) -> UObject*;

         /**
          * Templated version of the get_typed_outer function above,
          * returns the object already casted to the provided type too
          */
         template<UObjectDerivative T>
         auto GetTypedOuter() -> T*
         {
              return Cast<T>(GetTypedOuter(T::StaticClass()));
         }

        /**
         * Returns the full path to the object in form of Package.Outer:ObjectName
         * Safe to call on the NULL objects and will return None in that case
         */
        auto GetPathName(UObject* StopOuter = nullptr) -> std::wstring;

        /**
         * Returns the full name of the object in form Class Package.Outer:ObjectName
         */
        auto GetFullName(UObject* StopOuter = nullptr) -> std::wstring;

        auto GetPathName(UObject* StopOuter, std::wstring& ResultString) -> void;

        size_t HashObject();

        static bool IsReal(const void* RawObject);
    };

    template<UObjectDerivative CastResultType>
    auto Cast(UObject* Object) -> CastResultType*
    {
        return Object != nullptr && Object->IsA<CastResultType>() ? static_cast<CastResultType*>(Object) : nullptr;
    }
}


#endif //RC_UNREAL_UOBJECT_HPP
