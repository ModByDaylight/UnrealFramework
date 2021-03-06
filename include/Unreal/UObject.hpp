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
        RC_UE_API auto CreateCache(CacheInfo&) -> void;
        RC_UE_API auto LoadCache(CacheInfo&) -> void;
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
    private:
        friend class FUObjectCreateListener;
        friend class FUObjectDeleteListener;

    public:
        static std::unordered_map<std::wstring, uint32_t> VTableLayoutMap;

    public:
#include <MemberVariableLayout_HeaderWrapper_UObjectBase.hpp>

    public:
        // Wrappers for virtual engine functions
        auto RegisterDependencies() -> void;
        auto DeferredRegister(class UClass* UClassStaticClass, const File::CharType* PackageName, const File::CharType* Name) -> void;

    public:
        auto GetObjectItem() const -> const struct FUObjectItem*;
        auto GetObjectItem() -> struct FUObjectItem*;

        /**
         * Returns the Class of the object
         */
        auto GetClass() const -> UClass*;

        /**
         * Returns the object acting as the outer of this object
         * Every object must have a valid outer, with the exclusion of UPackage objects
         */
        auto GetOuter() const -> UObject*;

        /**
         * Returns name of the object as the instance of FName
         * Names of the objects are unique inside their relevant outers
         */
        auto GetFName() const -> FName;

        /**
         * Checks whenever this object is an instance of the specified class
         */
        auto IsA(UClass* Class) const -> bool;

        /**
         * Templated version of the IsA(UClass*) function
         */
        template<UObjectDerivative T>
        inline auto IsA() const -> bool
        {
            return IsA(T::StaticClass());
        }
    };

    class RC_UE_API UObjectBaseUtility : public UObjectBase
    {
    public:
        static std::unordered_map<std::wstring, uint32_t> VTableLayoutMap;

    public:
        // Wrappers for virtual engine functions
        auto CanBeClusterRoot() const -> bool;
        auto CanBeInCluster() const -> bool;
        auto CreateCluster() -> void;
        auto OnClusterMarkedAsPendingKill() -> void;
    };

    class RC_UE_API UObject : public UObjectBaseUtility
    {
        DECLARE_EXTERNAL_OBJECT_CLASS(UObject, CoreUObject);

    public:
        using ProcessEventSignature = void(UObject* Context, class UFunction* Function, void* Params);
        static Function<ProcessEventSignature> ProcessEventInternal;

        using ProcessConsoleExecSignature = bool(UObject* Context, const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor);
        static Function<ProcessConsoleExecSignature> ProcessConsoleExecInternal;

    protected:
        friend RC_UE_API void UnrealInitializer::CreateCache(UnrealInitializer::CacheInfo&);
        friend RC_UE_API void UnrealInitializer::LoadCache(UnrealInitializer::CacheInfo&);
        friend RC_UE_API void HookProcessEvent();
        friend struct FWeakObjectPtr;
        friend class AActor;
        friend class UAssetRegistry;

    public:
        static std::unordered_map<std::wstring, uint32_t> VTableLayoutMap;

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
        // Implement this when the vtable offset generator supports multiple functions with the same name
        struct FStructuredArchive { enum class FRecord {}; };
        void Serialize(FStructuredArchive::FRecord Record);
        void Serialize(FArchive& Ar);
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
        void ProcessEvent(UFunction* Function, void* Parms) const;
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
         * Updates the flags currently set on the object to the provided ones
         * This function overwrites the flags completely, use set_flags or clear_flags to
         * add or remove flags instead
         */
        auto SetFlagsTo(EObjectFlags NewFlags) -> void;

        /**
         * Returns the name of this object, as string
         */
        inline auto GetName() const -> std::wstring
        {
            return GetNamePrivate().ToString();
        }

        inline auto GetNameString() const -> FString {
            return GetNamePrivate().ToFString();
        }

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
        inline auto HasAnyFlags(EObjectFlags FlagsToCheck) const -> bool
        {
            return Container::UnrealObjectVC->UObject_has_any_flag(const_cast<UObject*>(this), FlagsToCheck);
        }

        /**
         * Checks whenever the object has all of the flags specified
         */
        auto HasAllFlags(EObjectFlags FlagsToCheck) const -> bool
        {
            return Container::UnrealObjectVC->UObject_has_all_flags(const_cast<UObject*>(this), FlagsToCheck);
        }

        bool HasAnyInternalFlags(EInternalObjectFlags FlagsToCheck)
        {
            return Container::UnrealVC->FUObjectItem_HasAnyFlags(Container::UnrealVC->UObjectArray_index_to_object(GetInternalIndex()), FlagsToCheck);
        }

        bool IsUnreachable() const
        {
            return Container::UnrealVC->FUObjectItem_is_object_unreachable(Container::UnrealVC->UObjectArray_index_to_object(GetInternalIndex()));
        }

        void SetRootSet()
        {
            Container::UnrealVC->FUObjectItem_set_object_root_set(Container::UnrealVC->UObjectArray_index_to_object(GetInternalIndex()), Base::SetOrUnsetFlag::Set);
        }

        void ClearRootSet()
        {
            Container::UnrealVC->FUObjectItem_set_object_root_set(Container::UnrealVC->UObjectArray_index_to_object(GetInternalIndex()), Base::SetOrUnsetFlag::Unset);
        }

        /** Checks if the object is unreachable. */
        FORCEINLINE bool IsNative() const {
            return Container::UnrealVC->FUObjectItem_is_object_native(Container::UnrealVC->UObjectArray_index_to_object(GetInternalIndex()));
        }

        /**
         * Returns the outermost object for this object, normally the returned
         * object will always represent the UPackage instance
         */
        auto GetOutermost() const -> UObject*;

        /**
         * Returns the first outer of the object that is a subclass of the provided type
         */
        auto GetTypedOuter(UClass* OuterType) const -> UObject*;

         /**
          * Templated version of the get_typed_outer function above,
          * returns the object already casted to the provided type too
          */
         template<UObjectDerivative T>
         auto GetTypedOuter() const -> T*
         {
              return Cast<T>(GetTypedOuter(T::StaticClass()));
         }

         /** Checks if the object is inside of the specified outer object */
        bool IsIn(const UObject* SomeOuter) const;

        /** Checks if the object is inside of the specified outer object */
        bool IsIn(const UPackage* SomePackage) const;

        /** Checks if the object is inside of the specified outer object */
        bool IsInOuter(const UObject* SomeOuter) const;

        /** Checks if the object is inside of the provided package */
        bool IsInPackage(const UPackage* SomePackage) const;

        /**
         * Walks up the list of outers until it finds a package directly associated with the object.
         *
         * @return the package the object is in.
         */
        UPackage* GetPackage() const;

        /** Returns the external UPackage associated with this object, if any */
        UPackage* GetExternalPackage() const;

        /**
         * Returns the full path to the object in form of Package.Outer:ObjectName
         * Safe to call on the NULL objects and will return None in that case
         */
        auto GetPathName(UObject* StopOuter = nullptr) const -> std::wstring;

        inline auto GetPathNameString(UObject* StopOuter = nullptr) const -> FString {
            return GetPathName(StopOuter).c_str();
        }

        /**
         * Returns the full name of the object in form Class Package.Outer:ObjectName
         */
        auto GetFullName(UObject* StopOuter = nullptr) const -> std::wstring;

        auto GetPathName(UObject* StopOuter, std::wstring& ResultString) const -> void;

        size_t HashObject();

        const void* GetValuePtrByPropertyName(const TCHAR* PropertyName) const;
        void* GetValuePtrByPropertyName(const TCHAR* PropertyName);

        template<typename ReturnType>
        ReturnType* GetValuePtrByPropertyName(const TCHAR* PropertyName) {
            static_assert(!std::is_same_v<ReturnType, bool>,
                    "GetValuePtrByPropertyName will give incorrect results on FBoolProperty. "
                    "Use GetClass()->FindProperty() and then FBoolProperty::GetPropertyValue");
            return static_cast<ReturnType*>(GetValuePtrByPropertyName(PropertyName));
        }

        template<typename ReturnType>
        const ReturnType* GetValuePtrByPropertyName(const TCHAR* PropertyName) const {
            static_assert(!std::is_same_v<ReturnType, bool>,
                          "GetValuePtrByPropertyName will give incorrect results on FBoolProperty. "
                          "Use GetClass()->FindProperty() and then FBoolProperty::GetPropertyValue");
            return static_cast<const ReturnType*>(GetValuePtrByPropertyName(PropertyName));
        }

        static bool IsReal(const void* RawObject);
    };

    template<UObjectDerivative CastResultType>
    auto Cast(UObject* Object) -> CastResultType* {
        return Object != nullptr && Object->IsA<CastResultType>() ? static_cast<CastResultType*>(Object) : nullptr;
    }

    template<UObjectDerivative CastResultType>
    auto Cast(const UObject* Object) -> const CastResultType* {
        return Object != nullptr && Object->IsA<CastResultType>() ? static_cast<const CastResultType*>(Object) : nullptr;
    }
}


#endif //RC_UNREAL_UOBJECT_HPP
