#ifndef RC_UNREAL_USTRUCT_HPP
#define RC_UNREAL_USTRUCT_HPP

#include <cstdint>
#include <stdexcept>
#include <functional>

#include <Constructs/Loop.hpp>

#include <Unreal/StaticOffsetFinder.hpp>
#include <Unreal/UObjectGlobals.hpp>
#include <Unreal/UnrealCoreStructs.hpp>
#include <Unreal/FField.hpp>
#include <Unreal/UField.hpp>
#include <Unreal/Script.hpp>

namespace RC::Unreal
{
    class FProperty;

    class RC_UE_API UStruct : public UField
    {
        DECLARE_EXTERNAL_OBJECT_CLASS(UStruct, CoreUObject)

    public:
        using LinkSignature = void(UStruct* Context, FArchive& Ar, bool bRelinkExistingProperties);
        static Function<LinkSignature> LinkInternal;

    protected:
        friend class StaticOffsetFinder;

    public:
#include <VTableOffsets_UStruct.hpp>

    public:
        // Wrappers for virtual engine functions
        UStruct* GetInheritanceSuper() const;
        void Link(FArchive& Ar, bool bRelinkExistingProperties);
        void SerializeBin(FArchive& Ar, void* Data) const;
        struct FStructuredArchive { enum class FSlot {}; }; // Remove if/when we have a FStructuredArchive implementation, for now, probably a bad idea to call
        //void SerializeBin(FStructuredArchive::FSlot Slot, void* Data) const;
        void SerializeTaggedProperties(FArchive& Ar, uint8* Data, UStruct* DefaultsStruct, uint8* Defaults, const UObject* BreakRecursionIfFullyLoad = nullptr) const;
        //void SerializeTaggedProperties(FStructuredArchive::FSlot Slot, uint8* Data, UStruct* DefaultsStruct, uint8* Defaults, const UObject* BreakRecursionIfFullyLoad = nullptr) const;
        void InitializeStruct(void* Dest, int32 ArrayDim = 1) const;
        void DestroyStruct(void* Dest, int32 ArrayDim = 1) const;
        FProperty* CustomFindProperty(const FName InName) const;
        EExprToken SerializeExpr(int32& iCode, FArchive& Ar);
        const TCHAR* GetPrefixCPP() const;
        void SetSuperStruct(UStruct* NewSuperStruct);
        FString PropertyNameToDisplayName(FName InName) const;
        FString GetAuthoredNameForField(const UField* Field) const;
        //FString GetAuthoredNameForField(const FField* Field) const;
        bool IsStructTrashed() const;
        FName FindPropertyNameFromGuid(const FGuid& PropertyGuid) const;
        FGuid FindPropertyGuidFromName(const FName InName) const;
        bool ArePropertyGuidsAvailable() const;

    protected:
        /**
         * Returns the linked list of UField* objects defined on this struct
         * This will include both UFunction and UProperty objects on UE versions before 4.25,
         * afterwards it will only include functions
         */
        auto GetChildren() -> UField*;

        /**
         * Returns the linked list of FField* objects defined on this struct
         * As of modern UE versions, only FField objects in this list are FProperty objects
         *
         * Will throw the exception if UE version is below 4.25 since FField did not exist back then
         */
        auto GetChildProperties() -> FField*;
    public:
        /**
         * Returns the parent struct of this struct
         * It might totally return nullptr when used on script structs or
         * UObject itself, since it does not have a superclass
         */
        auto GetSuperStruct() -> UStruct*;
        auto GetSuperStruct() const -> const UStruct*;

        /**
         * Returns the total size of this object's properties
         * This is effectively the size of the object in memory
         */
        auto GetPropertiesSize() -> int32_t;

        /**
         * Returns the alignment of this struct
         * Most of the objects and structs do not specify alignment requirements though
         */
        auto GetMinAlignment() -> int32_t;

        /**
         * Checks if this struct is a child of the provided one
         */
        auto IsChildOf(UStruct* Struct) const -> bool;

        template<UObjectDerivative UObjectDerivedType>
        auto IsChildOf() const -> bool
        {
            return IsChildOf(UObjectDerivedType::StaticClass());
        }

        /**
         * Iterates properties of this object and calls the provided function on each of them
         * Will stop the iteration once the callable returns Break
         */
        auto ForEachProperty(const std::function<LoopAction(FProperty*)>& Callable) -> void;

        /**
         * Iterates the functions of the object and calls the provided function on each of them
         * Will stop the iteration once the callable returns Break
         */
        auto ForEachFunction(const std::function<LoopAction(UFunction *)>& Callable) -> void;

        /**
         * Iterates the super structs of this object and calls the provided function on each of them
         * Will stop the iteration once the callable returns Break
         */
        using ForEachSuperStructCallable = std::function<LoopAction(UStruct*)>;
        auto ForEachSuperStruct(const ForEachSuperStructCallable& Callable) -> void;

        /**
         * Iterates every property in every super struct and calls the provided function on each of them
         * Will stop the iteration once the callable returns Break
         */
        using ForEachPropertyInChainCallable = std::function<LoopAction(FProperty*)>;
        auto ForEachPropertyInChain(const ForEachPropertyInChainCallable& Property) -> void;

        /**
         * Finds a property by FName
         * Returns the property or nullptr if one isn't found
         */
        FProperty* FindProperty(FName PropertyName);

        /**
         * Returns true if either 'ChildProperties' or 'Children' is non-nullptr
         */
        bool HasChildren();

        FProperty* GetFirstProperty();
    };
}

#endif //RC_UNREAL_USTRUCT_HPP
