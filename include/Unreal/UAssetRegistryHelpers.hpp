#ifndef RC_UNREAL_UASSETREGISTRYHELPERS_HPP
#define RC_UNREAL_UASSETREGISTRYHELPERS_HPP

#include <Unreal/Common.hpp>
#include <Unreal/ReflectedFunction.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/FAssetData.hpp>

namespace RC::Unreal
{
    class UAssetRegistry;

    class RC_API UAssetRegistryHelpers : public UObject
    {
    public:
        using StaticReflectedFunction = StaticReflectedFunctionBase<UAssetRegistryHelpers>;

        static inline UAssetRegistryHelpers* self{nullptr};
        struct StaticFunctions
        {
            static inline StaticReflectedFunction get_asset_registry{STR("/Script/AssetRegistry.AssetRegistryHelpers:GetAssetRegistry")};
            static inline StaticReflectedFunction get_asset{STR("/Script/AssetRegistry.AssetRegistryHelpers:GetAsset")};
        };

    public:
        struct GetAssetRegistry_Params
        {
            UAssetRegistry* ReturnValue; // 0x0
        };

        struct GetAsset_Params
        {
            FAssetData InAssetData; // 0x0
            UObject* ReturnValue; // 0x50
        };

    private:
        auto static verify_self() -> bool;

    public:
        auto static GetAssetRegistry() -> UAssetRegistry*;
        auto static GetAsset(FAssetData& in_asset_data) -> UObject*;
    };
}

#endif //RC_UNREAL_UASSETREGISTRYHELPERS_HPP
