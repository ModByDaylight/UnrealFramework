#ifndef RC_UNREAL_FASSET_DATA_HPP
#define RC_UNREAL_FASSET_DATA_HPP

#include <Unreal/Common.hpp>
#include <Unreal/TArray.hpp>

namespace RC::Unreal
{
    // Size in UE4.25 according to Rider: 84 (Is this dec or hex ? I'm thinking it's dec, but I'm not 100% sure)
    // Size in UE4.25 according to 'ScriptStruct::get_size()': 0x50
    // 0x4C currently (0x50 if you take into account alignment, sizeof(FAssetData) in UE4SS gives 0x50)
    struct RC_UE_API FAssetData
    {
        static inline class UScriptStruct* m_static_obj{};
        auto static set_static_obj_ptr(class UScriptStruct* ptr)
        {
            m_static_obj = ptr;
        }

        auto static static_class() -> class UScriptStruct*
        {
            if (!m_static_obj) { throw std::runtime_error{"[FAssetData::static_class] m_static_obj is nullptr"}; }

            return m_static_obj;
        }

        using FAssetDataTagMapSharedView = void*; // <-- Wrapper to a pointer to a map

        // Reflected
        FName ObjectPath;                           // Size: 0x8
        FName PackageName;                          // Size: 0x8
        FName PackagePath;                          // Size: 0x8
        FName AssetName;                            // Size: 0x8
        FName AssetClass;                           // Size: 0x8

        // Non-reflected
        // It gets bad below here because of changes that the ue devs made
        // I've put notes for each member variable where I noticed a changed
        // It looks like FAssetData needs to be put into a VC because of this
        // We really only need the reflected data but we also need the size so that the TArray for_each loop works properly
        // T size of '/Script/CoreUObject.AssetData' does take into account the unreflected variables, so we could use that instead of the hardcoded struct for the size
        FAssetDataTagMapSharedView TagsAndValues;   // Size: 0x8
        // This padding might be 'TSharedPtr<FAssetBundleData, ESPMode::ThreadSafe> TaggedAssetBundles' which was added in 4.27
        uint8_t padding[0x8]; // Appears to be right, but how am I missing 8 bytes ?
        // The ChunkIDs array allocator was changed to 'TInlineAllocator<2>' in 4.27
        TArray<int32_t> ChunkIDs;                   // Size: 0x10
        uint32_t PackageFlags;                      // Size: 0x4
    };
}

#endif //RC_UNREAL_FASSET_DATA_HPP
