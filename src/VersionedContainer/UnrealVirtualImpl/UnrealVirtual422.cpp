#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual422.hpp>

// These are all the structs that have virtuals that need to have their offset set
#include <Unreal/UObject.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/FField.hpp>
#include <Unreal/FProperty.hpp>
#include <Unreal/Property/FNumericProperty.hpp>
#include <Unreal/Property/FObjectProperty.hpp>

namespace RC::Unreal
{
    auto UnrealVirtual422::set_virtual_offsets() -> void
    {
#include <FunctionBodies/4_22_VTableOffsets_UObject_FunctionBody.cpp>
#include <FunctionBodies/4_22_VTableOffsets_UObjectBase_FunctionBody.cpp>
#include <FunctionBodies/4_22_VTableOffsets_UObjectBaseUtility_FunctionBody.cpp>
#include <FunctionBodies/4_22_VTableOffsets_UScriptStruct__ICppStructOps_FunctionBody.cpp>
#include <FunctionBodies/4_22_VTableOffsets_FMalloc_FunctionBody.cpp>
#include <FunctionBodies/4_22_VTableOffsets_UField_FunctionBody.cpp>
#include <FunctionBodies/4_22_VTableOffsets_UProperty_FunctionBody.cpp>
#include <FunctionBodies/4_22_VTableOffsets_UNumericProperty_FunctionBody.cpp>
#include <FunctionBodies/4_22_VTableOffsets_UObjectPropertyBase_FunctionBody.cpp>
    }
}
