#include <DynamicOutput/DynamicOutput.hpp>
#include <Unreal/UAssetRegistryHelpers.hpp>
#include <Unreal/UFunction.hpp>

namespace RC::Unreal
{
    auto UAssetRegistryHelpers::verify_self() -> bool
    {
        if (!self)
        {
            self = UObjectGlobals::static_find_object<UAssetRegistryHelpers*>(nullptr, nullptr, STR("/Script/AssetRegistry.Default__AssetRegistryHelpers"));
            if (!self)
            {
                Output::send(STR("Was unable to locate AssetRegistryHelpers\n"));
                return false;
            }
        }

        return true;
    }

    auto UAssetRegistryHelpers::GetAssetRegistry() -> UAssetRegistry*
    {
        if (!verify_self()) { return nullptr; }
        if (!StaticFunctions::get_asset_registry.is_valid()) { return nullptr; }

        GetAssetRegistry_Params params{nullptr};
        StaticFunctions::get_asset_registry(params);
        return params.ReturnValue;
    }

    auto UAssetRegistryHelpers::GetAsset(FAssetData& in_asset_data) -> UObject*
    {
        if (!verify_self()) { return nullptr; }
        if (!StaticFunctions::get_asset.is_valid()) { return nullptr; }

        GetAsset_Params params{in_asset_data};
        StaticFunctions::get_asset(params);
        return params.ReturnValue;
    }
}
