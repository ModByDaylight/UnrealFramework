#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual412.hpp>

// These are all the structs that have virtuals that need to have their offset set
#include <Unreal/UObject.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/FOutputDevice.hpp>
#include <Unreal/FProperty.hpp>
#include <Unreal/Property/FNumericProperty.hpp>
#include <Unreal/Property/FObjectProperty.hpp>
#include <Unreal/Property/FMulticastDelegateProperty.hpp>
#include <Unreal/Property/FStructProperty.hpp>
#include <Unreal/Property/FArrayProperty.hpp>
#include <Unreal/Property/FMapProperty.hpp>
#include <Unreal/World.hpp>

namespace RC::Unreal
{
    auto UnrealVirtual412::set_virtual_offsets() -> void
    {
#include <FunctionBodies/4_12_VTableOffsets_UObject_FunctionBody.cpp>
#include <FunctionBodies/4_12_VTableOffsets_UObjectBase_FunctionBody.cpp>
#include <FunctionBodies/4_12_VTableOffsets_UObjectBaseUtility_FunctionBody.cpp>
#include <FunctionBodies/4_12_VTableOffsets_UScriptStruct__ICppStructOps_FunctionBody.cpp>
#include <FunctionBodies/4_12_VTableOffsets_FOutputDevice_FunctionBody.cpp>
#include <FunctionBodies/4_12_VTableOffsets_FMalloc_FunctionBody.cpp>
#include <FunctionBodies/4_13_VTableOffsets_UStruct_FunctionBody.cpp>
#include <FunctionBodies/4_12_VTableOffsets_UField_FunctionBody.cpp>
#include <FunctionBodies/4_12_VTableOffsets_FProperty_FunctionBody.cpp>
#include <FunctionBodies/4_12_VTableOffsets_FNumericProperty_FunctionBody.cpp>
#include <FunctionBodies/4_12_VTableOffsets_FMulticastDelegateProperty_FunctionBody.cpp>
#include <FunctionBodies/4_12_VTableOffsets_FObjectPropertyBase_FunctionBody.cpp>

#include <FunctionBodies/4_12_MemberVariableLayout_DefaultSetter_UObjectBase.cpp>
#include <FunctionBodies/4_12_MemberVariableLayout_DefaultSetter_UScriptStruct__ICppStructOps.cpp>
#include <FunctionBodies/4_12_MemberVariableLayout_DefaultSetter_FOutputDevice.cpp>
#include <FunctionBodies/4_13_MemberVariableLayout_DefaultSetter_UStruct.cpp>
#include <FunctionBodies/4_12_MemberVariableLayout_DefaultSetter_UField.cpp>
#include <FunctionBodies/4_12_MemberVariableLayout_DefaultSetter_FProperty.cpp>
#include <FunctionBodies/4_12_MemberVariableLayout_DefaultSetter_FMulticastDelegateProperty.cpp>
#include <FunctionBodies/4_12_MemberVariableLayout_DefaultSetter_FObjectPropertyBase.cpp>
#include <FunctionBodies/4_12_MemberVariableLayout_DefaultSetter_FStructProperty.cpp>
#include <FunctionBodies/4_12_MemberVariableLayout_DefaultSetter_FArrayProperty.cpp>
#include <FunctionBodies/4_12_MemberVariableLayout_DefaultSetter_FMapProperty.cpp>
#include <FunctionBodies/4_12_MemberVariableLayout_DefaultSetter_UWorld.cpp>
    }
}
