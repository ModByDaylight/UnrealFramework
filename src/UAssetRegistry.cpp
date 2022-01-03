#include <Unreal/UAssetRegistry.hpp>
#include <Unreal/UFunction.hpp>

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
}
