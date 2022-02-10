#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual426.hpp>

// These are all the structs that have virtuals that need to have their offset set
#include <Unreal/UObject.hpp>
#include <Unreal/UScriptStruct.hpp>

namespace RC::Unreal
{
    auto UnrealVirtual426::set_virtual_offsets() -> void
    {
#include <FunctionBodies/4_26_VTableOffsets_UObject_FunctionBody.cpp>
#include <FunctionBodies/4_26_VTableOffsets_UObjectBase_FunctionBody.cpp>
#include <FunctionBodies/4_26_VTableOffsets_UObjectBaseUtility_FunctionBody.cpp>
#include <FunctionBodies/4_26_VTableOffsets_UScriptStruct__ICppStructOps_FunctionBody.cpp>
#include <FunctionBodies/4_26_VTableOffsets_UField_FunctionBody.cpp>
#include <FunctionBodies/4_26_VTableOffsets_FField_FunctionBody.cpp>
#include <FunctionBodies/4_26_VTableOffsets_FProperty_FunctionBody.cpp>
    }
}
