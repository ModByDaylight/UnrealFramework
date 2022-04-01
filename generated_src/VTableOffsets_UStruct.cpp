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
    uint32_t UStruct::VTableOffsets::PostLoad = 0;
    uint32_t UStruct::VTableOffsets::UStruct_Destructor = 0;
    uint32_t UStruct::VTableOffsets::Serialize = 0;
    uint32_t UStruct::VTableOffsets::FinishDestroy = 0;
    uint32_t UStruct::VTableOffsets::RegisterDependencies = 0;
    uint32_t UStruct::VTableOffsets::Link = 0;
    uint32_t UStruct::VTableOffsets::DestroyStruct = 0;
    uint32_t UStruct::VTableOffsets::AddCppProperty = 0;
    uint32_t UStruct::VTableOffsets::SerializeTaggedProperties = 0;
    uint32_t UStruct::VTableOffsets::GetInheritanceSuper = 0;
    uint32_t UStruct::VTableOffsets::SerializeBin = 0;
    uint32_t UStruct::VTableOffsets::InitializeStruct = 0;
    uint32_t UStruct::VTableOffsets::SerializeExpr = 0;
    uint32_t UStruct::VTableOffsets::TagSubobjects = 0;
    uint32_t UStruct::VTableOffsets::GetPrefixCPP = 0;
    uint32_t UStruct::VTableOffsets::SetSuperStruct = 0;
    uint32_t UStruct::VTableOffsets::__vecDelDtor = 0;
    uint32_t UStruct::VTableOffsets::SerializeSuperStruct = 0;
    uint32_t UStruct::VTableOffsets::PropertyNameToDisplayName = 0;
    uint32_t UStruct::VTableOffsets::FindPropertyNameFromGuid = 0;
    uint32_t UStruct::VTableOffsets::FindPropertyGuidFromName = 0;
    uint32_t UStruct::VTableOffsets::ArePropertyGuidsAvailable = 0;
    uint32_t UStruct::VTableOffsets::GetPreloadDependencies = 0;
    uint32_t UStruct::VTableOffsets::CustomFindProperty = 0;
    uint32_t UStruct::VTableOffsets::GetAuthoredNameForField = 0;
    uint32_t UStruct::VTableOffsets::IsStructTrashed = 0;
}
