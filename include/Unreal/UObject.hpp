#ifndef RC_UNREAL_UOBJECT_HPP
#define RC_UNREAL_UOBJECT_HPP

#include <stdexcept>
#include <File/File.hpp>
#include <Function/Function.hpp>
#include <Unreal/Common.hpp>
#include <Unreal/NameTypes.hpp>
#include <Unreal/FString.hpp>
#include <Unreal/UnrealFlags.hpp>
#include <Unreal/PrimitiveTypes.hpp>
#include <Unreal/UObjectMacros.h>

#define SUBOBJECT_DELIMITER_CHAR ':'

namespace RC::Unreal
{
    class UObject;
    struct FFrame;

    template<typename Key, typename Value>
    class TMap;

    /** Concept describing the type of the pointer pointing to the UObject-derived object */
    template<typename SupposedUObject>
    concept UObjectPointerDerivative = std::is_convertible_v<SupposedUObject, UObject*>;

    /** Concept describing the type derived from the UObject */
    template<typename SupposedUObject>
    concept UObjectDerivative = std::is_convertible_v<SupposedUObject, UObject>;

    /** Casts the object to the provided type if possible, returns nullptr otherwise */
    template<UObjectDerivative CastResultType>
    auto cast_object(UObject* object) -> CastResultType*;

    class RC_UE_API UObjectBase
    {
    public:
#include <VTableOffsets_UObjectBase.hpp>

    public:
        // Wrappers for virtual engine functions
        auto register_dependencies() -> void;
        auto deferred_register(class UClass* UClassStaticClass, const File::CharType* PackageName, const File::CharType* Name) -> void;
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
    protected:
        using ProcessEventSignature = void(UObject* context, class UFunction* function, void* params);
        static Function<ProcessEventSignature> process_event_internal;

        auto get_internal_index() -> uint32_t;
        auto get_object_item() -> struct FUObjectItem*;

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
        using FObjectInstancingGraph = void*; // Remove if/when we have an implementation for FObjectInstancingGraph
        void PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph);
        void BeginDestroy();
        bool IsReadyForFinishDestroy();
        void FinishDestroy();
        using FArchive = void*; // Remove if/when we have an implementation for FArchive
        void Serialize(FArchive& Ar);
        // Implement this when the vtable offset generator supports multiple functions with the same name
        //void Serialize(FStructuredArchive::FRecord Record);
        void ShutdownAfterError();
        void PostInterpChange(class FProperty* PropertyThatChanged);
        void PostRename(UObject* OldOuter, const FName OldName);
        using FObjectDuplicationParameters = void*; // Remove if/when we have an implementation for FObjectDuplicationParameters
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
        using FOutputDevice = void*; // Remove if/when we have an implementation for FOutputDevice
        void ExportCustomProperties(FOutputDevice& Out, uint32 Indent);
        using FFeedbackContext = void*; // Remove if/when we have an implementation for FFeedbackContext
        void ImportCustomProperties(const TCHAR* SourceText, FFeedbackContext* Warn);
        void PostEditImport();
        void PostReloadConfig(class FProperty* PropertyThatWasLoaded);
        bool Rename(const TCHAR* NewName=nullptr, UObject* NewOuter=nullptr, ERenameFlags Flags=REN_None);
        FString GetDesc();
        class UScriptStruct* GetSparseClassDataStruct() const;
        class UWorld* GetWorld() const;
        bool GetNativePropertyValues(TMap<FString, FString>& out_PropertyValues, uint32 ExportFlags=0) const;
        using FResourceSizeEx = void*; // Remove if/when we have an implementation for FResourceSizeEx
        void GetResourceSizeEx(FResourceSizeEx& CumulativeResourceSize);
        FName GetExporterName();
        using FRestoreForUObjectOverwrite = void*; // Remove if/when we have an implementation for FRestoreForUObjectOverwrite
        FRestoreForUObjectOverwrite* GetRestoreForUObjectOverwrite();
        bool AreNativePropertiesIdenticalTo(UObject* Other) const;
        using FAssetRegistryTag = void*; // Remove if/when we have an implementation for FAssetRegistryTag
        void GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const;
        bool IsAsset() const;
        using FPrimaryAssetId = void*; // Remove if/when we have an implementation for FPrimaryAssetId
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
        auto get_uclass() const -> UClass*;

        /**
         * Returns the object acting as the outer of this object
         * Every object must have a valid outer, with the exclusion of UPackage objects
         */
        auto get_outer() -> UObject*;

        /**
         * Returns name of the object as the instance of FName
         * Names of the objects are unique inside their relevant outers
         */
        auto get_fname() -> FName;

        /**
         * Returns the object flags currently set on the object
         */
        auto get_flags() -> EObjectFlags;

        /**
         * Updates the flags currently set on the object to the provided ones
         * This function overwrites the flags completely, use set_flags or clear_flags to
         * add or remove flags instead
         */
        auto set_flags_to(EObjectFlags new_flags) -> void;

        /**
         * Returns the name of this object, as string
         */
        inline auto get_name() -> std::wstring
        {
            return get_fname().to_string();
        }

        /**
         * Checks whenever this object is an instance of the specified class
         */
        auto is_a(UClass* uclass) -> bool;

        /**
         * Adds new flags to the object
         */
        inline auto set_flags(EObjectFlags new_flags) -> void
        {
            set_flags_to(get_flags() & new_flags);
        }

        /**
         * Removes the provided flags from the object
         */
        inline auto clear_flags(EObjectFlags clear_flags) -> void
        {
            set_flags_to(get_flags() & (~clear_flags));
        }

        /**
         * Checks whenever the object has any of the provided flags set
         */
        inline auto has_any_flag(EObjectFlags flags_to_check) -> bool
        {
            return (get_flags() & flags_to_check) != 0;
        }

        /**
         * Checks whenever the object has all of the flags specified
         */
        auto has_all_flags(EObjectFlags flags_to_check) -> bool
        {
            return (get_flags() & flags_to_check) == flags_to_check;
        }

        /**
         * Templated version of the IsA(UClass*) function
         */
        template<UObjectDerivative T>
        inline auto is_a() -> bool
        {
            return is_a(T::static_class());
        }

        /**
         * Returns the outermost object for this object, normally the returned
         * object will always represent the UPackage instance
         */
        auto get_outermost() -> UObject*;

        /**
         * Returns the first outer of the object that is a subclass of the provided type
         */
        auto get_typed_outer(UClass* outer_type) -> UObject*;

         /**
          * Templated version of the get_typed_outer function above,
          * returns the object already casted to the provided type too
          */
         template<UObjectDerivative T>
         auto get_typed_outer() -> T*
         {
              return cast_object<T>(get_typed_outer(T::static_class()));
         }

        /**
         * Executes the ProcessEvent on this object with the provided function,
         * which practically means calling the provided function with passed parameters struct
         * and object as it's context argument
         */
        auto process_event(UFunction* function, void* params) -> void;

        /**
         * Returns the full path to the object in form of Package.Outer:ObjectName
         * Safe to call on the NULL objects and will return None in that case
         */
        auto get_path_name(UObject* stop_outer = nullptr) -> std::wstring;

        /**
         * Returns the full name of the object in form Class Package.Outer:ObjectName
         */
        auto get_full_name(UObject* stop_outer = nullptr) -> std::wstring;

        auto get_path_name(UObject* stop_outer, std::wstring& result_name) -> void;
    };

    template<UObjectDerivative CastResultType>
    auto cast_object(UObject* object) -> CastResultType*
    {
        return object != nullptr && object->is_a<CastResultType>() ? static_cast<CastResultType*>(object) : nullptr;
    }
}


#endif //RC_UNREAL_UOBJECT_HPP
