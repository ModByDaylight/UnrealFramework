#include <cstdint>

#include <Unreal/UObject.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/FMemory.hpp>

namespace RC::Unreal
{
    uint32_t FMalloc::VTableOffsets::DumpAllocatorStats = 0;
    uint32_t FMalloc::VTableOffsets::Malloc = 0;
    uint32_t FMalloc::VTableOffsets::FMalloc_Destructor = 0;
    uint32_t FMalloc::VTableOffsets::Trim = 0;
    uint32_t FMalloc::VTableOffsets::ValidateHeap = 0;
    uint32_t FMalloc::VTableOffsets::Realloc = 0;
    uint32_t FMalloc::VTableOffsets::ClearAndDisableTLSCachesOnCurrentThread = 0;
    uint32_t FMalloc::VTableOffsets::QuantizeSize = 0;
    uint32_t FMalloc::VTableOffsets::Free = 0;
    uint32_t FMalloc::VTableOffsets::GetAllocationSize = 0;
    uint32_t FMalloc::VTableOffsets::SetupTLSCachesOnCurrentThread = 0;
    uint32_t FMalloc::VTableOffsets::InitializeStatsMetadata = 0;
    uint32_t FMalloc::VTableOffsets::Exec = 0;
    uint32_t FMalloc::VTableOffsets::UpdateStats = 0;
    uint32_t FMalloc::VTableOffsets::GetAllocatorStats = 0;
    uint32_t FMalloc::VTableOffsets::IsInternallyThreadSafe = 0;
    uint32_t FMalloc::VTableOffsets::GetDescriptiveName = 0;
    uint32_t FMalloc::VTableOffsets::__vecDelDtor = 0;
    uint32_t FMalloc::VTableOffsets::TryMalloc = 0;
    uint32_t FMalloc::VTableOffsets::TryRealloc = 0;
}
