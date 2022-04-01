#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual500.hpp>

// These are all the structs that have virtuals that need to have their offset set
#include <Unreal/UObject.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/FOutputDevice.hpp>
#include <Unreal/FField.hpp>
#include <Unreal/FProperty.hpp>
#include <Unreal/Property/FNumericProperty.hpp>
#include <Unreal/Property/FObjectProperty.hpp>

namespace RC::Unreal
{
    auto UnrealVirtual500::set_virtual_offsets() -> void
    {
#include <FunctionBodies/5_00_VTableOffsets_UObject_FunctionBody.cpp>
#include <FunctionBodies/5_00_VTableOffsets_UObjectBase_FunctionBody.cpp>
#include <FunctionBodies/5_00_VTableOffsets_UObjectBaseUtility_FunctionBody.cpp>
#include <FunctionBodies/5_00_VTableOffsets_UScriptStruct__ICppStructOps_FunctionBody.cpp>
#include <FunctionBodies/5_00_VTableOffsets_FOutputDevice_FunctionBody.cpp>
#include <FunctionBodies/5_00_VTableOffsets_FMalloc_FunctionBody.cpp>
#include <FunctionBodies/5_00_VTableOffsets_UStruct_FunctionBody.cpp>
#include <FunctionBodies/5_00_VTableOffsets_UField_FunctionBody.cpp>
#include <FunctionBodies/5_00_VTableOffsets_FField_FunctionBody.cpp>
#include <FunctionBodies/5_00_VTableOffsets_FProperty_FunctionBody.cpp>
#include <FunctionBodies/5_00_VTableOffsets_FNumericProperty_FunctionBody.cpp>
#include <FunctionBodies/5_00_VTableOffsets_FObjectPropertyBase_FunctionBody.cpp>
    }
}
