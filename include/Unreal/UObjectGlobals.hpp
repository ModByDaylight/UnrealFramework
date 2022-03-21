#ifndef RC_UNREAL_UOBJECTGLOBALS_HPP
#define RC_UNREAL_UOBJECTGLOBALS_HPP

#include <array>
#include <vector>
#include <functional>

#include <Function/Function.hpp>
#include <Constructs/Loop.hpp>
#include <Unreal/Common.hpp>
#include <Unreal/PrimitiveTypes.hpp>
#include <Unreal/NameTypes.hpp>
#include <Unreal/UnrealFlags.hpp>

namespace RC::Unreal
{
    class UObject;

    /** Concept describing the type of the pointer pointing to the UObject-derived object */
    template<typename SupposedUObject>
    concept UObjectPointerDerivative = std::is_convertible_v<SupposedUObject, UObject*>;

    /** Concept describing the type derived from the UObject */
    template<typename SupposedUObject>
    concept UObjectDerivative = std::is_convertible_v<SupposedUObject, UObject>;

    /** Casts the object to the provided type if possible, returns nullptr otherwise */
    template<UObjectDerivative CastResultType>
    auto Cast(UObject* Object) -> CastResultType*;

    // Adapted from UE source
    // This struct becomes deprecated in 4.26+ and as such is only used if <=4.25 is detected
#define StaticConstructObject_Internal_Params_Deprecated \
    UClass* InClass_,\
    UObject* InOuter_,\
    FName InName_,\
    EObjectFlags InFlags_,\
    EInternalObjectFlags InternalSetFlags_,\
    UObject* InTemplate_,\
    bool bCopyTransientsFromClassDefaults_,\
    FObjectInstancingGraph* InInstanceGraph_,\
    bool bAssumeTemplateIsArchetype_,\
    void* ExternalPackage_\

    // Struct From UE4.26 Source
    struct RC_UE_API FStaticConstructObjectParameters
    {
        /** The class of the object to create */
        /*const*/ class UClass* Class; // Making this non-const for the hook

        /** The object to create this object within (the Outer property for the new object will be set to the value specified here). */
        UObject* Outer;

        /** The name to give the new object.If no value(NAME_None) is specified, the object will be given a unique name in the form of ClassName_#. */
        FName Name;

        /** The ObjectFlags to assign to the new object. some flags can affect the behavior of constructing the object. */
        EObjectFlags SetFlags = RF_NoFlags;

        /** The InternalObjectFlags to assign to the new object. some flags can affect the behavior of constructing the object. */
        EInternalObjectFlags InternalSetFlags = EInternalObjectFlags::None;

        /** If true, copy transient from the class defaults instead of the pass in archetype ptr(often these are the same) */
        bool bCopyTransientsFromClassDefaults = false;

        /** If true, Template is guaranteed to be an archetype */
        bool bAssumeTemplateIsArchetype = false;

        /**
         * If specified, the property values from this object will be copied to the new object, and the new object's ObjectArchetype value will be set to this object.
         * If nullptr, the class default object is used instead.
         */
        UObject* Template = nullptr;

        /** Contains the mappings of instanced objects and components to their templates */
        struct FObjectInstancingGraph* InstanceGraph = nullptr;

        /** Assign an external Package to the created object if non-null */
        class UPackage* ExternalPackage = nullptr;

        // 5.0+
        void* SubobjectOverrides = nullptr;

    };
}

namespace RC::Unreal::UObjectGlobals
{

    static inline void* ANY_PACKAGE{reinterpret_cast<void*>(-1)};

    // Internal game functions
    struct GlobalState
    {
        static inline Function<UObject*(StaticConstructObject_Internal_Params_Deprecated)> StaticConstructObjectInternalDeprecated{};
        static inline Function<UObject*(const FStaticConstructObjectParameters&)> StaticConstructObjectInternal{};
    };

    // Setup internal game functions
    auto RC_UE_API SetupStaticConstructObjectInternalAddress(void* FunctionAddress) -> void;

    //Iterates object array and calls the provided function for each object
    auto ForEachUObject(const std::function<LoopAction(UObject* object, int32 object_index, int32 chunk_index)>& RawObject) -> void;

    // Internal work-around for not having access to UnrealVersion due to circ-inclusion
    auto RC_UE_API VersionIsAtMost(uint32_t Major, uint32_t Minor) -> bool;

    // Internal game function implementations
    // Slow implementation that's to be avoided whenever possible
    // This only exists for compatibility reasons with old Lua scripts
    // All params except 'orig_in_name' are also just there for compatibility reasons and do not have any effect
    auto StaticFindObjectImpl(UClass* Object, UObject* ChunkIndex, const wchar_t* ObjectIndex, bool bExactClass = false) -> UObject*;

    template<UObjectPointerDerivative ObjectType = UObject*>
    auto StaticFindObject(UClass* ObjectClass, UObject* InObjectPackage, const wchar_t* OrigInName, bool bExactClass = false) -> ObjectType
    {
        return static_cast<ObjectType>(StaticFindObjectImpl(ObjectClass, InObjectPackage, OrigInName, bExactClass));
    }
    template<UObjectPointerDerivative ObjectType = UObject*>
    auto StaticFindObject(UClass* ObjectClass, UObject* InObjectPackage, std::wstring_view OrigInName, bool bExactClass = false) -> ObjectType
    {
        return static_cast<ObjectType>(StaticFindObjectImpl(ObjectClass, InObjectPackage, OrigInName.data(), bExactClass));
    }

    template<UObjectPointerDerivative ObjectType = UObject*>
    auto StaticFindObject(UClass* ObjectClass, UObject* InObjectPackage, const std::wstring& OrigInName, bool bExactClass = false) -> ObjectType
    {
        return static_cast<ObjectType>(StaticFindObjectImpl(ObjectClass, InObjectPackage, OrigInName.c_str(), bExactClass));
    }

    template<UObjectPointerDerivative ObjectType = UObject*>
    auto StaticConstructObject(const FStaticConstructObjectParameters& Params) -> ObjectType
    {
        if (!GlobalState::StaticConstructObjectInternal.is_ready()) { return nullptr; }

        if (VersionIsAtMost(4, 25))
        {
            return static_cast<ObjectType>(GlobalState::StaticConstructObjectInternalDeprecated(
                    Params.Class,
                    Params.Outer,
                    Params.Name,
                    Params.SetFlags,
                    Params.InternalSetFlags,
                    Params.Template,
                    Params.bCopyTransientsFromClassDefaults,
                    Params.InstanceGraph,
                    Params.bAssumeTemplateIsArchetype,
                    Params.ExternalPackage
            ));
        }
        else
        {
            return static_cast<ObjectType>(GlobalState::StaticConstructObjectInternal(Params));
        }
    }

    // Custom Helpers -> START
    // Find the first instance of a class
    // Does not find ClassDefaultObjects (CDO) or non-instances of classes
    // Takes inheritance into account, that means you can give it an FName of "Controller" and
    // it will also find instances of "PlayerController" and any instances from any other derived class
    auto RC_UE_API FindFirstOf(FName Object) -> UObject*;
    auto RC_UE_API FindFirstOf(const wchar_t* ClassName) -> UObject*;
    auto RC_UE_API FindFirstOf(std::wstring_view ClassName) -> UObject*;
    auto RC_UE_API FindFirstOf(const std::wstring& ClassName) -> UObject*;
    auto RC_UE_API FindFirstOf(std::string_view ClassName) -> UObject*;
    auto RC_UE_API FindFirstOf(const std::string& ClassName) -> UObject*;

    // Find all instances of a class
    // Follows the same rules as 'find_first_of'
    auto RC_UE_API FindAllOf(FName SuperStruct, std::vector<UObject*>& ChunkIndex) -> void;
    auto RC_UE_API FindAllOf(const wchar_t* ClassName, std::vector<UObject*>& OutStorage) -> void;
    auto RC_UE_API FindAllOf(std::wstring_view ClassName, std::vector<UObject*>& OutStorage) -> void;
    auto RC_UE_API FindAllOf(const std::wstring& ClassName, std::vector<UObject*>& OutStorage) -> void;
    auto RC_UE_API FindAllOf(std::string_view ClassName, std::vector<UObject*>& OutStorage) -> void;
    auto RC_UE_API FindAllOf(const std::string& ClassName, std::vector<UObject*>& OutStorage) -> void;

    // Find a specified number of objects with the specified class (or none) and name (or none)
    // Must have at least either class or name, or both
    // Required & banned flags can be specified or left to the default (no required/banned flags)
    // Find one or specified amount of objects
    // Specify 0 for 'num_objects_to_find' to not limit to number of objects to find
    // The 'flags' parameters for the following functions are of type EObjectFlags
    auto RC_UE_API FindObjects(size_t Object, const FName ClassName, const FName ObjectShortName, std::vector<UObject*>& OutStorage, int32 RequiredFlags = {}, int32 BannedFlags = {}) -> void;
    auto RC_UE_API FindObjects(size_t NumObjectsToFind, const wchar_t* ClassName, const wchar_t* ObjectShortName, std::vector<UObject*>& OutStorage, int32 RequiredFlags = {}, int32 BannedFlags = {}) -> void;
    auto RC_UE_API FindObject(const FName ClassName, const FName ObjectShortName, int32 RequiredFlags = {}, int32 BannedFlags = {}) -> UObject*;
    auto RC_UE_API FindObject(const wchar_t* ClassName, const wchar_t* ObjectShortName, int32 RequiredFlags = {}, int32 BannedFlags = {}) -> UObject*;

    // Find all objects
    // The 'flags' parameters for the following functions are of type EObjectFlags
    auto RC_UE_API FindObjects(const FName ClassName, const FName ObjectShortName, std::vector<UObject*>& OutStorage, int32 RequiredFlags = {}, int32 BannedFlags = {}) -> void;
    auto RC_UE_API FindObjects(const wchar_t* ClassName, const wchar_t* ObjectShortName, std::vector<UObject*>& OutStorage, int32 RequiredFlags = {}, int32 BannedFlags = {}) -> void;
    // Custom Helpers -> END
}


#endif //RC_UNREAL_UOBJECTGLOBALS_HPP
