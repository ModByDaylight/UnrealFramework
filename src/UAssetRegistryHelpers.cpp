#include <DynamicOutput/DynamicOutput.hpp>
#include <Unreal/UAssetRegistryHelpers.hpp>
#include <Unreal/UFunction.hpp>

namespace RC::Unreal
{
    FAssetData UAssetRegistryHelpers::GetAsset_Params::InAssetData()
    {
        return InAssetData_Private;
    }

    UObject* UAssetRegistryHelpers::GetAsset_Params::ReturnValue()
    {
        return Helper::Casting::ptr_cast_deref<UObject*>(this, UAssetRegistryHelpers::StaticFunctions::get_asset.GetReturnValueOffset());
    }

    UAssetRegistryHelpers::GetAsset_Params::GetAsset_Params(const FAssetData& InAssetData) : InAssetData_Private(InAssetData)
    {
    }

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

    auto UAssetRegistryHelpers::GetAssetRegistry() -> FScriptInterface
    {
        if (!verify_self()) { return {}; }
        if (!StaticFunctions::get_asset_registry.is_valid()) { return {}; }

        GetAssetRegistry_Params params{};
        StaticFunctions::get_asset_registry(params);
        return params.ReturnValue;
    }

    auto UAssetRegistryHelpers::GetAsset(FAssetData& in_asset_data) -> UObject*
    {
        if (!verify_self()) { return nullptr; }
        if (!StaticFunctions::get_asset.is_valid()) { return nullptr; }

        GetAsset_Params params{in_asset_data};
        StaticFunctions::get_asset(params);
        return params.ReturnValue();
    }
}
