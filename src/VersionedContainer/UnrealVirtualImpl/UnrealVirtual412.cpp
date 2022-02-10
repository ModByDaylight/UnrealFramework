#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual412.hpp>

// These are all the structs that have virtuals that need to have their offset set
#include <Unreal/UObject.hpp>
#include <Unreal/UScriptStruct.hpp>

namespace RC::Unreal
{
    auto UnrealVirtual412::set_virtual_offsets() -> void
    {
#include <FunctionBodies/4_12_VTableOffsets_UObject_FunctionBody.cpp>
#include <FunctionBodies/4_12_VTableOffsets_UObjectBase_FunctionBody.cpp>
#include <FunctionBodies/4_12_VTableOffsets_UObjectBaseUtility_FunctionBody.cpp>
#include <FunctionBodies/4_12_VTableOffsets_UScriptStruct__ICppStructOps_FunctionBody.cpp>
    }
}
