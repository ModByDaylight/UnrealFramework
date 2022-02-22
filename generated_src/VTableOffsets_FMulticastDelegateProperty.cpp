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
    uint32_t FMulticastDelegateProperty::VTableOffsets::AddDelegate = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::Serialize = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::Identical = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::ContainsObjectReference = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::GetCPPType = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::GetMulticastDelegate = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::BeginDestroy = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::InstanceSubobjects = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::SerializeItem = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::ExportTextItem = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::NetSerializeItem = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::ImportText_Internal = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::ContainsWeakObjectReference = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::SameType = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::UMulticastDelegateProperty_Destructor = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::__vecDelDtor = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::FMulticastDelegateProperty_Destructor = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::GetCPPTypeForwardDeclaration = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::ConvertFromType = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::SetMulticastDelegate = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::RemoveDelegate = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::ClearDelegate = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::GetInvocationList = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::EmitReferenceInfo = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::AddReferencedObjects = 0;
    uint32_t FMulticastDelegateProperty::VTableOffsets::PostDuplicate = 0;
}
