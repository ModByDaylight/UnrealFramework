#ifndef RC_UNREAL_FASSET_DATA_HPP
#define RC_UNREAL_FASSET_DATA_HPP

#include <Unreal/Common.hpp>
#include <Unreal/PrimitiveTypes.hpp>
#include <Unreal/UObjectMacros.hpp>
#include <Unreal/TypeChecker.hpp>
#include <Unreal/TArray.hpp>
#include <Unreal/Property/FSoftObjectProperty.hpp>
#include <Unreal/SharedPointer.hpp>

namespace RC::Unreal {
    class UScriptStruct;

    class FAssetDataTagMapSharedView {
        union {
            //FMapHandle Fixed;
            //FAssetDataTagMap* Loose;
            uint64 Bits = 0;
        };
    };

    struct FAssetBundleEntry {
        FName BundleName;
        TArray<FSoftObjectPath> BundleAssets;

        IMPLEMENT_UNREAL_TYPE(FAssetBundleData);
    };

    struct FAssetBundleData {
        TArray<FAssetBundleEntry> Bundles;

        IMPLEMENT_UNREAL_TYPE(FAssetBundleData);
    };

    namespace Internal {
        struct RC_UE_API FAssetData_UE427Plus {
            // Reflected
            FName ObjectPath;                           // Size: 0x8
            FName PackageName;                          // Size: 0x8
            FName PackagePath;                          // Size: 0x8
            FName AssetName;                            // Size: 0x8
            FName AssetClass;                           // Size: 0x8

            // Non-reflected
            FAssetDataTagMapSharedView TagsAndValues{}; // Size: 0x8
            TSharedPtr<FAssetBundleData, ESPMode::ThreadSafe> TaggedAssetBundles{};
            TArray<int32, TInlineAllocator<2>> ChunkIDs;// Size: 0x18
            uint32 PackageFlags{0};                     // Size: 0x4
        };

        struct RC_UE_API FAssetData_UE427Pre {
            // Reflected
            FName ObjectPath;                           // Size: 0x8
            FName PackageName;                          // Size: 0x8
            FName PackagePath;                          // Size: 0x8
            FName AssetName;                            // Size: 0x8
            FName AssetClass;                           // Size: 0x8

            // Non-reflected
            FAssetDataTagMapSharedView TagsAndValues{};   // Size: 0x8
            uint8_t padding[0x8]{};
            TArray<int32> ChunkIDs;                       // Size: 0x10
            uint32 PackageFlags{0};                       // Size: 0x4
        };
    }

    struct RC_UE_API FAssetData {
    private:
        friend class TypeChecker;
        static UScriptStruct* InternalStaticStruct;
        union {
            std::conditional_t<UNREAL_VERSION >= 4270, Internal::FAssetData_UE427Plus, Internal::FAssetData_UE427Pre> Data;
            uint64_t Alignment{};
        };
    public:
        FAssetData();
        FAssetData(const FAssetData& InData);
        ~FAssetData();
        FAssetData& operator=(const FAssetData& Other);

        FORCEINLINE FName ObjectPath() const { return Data.ObjectPath; }
        FORCEINLINE FName PackageName() const { return Data.PackageName; };
        FORCEINLINE FName PackagePath() const { return Data.PackagePath; };
        FORCEINLINE FName AssetName() const { return Data.AssetName; };
        FORCEINLINE FName AssetClass() const { return Data.AssetClass; };
        FORCEINLINE FAssetBundleData* TaggedAssetBundles() const { return Data.TaggedAssetBundles.Get(); };
        FORCEINLINE TArray<int32> ChunkIDs() const { return Data.ChunkIDs; };
        FORCEINLINE uint32 PackageFlags() const { return Data.PackageFlags; };

        FORCEINLINE void SetObjectPath(FName NewObjectPath) { Data.ObjectPath = NewObjectPath; };

        IMPLEMENT_UNREAL_TYPE(FAssetData);

        static UScriptStruct* StaticStruct();
    };
}

#endif //RC_UNREAL_FASSET_DATA_HPP
