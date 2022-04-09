#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual418.hpp>

// These are all the structs that have virtuals that need to have their offset set
#include <Unreal/UObject.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/FOutputDevice.hpp>
#include <Unreal/FProperty.hpp>
#include <Unreal/Property/FNumericProperty.hpp>
#include <Unreal/Property/FObjectProperty.hpp>

namespace RC::Unreal
{
    auto UnrealVirtual418::set_virtual_offsets() -> void
    {
#include <FunctionBodies/4_18_VTableOffsets_UObject_FunctionBody.cpp>
#include <FunctionBodies/4_18_VTableOffsets_UObjectBase_FunctionBody.cpp>
#include <FunctionBodies/4_18_VTableOffsets_UObjectBaseUtility_FunctionBody.cpp>
#include <FunctionBodies/4_18_VTableOffsets_UScriptStruct__ICppStructOps_FunctionBody.cpp>
#include <FunctionBodies/4_18_VTableOffsets_FOutputDevice_FunctionBody.cpp>
#include <FunctionBodies/4_18_VTableOffsets_FMalloc_FunctionBody.cpp>
#include <FunctionBodies/4_18_VTableOffsets_UStruct_FunctionBody.cpp>
#include <FunctionBodies/4_18_VTableOffsets_UField_FunctionBody.cpp>
#include <FunctionBodies/4_18_VTableOffsets_FProperty_FunctionBody.cpp>
#include <FunctionBodies/4_18_VTableOffsets_FNumericProperty_FunctionBody.cpp>
#include <FunctionBodies/4_18_VTableOffsets_FObjectPropertyBase_FunctionBody.cpp>
    }
}
