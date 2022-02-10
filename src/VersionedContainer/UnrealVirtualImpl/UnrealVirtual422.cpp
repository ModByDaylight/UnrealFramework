#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual422.hpp>

// These are all the structs that have virtuals that need to have their offset set
#include <Unreal/UObject.hpp>
#include <Unreal/UScriptStruct.hpp>

namespace RC::Unreal
{
    auto UnrealVirtual422::set_virtual_offsets() -> void
    {
#include <FunctionBodies/4_22_VTableOffsets_UObject_FunctionBody.cpp>
#include <FunctionBodies/4_22_VTableOffsets_UObjectBase_FunctionBody.cpp>
#include <FunctionBodies/4_22_VTableOffsets_UObjectBaseUtility_FunctionBody.cpp>
#include <FunctionBodies/4_22_VTableOffsets_UScriptStruct__ICppStructOps_FunctionBody.cpp>
    }
}
