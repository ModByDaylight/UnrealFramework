#include <cstdint>

#include <Unreal/UObject.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/FOutputDevice.hpp>
#include <Unreal/FMemory.hpp>
#include <Unreal/Property/FMulticastDelegateProperty.hpp>
#include <Unreal/Property/FNumericProperty.hpp>
#include <Unreal/Property/FObjectProperty.hpp>
#include <Unreal/FProperty.hpp>

namespace RC::Unreal
{
    uint32_t FOutputDevice::VTableOffsets::CanBeUsedOnAnyThread = 0;
    uint32_t FOutputDevice::VTableOffsets::FOutputDevice_Destructor = 0;
    uint32_t FOutputDevice::VTableOffsets::Serialize = 0;
    uint32_t FOutputDevice::VTableOffsets::Dump = 0;
    uint32_t FOutputDevice::VTableOffsets::Flush = 0;
    uint32_t FOutputDevice::VTableOffsets::TearDown = 0;
    uint32_t FOutputDevice::VTableOffsets::__vecDelDtor = 0;
    uint32_t FOutputDevice::VTableOffsets::IsMemoryOnly = 0;
    uint32_t FOutputDevice::VTableOffsets::CanBeUsedOnMultipleThreads = 0;
}
