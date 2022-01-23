#ifndef RC_UNREAL_UNREALFLAGS_HPP
#define RC_UNREAL_UNREALFLAGS_HPP

#include "Unreal/EnumHelper.h"

namespace RC::Unreal
{
    // Enum used for converting to the real enum
    // Must be kept updated with the latest version of UE
    enum EObjectFlags
    {
        RF_NoFlags = 0x00000000,

        RF_Public = 0x00000001,
        RF_Standalone = 0x00000002,
        RF_MarkAsNative = 0x00000004,
        RF_Transactional = 0x00000008,
        RF_ClassDefaultObject = 0x00000010,
        RF_ArchetypeObject = 0x00000020,
        RF_Transient = 0x00000040,

        RF_MarkAsRootSet = 0x00000080,
        RF_TagGarbageTemp = 0x00000100,

        RF_NeedInitialization = 0x00000200,
        RF_NeedLoad = 0x00000400,
        RF_KeepForCooker = 0x00000800,
        RF_NeedPostLoad = 0x00001000,
        RF_NeedPostLoadSubobjects = 0x00002000,
        RF_NewerVersionExists = 0x00004000,
        RF_BeginDestroyed = 0x00008000,
        RF_FinishDestroyed = 0x00010000,

        RF_BeingRegenerated = 0x00020000,
        RF_DefaultSubObject = 0x00040000,
        RF_WasLoaded = 0x00080000,
        RF_TextExportTransient = 0x00100000,
        RF_LoadCompleted = 0x00200000,
        RF_InheritableComponentTemplate = 0x00400000,
        RF_DuplicateTransient = 0x00800000,
        RF_StrongRefOnFrame = 0x01000000,
        RF_NonPIEDuplicateTransient = 0x01000000,
        RF_Dynamic = 0x02000000,
        RF_WillBeLoaded = 0x04000000,
        RF_HasExternalPackage = 0x08000000,
        RF_AllFlags = 0x0FFFFFFF
    };
    ENUM_CLASS_FLAGS(EObjectFlags);

    // PropertyFlags from UE4.25 source
    // In the range 4.12 to 5.0 EA (branch: ue5-main):
    // It's confirmed that there a no breaking changes, so this enum does not need to be moved to a VC yet
    // Therefore, this is the real enum and not one to be used for converting
    enum EPropertyFlags : uint64_t
    {
        CPF_None = 0,

        CPF_Edit                            = 0x0000000000000001,    ///< Property is user-settable in the editor.
        CPF_ConstParm                       = 0x0000000000000002,    ///< This is a constant function parameter
        CPF_BlueprintVisible                = 0x0000000000000004,    ///< This property can be read by blueprint code
        CPF_ExportObject                    = 0x0000000000000008,    ///< Object can be exported with actor.
        CPF_BlueprintReadOnly               = 0x0000000000000010,    ///< This property cannot be modified by blueprint code
        CPF_Net                             = 0x0000000000000020,    ///< Property is relevant to network replication.
        CPF_EditFixedSize                   = 0x0000000000000040,    ///< Indicates that elements of an array can be modified, but its size cannot be changed.
        CPF_Parm                            = 0x0000000000000080,    ///< Function/When call parameter.
        CPF_OutParm                         = 0x0000000000000100,    ///< Value is copied out after function call.
        CPF_ZeroConstructor                 = 0x0000000000000200,    ///< memset is fine for construction
        CPF_ReturnParm                      = 0x0000000000000400,    ///< Return value.
        CPF_DisableEditOnTemplate           = 0x0000000000000800,    ///< Disable editing of this property on an archetype/sub-blueprint
        //CPF_                              = 0x0000000000001000,    ///<
        CPF_Transient                       = 0x0000000000002000,    ///< Property is transient: shouldn't be saved or loaded, except for Blueprint CDOs.
        CPF_Config                          = 0x0000000000004000,    ///< Property should be loaded/saved as permanent profile.
        //CPF_                              = 0x0000000000008000,    ///<
        CPF_DisableEditOnInstance           = 0x0000000000010000,    ///< Disable editing on an instance of this class
        CPF_EditConst                       = 0x0000000000020000,    ///< Property is uneditable in the editor.
        CPF_GlobalConfig                    = 0x0000000000040000,    ///< Load config from base class, not subclass.
        CPF_InstancedReference              = 0x0000000000080000,    ///< Property is a component references.
        //CPF_                              = 0x0000000000100000,    ///<
        CPF_DuplicateTransient              = 0x0000000000200000,    ///< Property should always be reset to the default value during any type of duplication (copy/paste, binary duplication, etc.)
        CPF_SubobjectReference              = 0x0000000000400000,    ///< Property contains subobject references (TSubobjectPtr)
        //CPF_                              = 0x0000000000800000,    ///<
        CPF_SaveGame                        = 0x0000000001000000,    ///< Property should be serialized for save games, this is only checked for game-specific archives with ArIsSaveGame
        CPF_NoClear                         = 0x0000000002000000,    ///< Hide clear (and browse) button.
        //CPF_                              = 0x0000000004000000,    ///<
        CPF_ReferenceParm                   = 0x0000000008000000,    ///< Value is passed by reference; CPF_OutParam and CPF_Param should also be set.
        CPF_BlueprintAssignable             = 0x0000000010000000,    ///< MC Delegates only.  Property should be exposed for assigning in blueprint code
        CPF_Deprecated                      = 0x0000000020000000,    ///< Property is deprecated.  Read it from an archive, but don't save it.
        CPF_IsPlainOldData                  = 0x0000000040000000,    ///< If this is set, then the property can be memcopied instead of CopyCompleteValue / CopySingleValue
        CPF_RepSkip                         = 0x0000000080000000,    ///< Not replicated. For non replicated properties in replicated structs
        CPF_RepNotify                       = 0x0000000100000000,    ///< Notify actors when a property is replicated
        CPF_Interp                          = 0x0000000200000000,    ///< interpolatable property for use with matinee
        CPF_NonTransactional                = 0x0000000400000000,    ///< Property isn't transacted
        CPF_EditorOnly                      = 0x0000000800000000,    ///< Property should only be loaded in the editor
        CPF_NoDestructor                    = 0x0000001000000000,    ///< No destructor
        //CPF_                              = 0x0000002000000000,    ///<
        CPF_AutoWeak                        = 0x0000004000000000,    ///< Only used for weak pointers, means the export type is autoweak
        CPF_ContainsInstancedReference      = 0x0000008000000000,    ///< Property contains component references.
        CPF_AssetRegistrySearchable         = 0x0000010000000000,    ///< asset instances will add properties with this flag to the asset registry automatically
        CPF_SimpleDisplay                   = 0x0000020000000000,    ///< The property is visible by default in the editor details view
        CPF_AdvancedDisplay                 = 0x0000040000000000,    ///< The property is advanced and not visible by default in the editor details view
        CPF_Protected                       = 0x0000080000000000,    ///< property is protected from the perspective of script
        CPF_BlueprintCallable               = 0x0000100000000000,    ///< MC Delegates only.  Property should be exposed for calling in blueprint code
        CPF_BlueprintAuthorityOnly          = 0x0000200000000000,    ///< MC Delegates only.  This delegate accepts (only in blueprint) only events with BlueprintAuthorityOnly.
        CPF_TextExportTransient             = 0x0000400000000000,    ///< Property shouldn't be exported to text format (e.g. copy/paste)
        CPF_NonPIEDuplicateTransient        = 0x0000800000000000,    ///< Property should only be copied in PIE
        CPF_ExposeOnSpawn                   = 0x0001000000000000,    ///< Property is exposed on spawn
        CPF_PersistentInstance              = 0x0002000000000000,    ///< A object referenced by the property is duplicated like a component. (Each actor should have an own instance.)
        CPF_UObjectWrapper                  = 0x0004000000000000,    ///< Property was parsed as a wrapper class like TSubclassOf<T>, FScriptInterface etc., rather than a USomething*
        CPF_HasGetValueTypeHash             = 0x0008000000000000,    ///< This property can generate a meaningful hash value.
        CPF_NativeAccessSpecifierPublic     = 0x0010000000000000,    ///< Public native access specifier
        CPF_NativeAccessSpecifierProtected  = 0x0020000000000000,    ///< Protected native access specifier
        CPF_NativeAccessSpecifierPrivate    = 0x0040000000000000,    ///< Private native access specifier
        CPF_SkipSerialization               = 0x0080000000000000,    ///< Property shouldn't be serialized, can still be exported to text
    };

    enum class EInternalObjectFlags : int32_t
    {
        None = 0,
        //~ All the other bits are reserved, DO NOT ADD NEW FLAGS HERE!

        ReachableInCluster = 1 << 23,
        ///< External reference to object in cluster exists
        ClusterRoot = 1 << 24,
        ///< Root of a cluster
        Native = 1 << 25,
        ///< Native (UClass only).
        Async = 1 << 26,
        ///< Object exists only on a different thread than the game thread.
        AsyncLoading = 1 << 27,
        ///< Object is being asynchronously loaded.
        Unreachable = 1 << 28,
        ///< Object is not reachable on the object graph.
        PendingKill = 1 << 29,
        ///< Objects that are pending destruction (invalid for gameplay but valid objects)
        RootSet = 1 << 30,
        ///< Object will not be garbage collected, even if unreferenced.
        //~ UnusedFlag = 1 << 31,

        GarbageCollectionKeepFlags = Native | Async | AsyncLoading,

        //~ Make sure this is up to date!
        AllFlags = ReachableInCluster | ClusterRoot | Native | Async | AsyncLoading | Unreachable | PendingKill | RootSet
    };

/** Mask of all property flags */
#define CPF_AllFlags                ((EPropertyFlags)0xFFFFFFFFFFFFFFFF)


/** Flags to inherit from base class */
#define CLASS_Inherit ((EClassFlags)(CLASS_Transient | CLASS_DefaultConfig | CLASS_Config | CLASS_PerObjectConfig | CLASS_ConfigDoNotCheckDefaults | CLASS_NotPlaceable \
						| CLASS_Const | CLASS_HasInstancedReference | CLASS_Deprecated | CLASS_DefaultToInstanced | CLASS_GlobalUserConfig | CLASS_ProjectUserConfig))

/** Flags that are inherited from the parent struct */
#define STRUCT_Inherit ((EStructFlags) (STRUCT_HasInstancedReference | STRUCT_Atomic))

/** Flags that are never serialized and are always computed in runtime */
#define STRUCT_ComputedFlags ((EStructFlags) (STRUCT_NetDeltaSerializeNative | STRUCT_NetSerializeNative | STRUCT_SerializeNative | STRUCT_PostSerializeNative | STRUCT_CopyNative | STRUCT_IsPlainOldData | STRUCT_NoDestructor | STRUCT_ZeroConstructor | STRUCT_IdenticalNative | STRUCT_AddStructReferencedObjects | STRUCT_ExportTextItemNative | STRUCT_ImportTextItemNative | STRUCT_SerializeFromMismatchedTag | STRUCT_PostScriptConstruct | STRUCT_NetSharedSerialization))

    // TODO: Check for all UE4+ versions and move to VC
    enum EClassFlags
    {
        CLASS_None				  = 0x00000000u,
        CLASS_Abstract            = 0x00000001u,
        CLASS_DefaultConfig		  = 0x00000002u,
        CLASS_Config			  = 0x00000004u,
        CLASS_Transient			  = 0x00000008u,
        CLASS_Parsed              = 0x00000010u,
        CLASS_MatchedSerializers  = 0x00000020u,
        CLASS_ProjectUserConfig	  = 0x00000040u,
        CLASS_Native			  = 0x00000080u,
        CLASS_NoExport            = 0x00000100u,
        CLASS_NotPlaceable        = 0x00000200u,
        CLASS_PerObjectConfig     = 0x00000400u,
        CLASS_ReplicationDataIsSetUp = 0x00000800u,
        CLASS_EditInlineNew		  = 0x00001000u,
        CLASS_CollapseCategories  = 0x00002000u,
        CLASS_Interface           = 0x00004000u,
        CLASS_CustomConstructor   = 0x00008000u,
        CLASS_Const			      = 0x00010000u,
        CLASS_LayoutChanging	  = 0x00020000u,
        CLASS_CompiledFromBlueprint  = 0x00040000u,
        CLASS_MinimalAPI	      = 0x00080000u,
        CLASS_RequiredAPI	      = 0x00100000u,
        CLASS_DefaultToInstanced  = 0x00200000u,
        CLASS_TokenStreamAssembled  = 0x00400000u,
        CLASS_HasInstancedReference= 0x00800000u,
        CLASS_Hidden			  = 0x01000000u,
        CLASS_Deprecated		  = 0x02000000u,
        CLASS_HideDropDown		  = 0x04000000u,
        CLASS_GlobalUserConfig	  = 0x08000000u,
        CLASS_Intrinsic			  = 0x10000000u,
        CLASS_Constructed		  = 0x20000000u,
        CLASS_ConfigDoNotCheckDefaults = 0x40000000u,
        CLASS_NewerVersionExists  = 0x80000000u,
    };

    enum EFunctionFlags : uint32_t
    {
        // Function flags.
        FUNC_None                = 0x00000000,

        FUNC_Final                = 0x00000001,
        FUNC_RequiredAPI            = 0x00000002,
        FUNC_BlueprintAuthorityOnly= 0x00000004,
        FUNC_BlueprintCosmetic    = 0x00000008,
        // FUNC_                = 0x00000010,
        // FUNC_                = 0x00000020,
        FUNC_Net                = 0x00000040,
        FUNC_NetReliable        = 0x00000080,
        FUNC_NetRequest            = 0x00000100,
        FUNC_Exec                = 0x00000200,
        FUNC_Native                = 0x00000400,
        FUNC_Event                = 0x00000800,
        FUNC_NetResponse        = 0x00001000,
        FUNC_Static                = 0x00002000,
        FUNC_NetMulticast        = 0x00004000,
        FUNC_UbergraphFunction    = 0x00008000,
        FUNC_MulticastDelegate    = 0x00010000,
        FUNC_Public                = 0x00020000,
        FUNC_Private            = 0x00040000,
        FUNC_Protected            = 0x00080000,
        FUNC_Delegate            = 0x00100000,
        FUNC_NetServer            = 0x00200000,
        FUNC_HasOutParms        = 0x00400000,
        FUNC_HasDefaults        = 0x00800000,
        FUNC_NetClient            = 0x01000000,
        FUNC_DLLImport            = 0x02000000,
        FUNC_BlueprintCallable    = 0x04000000,
        FUNC_BlueprintEvent        = 0x08000000,
        FUNC_BlueprintPure        = 0x10000000,
        FUNC_EditorOnly            = 0x20000000,
        FUNC_Const                = 0x40000000,
        FUNC_NetValidate        = 0x80000000,

        FUNC_AllFlags        = 0xFFFFFFFF,
    };

    enum EStructFlags
    {
        STRUCT_NoFlags				= 0x00000000,
        STRUCT_Native				= 0x00000001,
        STRUCT_IdenticalNative		= 0x00000002,
        STRUCT_HasInstancedReference= 0x00000004,
        STRUCT_NoExport				= 0x00000008,
        STRUCT_Atomic				= 0x00000010,
        STRUCT_Immutable			= 0x00000020,
        STRUCT_AddStructReferencedObjects = 0x00000040,
        STRUCT_RequiredAPI			= 0x00000200,
        STRUCT_NetSerializeNative	= 0x00000400,
        STRUCT_SerializeNative		= 0x00000800,
        STRUCT_CopyNative			= 0x00001000,
        STRUCT_IsPlainOldData		= 0x00002000,
        STRUCT_NoDestructor			= 0x00004000,
        STRUCT_ZeroConstructor		= 0x00008000,
        STRUCT_ExportTextItemNative	= 0x00010000,
        STRUCT_ImportTextItemNative	= 0x00020000,
        STRUCT_PostSerializeNative  = 0x00040000,
        STRUCT_SerializeFromMismatchedTag = 0x00080000,
        STRUCT_NetDeltaSerializeNative = 0x00100000,
        STRUCT_PostScriptConstruct     = 0x00200000,
        STRUCT_NetSharedSerialization = 0x00400000,
        STRUCT_Trashed = 0x00800000,
    };
}

#endif //RC_UNREAL_UNREALFLAGS_HPP
