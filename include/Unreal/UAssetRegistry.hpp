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

    public:
        auto GetAllAssets(TArray<FAssetData>& OutAssetData, bool bIncludeOnlyOnDiskAssets) -> bool;
    };
}

#endif //RC_UNREAL_UASSETREGISTRY_HPP
