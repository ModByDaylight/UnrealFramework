#include <cstdint>

#include <Unreal/UObject.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/FMemory.hpp>
#include <Unreal/Property/FMulticastDelegateProperty.hpp>
#include <Unreal/Property/FNumericProperty.hpp>
#include <Unreal/Property/FObjectProperty.hpp>
#include <Unreal/FProperty.hpp>

namespace RC::Unreal
{
    uint32_t UObjectBase::VTableOffsets::UObjectBase_Destructor = 0;
    uint32_t UObjectBase::VTableOffsets::__vecDelDtor = 0;
    uint32_t UObjectBase::VTableOffsets::RegisterDependencies = 0;
    uint32_t UObjectBase::VTableOffsets::DeferredRegister = 0;
}
