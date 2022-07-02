#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual417.hpp>

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
    auto UnrealVirtual417::set_virtual_offsets() -> void
    {
#include <FunctionBodies/4_17_VTableOffsets_UObject_FunctionBody.cpp>
#include <FunctionBodies/4_17_VTableOffsets_UObjectBase_FunctionBody.cpp>
#include <FunctionBodies/4_17_VTableOffsets_UObjectBaseUtility_FunctionBody.cpp>
#include <FunctionBodies/4_17_VTableOffsets_UScriptStruct__ICppStructOps_FunctionBody.cpp>
#include <FunctionBodies/4_17_VTableOffsets_FOutputDevice_FunctionBody.cpp>
#include <FunctionBodies/4_17_VTableOffsets_FMalloc_FunctionBody.cpp>
#include <FunctionBodies/4_17_VTableOffsets_UStruct_FunctionBody.cpp>
#include <FunctionBodies/4_17_VTableOffsets_UField_FunctionBody.cpp>
#include <FunctionBodies/4_17_VTableOffsets_FProperty_FunctionBody.cpp>
#include <FunctionBodies/4_17_VTableOffsets_FNumericProperty_FunctionBody.cpp>
#include <FunctionBodies/4_17_VTableOffsets_FMulticastDelegateProperty_FunctionBody.cpp>
#include <FunctionBodies/4_17_VTableOffsets_FObjectPropertyBase_FunctionBody.cpp>

#include <FunctionBodies/4_17_MemberVariableLayout_DefaultSetter_UObjectBase.cpp>
#include <FunctionBodies/4_17_MemberVariableLayout_DefaultSetter_UScriptStruct__ICppStructOps.cpp>
#include <FunctionBodies/4_17_MemberVariableLayout_DefaultSetter_FOutputDevice.cpp>
#include <FunctionBodies/4_17_MemberVariableLayout_DefaultSetter_UStruct.cpp>
#include <FunctionBodies/4_17_MemberVariableLayout_DefaultSetter_UField.cpp>
#include <FunctionBodies/4_17_MemberVariableLayout_DefaultSetter_FProperty.cpp>
#include <FunctionBodies/4_17_MemberVariableLayout_DefaultSetter_FMulticastDelegateProperty.cpp>
#include <FunctionBodies/4_17_MemberVariableLayout_DefaultSetter_FObjectPropertyBase.cpp>
#include <FunctionBodies/4_17_MemberVariableLayout_DefaultSetter_FStructProperty.cpp>
#include <FunctionBodies/4_17_MemberVariableLayout_DefaultSetter_FArrayProperty.cpp>
#include <FunctionBodies/4_17_MemberVariableLayout_DefaultSetter_FMapProperty.cpp>
#include <FunctionBodies/4_17_MemberVariableLayout_DefaultSetter_UWorld.cpp>
    }
}