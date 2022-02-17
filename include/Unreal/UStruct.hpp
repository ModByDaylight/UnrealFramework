#ifndef RC_UNREAL_USTRUCT_HPP
#define RC_UNREAL_USTRUCT_HPP

#include <cstdint>
#include <stdexcept>
#include <functional>

#include <Constructs/Loop.hpp>

#include <Unreal/StaticOffsetFinder.hpp>
#include <Unreal/UObjectGlobals.hpp>
#include <Unreal/FField.hpp>
#include <Unreal/UField.hpp>

namespace RC::Unreal
{
    class FProperty;

    class RC_UE_API UStruct : public UField
    {
        DECLARE_EXTERNAL_OBJECT_CLASS(UStruct, CoreUObject)

        friend class StaticOffsetFinder;

    protected:
        /**
         * Returns the linked list of UField* objects defined on this struct
         * This will include both UFunction and UProperty objects on UE versions before 4.25,
         * afterwards it will only include functions
         */
        auto get_children() -> UField*;

        /**
         * Returns the linked list of FField* objects defined on this struct
         * As of modern UE versions, only FField objects in this list are FProperty objects
         *
         * Will throw the exception if UE version is below 4.25 since FField did not exist back then
         */
        auto get_child_properties() -> FField*;
    public:
        /**
         * Returns the parent struct of this struct
         * It might totally return nullptr when used on script structs or
         * UObject itself, since it does not have a superclass
         */
        auto get_super_struct() -> UStruct*;

        /**
         * Returns the total size of this object's properties
         * This is effectively the size of the object in memory
         */
        auto get_properties_size() -> int32_t;

        /**
         * Returns the alignment of this struct
         * Most of the objects and structs do not specify alignment requirements though
         */
        auto get_min_alignment() -> int32_t;

        /**
         * Checks if this struct is a child of the provided one
         */
        auto is_child_of(UStruct* ustruct) -> bool;

        template<UObjectDerivative UObjectDerivedType>
        auto is_child_of() -> bool
        {
            return is_child_of(UObjectDerivedType::static_class());
        }

        /**
         * Iterates properties of this object and calls the provided function on each of them
         * Will stop the iteration once the callable returns Break
         */
        auto for_each_property(const std::function<LoopAction(FProperty*)>& callable) -> void;

        /**
         * Iterates the functions of the object and calls the provided function on each of them
         * Will stop the iteration once the callable returns Break
         */
        auto for_each_function(const std::function<LoopAction(UFunction*)>& callable) -> void;

        /**
         * Iterates the super structs of this object and calls the provided function on each of them
         * Will stop the iteration once the callable returns Break
         */
        using ForEachSuperStructCallable = std::function<LoopAction(UStruct*)>;
        auto for_each_super_struct(const ForEachSuperStructCallable& callable) -> void;

        /**
         * Iterates every property in every super struct and calls the provided function on each of them
         * Will stop the iteration once the callable returns Break
         */
        using ForEachPropertyInChainCallable = std::function<LoopAction(FProperty*)>;
        auto ForEachPropertyInChain(const ForEachPropertyInChainCallable& callable) -> void;

        /**
         * Finds a property by FName
         * Returns the property or nullptr if one isn't found
         */
        FProperty* FindProperty(FName PropertyName);
    };
}

#endif //RC_UNREAL_USTRUCT_HPP
