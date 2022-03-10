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
    uint32_t FField::VTableOffsets::AddReferencedObjects = 0;
    uint32_t FField::VTableOffsets::FField_Destructor = 0;
    uint32_t FField::VTableOffsets::GetPreloadDependencies = 0;
    uint32_t FField::VTableOffsets::GetInnerFieldByName = 0;
    uint32_t FField::VTableOffsets::Serialize = 0;
    uint32_t FField::VTableOffsets::PostLoad = 0;
    uint32_t FField::VTableOffsets::AddCppProperty = 0;
    uint32_t FField::VTableOffsets::BeginDestroy = 0;
    uint32_t FField::VTableOffsets::Bind = 0;
    uint32_t FField::VTableOffsets::PostDuplicate = 0;
    uint32_t FField::VTableOffsets::__vecDelDtor = 0;
    uint32_t FField::VTableOffsets::GetInnerFields = 0;
}
