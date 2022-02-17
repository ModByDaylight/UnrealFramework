#include <Unreal/FAssetData.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UScriptStruct.hpp>

namespace RC::Unreal
{
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(FAssetData)

    int32 FAssetData::get_size_in_container()
    {
        return std::bit_cast<UScriptStruct*>(FAssetData::static_class())->GetSize();
    }
}
