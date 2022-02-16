#include <Unreal/FAssetData.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UScriptStruct.hpp>

namespace RC::Unreal
{
    int32 FAssetData::get_size_in_container()
    {
        return std::bit_cast<UScriptStruct*>(FAssetData::static_class())->GetSize();
    }
}
