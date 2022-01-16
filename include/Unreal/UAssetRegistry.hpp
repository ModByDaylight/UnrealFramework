#ifndef RC_UNREAL_UASSETREGISTRY_HPP
#define RC_UNREAL_UASSETREGISTRY_HPP

#include <Unreal/Common.hpp>
#include <Unreal/ReflectedFunction.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/TArray.hpp>
#include <Unreal/FAssetData.hpp>

namespace RC::Unreal
{
    class RC_API UAssetRegistry : public UObject
    {
    public:
        struct Functions
        {
            static inline InstancedReflectedFunction get_all_assets{STR("/Script/AssetRegistry.AssetRegistry:GetAllAssets")};
        };

    public:
        struct GetAllAssets_Params
        {
            TArray<FAssetData> OutAssetData; // 0x0 // Out-param
            bool bIncludeOnlyOnDiskAssets; // 0x10
            bool ReturnValue; // 0x11
        };

    private:
        struct StaticStorage
        {
            static inline bool assets_are_loading{false};
            static inline bool should_load_all_assets{false};
        };

    public:
        auto GetAllAssets(TArray<FAssetData>& OutAssetData, bool bIncludeOnlyOnDiskAssets) -> bool;

        // Custom functions -> START
        auto static assets_are_loading() -> bool { return StaticStorage::assets_are_loading; }
        auto static should_load_all_assets() -> bool { return StaticStorage::should_load_all_assets; }
        auto static set_assets_are_loading(bool new_value) -> void { StaticStorage::assets_are_loading = new_value; }
        auto static set_should_load_all_assets(bool new_value) -> void { StaticStorage::should_load_all_assets = new_value; }

    private:
        // Internal function, do not use!
        // Call 'load_all_assets()' to safely load all assets regardless of thread context
        // This function is responsible for loading all assets into GUObjectArray
        // Must be called from within the game thread
        auto static load_all_assets_thread() -> void;
        auto friend hooked_process_event(UObject*, UFunction*, void*) -> void;

    public:
        // Helper function to load all assets from the AssetRegistry into GUObjectArray
        auto static load_all_assets() -> void;
        // Custom functions -> END
    };
}

#endif //RC_UNREAL_UASSETREGISTRY_HPP
