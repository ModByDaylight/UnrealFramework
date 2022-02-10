#include <cstdint>

#include <Unreal/UObject.hpp>
#include <Unreal/UScriptStruct.hpp>

namespace RC::Unreal
{
    uint32_t UObjectBaseUtility::VTableOffsets::CanBeClusterRoot = 0;
    uint32_t UObjectBaseUtility::VTableOffsets::CreateCluster = 0;
    uint32_t UObjectBaseUtility::VTableOffsets::CanBeInCluster = 0;
    uint32_t UObjectBaseUtility::VTableOffsets::OnClusterMarkedAsPendingKill = 0;
    uint32_t UObjectBaseUtility::VTableOffsets::__vecDelDtor = 0;
    uint32_t UObjectBaseUtility::VTableOffsets::AddToCluster = 0;
    uint32_t UObjectBaseUtility::VTableOffsets::UObjectBaseUtility_Destructor = 0;
}
