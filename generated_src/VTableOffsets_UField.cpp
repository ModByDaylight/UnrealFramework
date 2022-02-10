#include <cstdint>

#include <Unreal/UObject.hpp>
#include <Unreal/UScriptStruct.hpp>

namespace RC::Unreal
{
    uint32_t UField::VTableOffsets::PostLoad = 0;
    uint32_t UField::VTableOffsets::Serialize = 0;
    uint32_t UField::VTableOffsets::NeedsLoadForClient = 0;
    uint32_t UField::VTableOffsets::AddCppProperty = 0;
    uint32_t UField::VTableOffsets::NeedsLoadForServer = 0;
    uint32_t UField::VTableOffsets::Bind = 0;
    uint32_t UField::VTableOffsets::UField_Destructor = 0;
    uint32_t UField::VTableOffsets::__vecDelDtor = 0;
}
