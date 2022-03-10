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
    auto cast_object(UObject* object) -> CastResultType*;

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
        static inline Function<UObject*(StaticConstructObject_Internal_Params_Deprecated)> static_construct_object_internal_deprecated{};
        static inline Function<UObject*(const FStaticConstructObjectParameters&)> static_construct_object_internal{};
    };

    // Setup internal game functions
    auto RC_UE_API setup_static_construct_object_internal_address(void* function_address) -> void;

    //Iterates object array and calls the provided function for each object
    auto ForEachUObject(const std::function<LoopAction(UObject* object, int32 object_index, int32 chunk_index)>& callable) -> void;

    // Internal work-around for not having access to UnrealVersion due to circ-inclusion
    auto RC_UE_API version_is_atmost(uint32_t major_p, uint32_t minor_p) -> bool;

    // Internal game function implementations
    // Slow implementation that's to be avoided whenever possible
    // This only exists for compatibility reasons with old Lua scripts
    // All params except 'orig_in_name' are also just there for compatibility reasons and do not have any effect
    auto static_find_object_impl(UClass* object_class, UObject* in_object_package, const wchar_t* orig_in_name, bool exact_class = false) -> UObject*;

    template<UObjectPointerDerivative ObjectType = UObject*>
    auto static_find_object(UClass* object_class, UObject* in_object_package, const wchar_t* orig_in_name, bool exact_class = false) -> ObjectType
    {
        return static_cast<ObjectType>(static_find_object_impl(object_class, in_object_package, orig_in_name, exact_class));
    }
    template<UObjectPointerDerivative ObjectType = UObject*>
    auto static_find_object(UClass* object_class, UObject* in_object_package, std::wstring_view orig_in_name, bool exact_class = false) -> ObjectType
    {
        return static_cast<ObjectType>(static_find_object_impl(object_class, in_object_package, orig_in_name.data(), exact_class));
    }

    template<UObjectPointerDerivative ObjectType = UObject*>
    auto static_find_object(UClass* object_class, UObject* in_object_package, const std::wstring& orig_in_name, bool exact_class = false) -> ObjectType
    {
        return static_cast<ObjectType>(static_find_object_impl(object_class, in_object_package, orig_in_name.c_str(), exact_class));
    }

    template<UObjectPointerDerivative ObjectType = UObject*>
    auto static_construct_object(const FStaticConstructObjectParameters& params) -> ObjectType
    {
        if (!GlobalState::static_construct_object_internal.is_ready()) { return nullptr; }

        if (version_is_atmost(4, 25))
        {
            return static_cast<ObjectType>(GlobalState::static_construct_object_internal_deprecated(
                    params.Class,
                    params.Outer,
                    params.Name,
                    params.SetFlags,
                    params.InternalSetFlags,
                    params.Template,
                    params.bCopyTransientsFromClassDefaults,
                    params.InstanceGraph,
                    params.bAssumeTemplateIsArchetype,
                    params.ExternalPackage
            ));
        }
        else
        {
            return static_cast<ObjectType>(GlobalState::static_construct_object_internal(params));
        }
    }

    // Custom Helpers -> START
    // Find the first instance of a class
    // Does not find ClassDefaultObjects (CDO) or non-instances of classes
    // Takes inheritance into account, that means you can give it an FName of "Controller" and
    // it will also find instances of "PlayerController" and any instances from any other derived class
    auto RC_UE_API find_first_of(FName class_name) -> UObject*;
    auto RC_UE_API find_first_of(const wchar_t* class_name) -> UObject*;
    auto RC_UE_API find_first_of(std::wstring_view class_name) -> UObject*;
    auto RC_UE_API find_first_of(const std::wstring& class_name) -> UObject*;
    auto RC_UE_API find_first_of(std::string_view class_name) -> UObject*;
    auto RC_UE_API find_first_of(const std::string& class_name) -> UObject*;

    // Find all instances of a class
    // Follows the same rules as 'find_first_of'
    auto RC_UE_API find_all_of(FName class_name, std::vector<UObject*>& out_storage) -> void;
    auto RC_UE_API find_all_of(const wchar_t* class_name, std::vector<UObject*>& out_storage) -> void;
    auto RC_UE_API find_all_of(std::wstring_view class_name, std::vector<UObject*>& out_storage) -> void;
    auto RC_UE_API find_all_of(const std::wstring& class_name, std::vector<UObject*>& out_storage) -> void;
    auto RC_UE_API find_all_of(std::string_view class_name, std::vector<UObject*>& out_storage) -> void;
    auto RC_UE_API find_all_of(const std::string& class_name, std::vector<UObject*>& out_storage) -> void;

    // Find a specified number of objects with the specified class (or none) and name (or none)
    // Must have at least either class or name, or both
    // Required & banned flags can be specified or left to the default (no required/banned flags)
    // Find one or specified amount of objects
    // Specify 0 for 'num_objects_to_find' to not limit to number of objects to find
    // The 'flags' parameters for the following functions are of type EObjectFlags
    auto RC_UE_API find_objects(size_t num_objects_to_find, const FName class_name, const FName object_short_name, std::vector<UObject*>& out_storage, int32 required_flags = {}, int32 banned_flags = {}) -> void;
    auto RC_UE_API find_objects(size_t num_objects_to_find, const wchar_t* class_name, const wchar_t* object_short_name, std::vector<UObject*>& out_storage, int32 required_flags = {}, int32 banned_flags = {}) -> void;
    auto RC_UE_API find_object(const FName class_name, const FName object_short_name, int32 required_flags = {}, int32 banned_flags = {}) -> UObject*;
    auto RC_UE_API find_object(const wchar_t* class_name, const wchar_t* object_short_name, int32 required_flags = {}, int32 banned_flags = {}) -> UObject*;

    // Find all objects
    // The 'flags' parameters for the following functions are of type EObjectFlags
    auto RC_UE_API find_objects(const FName class_name, const FName object_short_name, std::vector<UObject*>& out_storage, int32 required_flags = {}, int32 banned_flags = {}) -> void;
    auto RC_UE_API find_objects(const wchar_t* class_name, const wchar_t* object_short_name, std::vector<UObject*>& out_storage, int32 required_flags = {}, int32 banned_flags = {}) -> void;
    // Custom Helpers -> END
}


#endif //RC_UNREAL_UOBJECTGLOBALS_HPP
