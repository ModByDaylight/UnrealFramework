#include <DynamicOutput/DynamicOutput.hpp>

#include <Unreal/UAssetRegistry.hpp>
#include <Unreal/UAssetRegistryHelpers.hpp>
#include <Unreal/UObjectArray.hpp>
#include <Unreal/UObjectGlobals.hpp>
#include <Unreal/UFunction.hpp>
#include <Unreal/UScriptStruct.hpp>

namespace RC::Unreal
{
    auto UAssetRegistry::GetAllAssets(TArray <FAssetData>& OutAssetData, bool bIncludeOnlyOnDiskAssets) -> bool
    {
        if (!Functions::get_all_assets.is_valid()) { return false; }

        GetAllAssets_Params params{OutAssetData, bIncludeOnlyOnDiskAssets};
        Functions::get_all_assets(this, params);

        // Copy here because we can't actually make the variable in the params struct a reference
        // This is because UE4 expects the TArray to be inlined into the struct
        // This may be a design problem with UE4SS, but it would be annoying to have to construct the params variable outside this wrapper function
        // With TArray it's not super slow because we can simply copy the pointer and the num/max integers
        // We don't own the data pointer, so it won't get deallocated by doing this
        OutAssetData.copy_fast(params.OutAssetData);
        return params.ReturnValue;
    }

    auto UAssetRegistry::load_all_assets_thread() -> void
    {
        set_assets_are_loading(true);

        UAssetRegistry* asset_registry = UAssetRegistryHelpers::GetAssetRegistry();
        if (!asset_registry)
        {
            Output::send(STR("Did not load assets because asset_registry was nullptr\n"));
            set_assets_are_loading(false);
            return;
        }

        TArray<FAssetData> all_assets{nullptr, 0, 0};
        asset_registry->GetAllAssets(all_assets, false);

        auto* asset_data_script_struct = UObjectGlobals::static_find_object<UScriptStruct*>(nullptr, nullptr, STR("/Script/CoreUObject.AssetData"));
        if (!asset_data_script_struct)
        {
            asset_data_script_struct = UObjectGlobals::static_find_object<UScriptStruct*>(nullptr, nullptr, STR("/Script/AssetRegistry.AssetData"));
        }

        if (!asset_data_script_struct)
        {
            Output::send(STR("Did not load assets because could not get size of AssetData ScriptStruct because the pointer was nullptr\n"));
        }

        all_assets.for_each([](FAssetData* asset) {
            if (!asset) { return LoopAction::Continue; }

            UObject* loaded_asset = UAssetRegistryHelpers::GetAsset(*asset);
            if (!loaded_asset) { return LoopAction::Continue; }

            if (auto* object_item = loaded_asset->get_object_item(); object_item)
            {
                // TODO: Undo this, it's very important or we leak multiple gigabytes of memory
                object_item->set_root_set();
                object_item->set_gc_keep();
            }

            return LoopAction::Continue;
        });

        set_assets_are_loading(false);
    }

    auto UAssetRegistry::load_all_assets() -> void
    {
        if (!assets_are_loading())
        {
            set_should_load_all_assets(true);
        }

        // Wait for all assets to load
        while (should_load_all_assets())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
}
