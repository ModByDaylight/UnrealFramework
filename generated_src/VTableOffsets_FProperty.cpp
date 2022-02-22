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
    uint32_t FProperty::VTableOffsets::GetCPPTypeForwardDeclaration = 0;
    uint32_t FProperty::VTableOffsets::UProperty_Destructor = 0;
    uint32_t FProperty::VTableOffsets::DestroyValueInternal = 0;
    uint32_t FProperty::VTableOffsets::Serialize = 0;
    uint32_t FProperty::VTableOffsets::LinkInternal = 0;
    uint32_t FProperty::VTableOffsets::PassCPPArgsByRef = 0;
    uint32_t FProperty::VTableOffsets::PostDuplicate = 0;
    uint32_t FProperty::VTableOffsets::GetCPPMacroType = 0;
    uint32_t FProperty::VTableOffsets::Identical = 0;
    uint32_t FProperty::VTableOffsets::ContainsObjectReference = 0;
    uint32_t FProperty::VTableOffsets::GetCPPType = 0;
    uint32_t FProperty::VTableOffsets::InstanceSubobjects = 0;
    uint32_t FProperty::VTableOffsets::SerializeItem = 0;
    uint32_t FProperty::VTableOffsets::NetSerializeItem = 0;
    uint32_t FProperty::VTableOffsets::ExportTextItem = 0;
    uint32_t FProperty::VTableOffsets::ImportText_Internal = 0;
    uint32_t FProperty::VTableOffsets::CopyValuesInternal = 0;
    uint32_t FProperty::VTableOffsets::__vecDelDtor = 0;
    uint32_t FProperty::VTableOffsets::GetValueTypeHashInternal = 0;
    uint32_t FProperty::VTableOffsets::CopySingleValueToScriptVM = 0;
    uint32_t FProperty::VTableOffsets::CopyCompleteValueToScriptVM = 0;
    uint32_t FProperty::VTableOffsets::CopySingleValueFromScriptVM = 0;
    uint32_t FProperty::VTableOffsets::CopyCompleteValueFromScriptVM = 0;
    uint32_t FProperty::VTableOffsets::FProperty_Destructor = 0;
    uint32_t FProperty::VTableOffsets::ClearValueInternal = 0;
    uint32_t FProperty::VTableOffsets::InitializeValueInternal = 0;
    uint32_t FProperty::VTableOffsets::GetID = 0;
    uint32_t FProperty::VTableOffsets::GetMinAlignment = 0;
    uint32_t FProperty::VTableOffsets::ContainsWeakObjectReference = 0;
    uint32_t FProperty::VTableOffsets::EmitReferenceInfo = 0;
    uint32_t FProperty::VTableOffsets::SameType = 0;
    uint32_t FProperty::VTableOffsets::ConvertFromType = 0;
    uint32_t FProperty::VTableOffsets::IsPostLoadThreadSafe = 0;
    uint32_t FProperty::VTableOffsets::SupportsNetSharedSerialization = 0;
}
