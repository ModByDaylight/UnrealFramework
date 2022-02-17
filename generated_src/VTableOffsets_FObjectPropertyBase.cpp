#include <cstdint>

#include <Unreal/UObject.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/FMemory.hpp>
#include <Unreal/Property/FObjectProperty.hpp>

namespace RC::Unreal
{
    uint32_t FObjectPropertyBase::VTableOffsets::Serialize = 0;
    uint32_t FObjectPropertyBase::VTableOffsets::AddReferencedObjects = 0;
    uint32_t FObjectPropertyBase::VTableOffsets::Identical = 0;
    uint32_t FObjectPropertyBase::VTableOffsets::CheckValidObject = 0;
    uint32_t FObjectPropertyBase::VTableOffsets::BeginDestroy = 0;
    uint32_t FObjectPropertyBase::VTableOffsets::PostDuplicate = 0;
    uint32_t FObjectPropertyBase::VTableOffsets::ExportTextItem = 0;
    uint32_t FObjectPropertyBase::VTableOffsets::SupportsNetSharedSerialization = 0;
    uint32_t FObjectPropertyBase::VTableOffsets::NetSerializeItem = 0;
    uint32_t FObjectPropertyBase::VTableOffsets::ImportText_Internal = 0;
    uint32_t FObjectPropertyBase::VTableOffsets::GetID = 0;
    uint32_t FObjectPropertyBase::VTableOffsets::InstanceSubobjects = 0;
    uint32_t FObjectPropertyBase::VTableOffsets::SameType = 0;
    uint32_t FObjectPropertyBase::VTableOffsets::SetObjectPropertyValue = 0;
    uint32_t FObjectPropertyBase::VTableOffsets::CopySingleValueToScriptVM = 0;
    uint32_t FObjectPropertyBase::VTableOffsets::CopyCompleteValueToScriptVM = 0;
    uint32_t FObjectPropertyBase::VTableOffsets::CopySingleValueFromScriptVM = 0;
    uint32_t FObjectPropertyBase::VTableOffsets::CopyCompleteValueFromScriptVM = 0;
    uint32_t FObjectPropertyBase::VTableOffsets::GetCPPTypeCustom = 0;
    uint32_t FObjectPropertyBase::VTableOffsets::LoadObjectPropertyValue = 0;
    uint32_t FObjectPropertyBase::VTableOffsets::GetObjectPropertyValue = 0;
    uint32_t FObjectPropertyBase::VTableOffsets::AllowCrossLevel = 0;
    uint32_t FObjectPropertyBase::VTableOffsets::FObjectPropertyBase_Destructor = 0;
    uint32_t FObjectPropertyBase::VTableOffsets::__vecDelDtor = 0;
}
