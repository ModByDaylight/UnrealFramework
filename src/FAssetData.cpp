#include <Unreal/FAssetData.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UScriptStruct.hpp>

namespace RC::Unreal
{
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(FAssetData)

    int32 FAssetData::StaticSize_Private{-1};

    FAssetData::FAssetData() = default;

    FAssetData::FAssetData(const FAssetData& Other)
    {
        if (Version::is_below(4, 17)) { throw std::runtime_error{"FAssetData only available in 4.17+"}; }

        if (Version::is_atleast(4, 27))
        {
            auto* TypedThis = std::bit_cast<FAssetData427Plus*>(this);
            auto* TypedOther = std::bit_cast<FAssetData427Plus*>(&Other);

            TypedThis->ObjectPath = TypedOther->ObjectPath;
            TypedThis->PackageName = TypedOther->PackageName;
            TypedThis->PackagePath = TypedOther->PackagePath;
            TypedThis->AssetName = TypedOther->AssetName;
            TypedThis->AssetClass = TypedOther->AssetClass;
            TypedThis->TagsAndValues = TypedOther->TagsAndValues;
            TypedThis->TaggedAssetBundles = TypedOther->TaggedAssetBundles;
            TypedThis->ChunkIDs = TypedOther->ChunkIDs;
            TypedThis->PackageFlags = TypedOther->PackageFlags;
        }
        else
        {
            auto* TypedThis = std::bit_cast<FAssetDataPre427*>(this);
            auto* TypedOther = std::bit_cast<FAssetDataPre427*>(&Other);

            TypedThis->ObjectPath = TypedOther->ObjectPath;
            TypedThis->PackageName = TypedOther->PackageName;
            TypedThis->PackagePath = TypedOther->PackagePath;
            TypedThis->AssetName = TypedOther->AssetName;
            TypedThis->AssetClass = TypedOther->AssetClass;
            TypedThis->TagsAndValues = TypedOther->TagsAndValues;
            //TypedThis->TaggedAssetBundles = TypedOther->TaggedAssetBundles;
            TypedThis->ChunkIDs = TypedOther->ChunkIDs;
            TypedThis->PackageFlags = TypedOther->PackageFlags;
        }
    }

    FName FAssetData::ObjectPath()
    {
        if (Version::is_below(4, 17)) { throw std::runtime_error{"FAssetData only available in 4.17+"}; }

        if (Version::is_atleast(4, 27))
        {
            return std::bit_cast<FAssetData427Plus*>(this)->ObjectPath;
        }
        else
        {
            return std::bit_cast<FAssetDataPre427*>(this)->ObjectPath;
        }
    }

    FName FAssetData::PackageName()
    {
        if (Version::is_below(4, 17)) { throw std::runtime_error{"FAssetData only available in 4.17+"}; }

        if (Version::is_atleast(4, 27))
        {
            return std::bit_cast<FAssetData427Plus*>(this)->PackageName;
        }
        else
        {
            return std::bit_cast<FAssetDataPre427*>(this)->PackageName;
        }
    }

    FName FAssetData::PackagePath()
    {
        if (Version::is_below(4, 17)) { throw std::runtime_error{"FAssetData only available in 4.17+"}; }

        if (Version::is_atleast(4, 27))
        {
            return std::bit_cast<FAssetData427Plus*>(this)->PackagePath;
        }
        else
        {
            return std::bit_cast<FAssetDataPre427*>(this)->PackagePath;
        }
    }

    FName FAssetData::AssetName()
    {
        if (Version::is_below(4, 17)) { throw std::runtime_error{"FAssetData only available in 4.17+"}; }

        if (Version::is_atleast(4, 27))
        {
            return std::bit_cast<FAssetData427Plus*>(this)->AssetName;
        }
        else
        {
            return std::bit_cast<FAssetDataPre427*>(this)->AssetName;
        }
    }

    FName FAssetData::AssetClass()
    {
        if (Version::is_below(4, 17)) { throw std::runtime_error{"FAssetData only available in 4.17+"}; }

        if (Version::is_atleast(4, 27))
        {
            return std::bit_cast<FAssetData427Plus*>(this)->AssetClass;
        }
        else
        {
            return std::bit_cast<FAssetDataPre427*>(this)->AssetClass;
        }
    }

    FAssetDataTagMapSharedView FAssetData::TagsAndValues()
    {
        if (Version::is_below(4, 17)) { throw std::runtime_error{"FAssetData only available in 4.17+"}; }

        if (Version::is_atleast(4, 27))
        {
            return std::bit_cast<FAssetData427Plus*>(this)->TagsAndValues;
        }
        else
        {
            return std::bit_cast<FAssetDataPre427*>(this)->TagsAndValues;
        }
    }

    FAssetBundleData* FAssetData::TaggedAssetBundles()
    {
        if (Version::is_below(4, 17)) { throw std::runtime_error{"FAssetData only available in 4.17+"}; }

        if (Version::is_atleast(4, 27))
        {
            return std::bit_cast<FAssetData427Plus*>(this)->TaggedAssetBundles;
        }
        else
        {
            throw std::runtime_error{"FAssetData::TaggedAssetBundles is not available in <4.27"};
        }
    }

    /*TArray<int32>*/void FAssetData::ChunkIDs()
    {
        // This isn't implemented because we need some sort of abstraction of the return value which we don't have
        // The reason we need this is because:
        // In <4.27, the return type was TArray<int32>, and in 4.27+ it's TArray<int32, TInlineAllocator<2>>
        throw std::runtime_error{"FAssetData:ChunkIDs is not implemented"};
    }

    TArray<int32>& FAssetData::OldChunkIDsUnsafe()
    {
        if (Version::is_below(4, 17)) { throw std::runtime_error{"FAssetData only available in 4.17+"}; }

        if (Version::is_atleast(4, 27))
        {
            throw std::runtime_error{"FAssetData::OldChunkIDsUnsafe can only be used in pre-4.27"};
        }
        else
        {
            return std::bit_cast<FAssetDataPre427*>(this)->ChunkIDs;
        }
    }

    TArray<int32, TInlineAllocator<2>>& FAssetData::NewChunkIDsUnsafe()
    {
        if (Version::is_below(4, 17)) { throw std::runtime_error{"FAssetData only available in 4.17+"}; }

        if (Version::is_atleast(4, 27))
        {
            return std::bit_cast<FAssetData427Plus*>(this)->ChunkIDs;
        }
        else
        {
            throw std::runtime_error{"FAssetData::NewChunkIDsUnsafe can only be used in 4.27+"};
        }
    }

    uint32 FAssetData::PackageFlags()
    {
        if (Version::is_below(4, 17)) { throw std::runtime_error{"FAssetData only available in 4.17+"}; }

        if (Version::is_atleast(4, 27))
        {
            return std::bit_cast<FAssetData427Plus*>(this)->PackageFlags;
        }
        else
        {
            return std::bit_cast<FAssetDataPre427*>(this)->PackageFlags;
        }
    }

    int32 FAssetData::StaticSize()
    {
        if (Version::is_below(4, 17)) { throw std::runtime_error{"FAssetData only available in 4.17+"}; }

        if (StaticSize_Private == -1)
        {
            StaticSize_Private = std::bit_cast<UScriptStruct*>(FAssetData::static_class())->GetSize();
        }

        return StaticSize_Private;
    }
}
