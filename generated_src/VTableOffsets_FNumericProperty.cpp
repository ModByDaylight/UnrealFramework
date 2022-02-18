#include <cstdint>

#include <Unreal/UObject.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/Property/FNumericProperty.hpp>

namespace RC::Unreal
{
    uint32_t FNumericProperty::VTableOffsets::ImportText_Internal = 0;
    uint32_t FNumericProperty::VTableOffsets::IsInteger = 0;
    uint32_t FNumericProperty::VTableOffsets::ExportTextItem = 0;
    uint32_t FNumericProperty::VTableOffsets::CanHoldUnsignedValueInternal = 0;
    uint32_t FNumericProperty::VTableOffsets::GetFloatingPointPropertyValue = 0;
    uint32_t FNumericProperty::VTableOffsets::SetNumericPropertyValueFromString = 0;
    uint32_t FNumericProperty::VTableOffsets::IsFloatingPoint = 0;
    uint32_t FNumericProperty::VTableOffsets::GetIntPropertyEnum = 0;
    uint32_t FNumericProperty::VTableOffsets::SetIntPropertyValue = 0;
    uint32_t FNumericProperty::VTableOffsets::CanHoldDoubleValueInternal = 0;
    uint32_t FNumericProperty::VTableOffsets::SetFloatingPointPropertyValue = 0;
    uint32_t FNumericProperty::VTableOffsets::GetSignedIntPropertyValue = 0;
    uint32_t FNumericProperty::VTableOffsets::GetUnsignedIntPropertyValue = 0;
    uint32_t FNumericProperty::VTableOffsets::GetNumericPropertyValueToString = 0;
    uint32_t FNumericProperty::VTableOffsets::CanHoldSignedValueInternal = 0;
    uint32_t FNumericProperty::VTableOffsets::FNumericProperty_Destructor = 0;
    uint32_t FNumericProperty::VTableOffsets::__vecDelDtor = 0;
}
