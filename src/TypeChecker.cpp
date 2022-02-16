#include <algorithm>
#include <DynamicOutput/DynamicOutput.hpp>
#include <Unreal/TypeChecker.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/AActor.hpp>
#include <Unreal/FField.hpp>
#include <Unreal/UStruct.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UFunction.hpp>
#include <Unreal/FProperty.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/UEnum.hpp>
#include <Unreal/FAssetData.hpp>
#include <Unreal/UInterface.hpp>
#include <Unreal/UActorComponent.hpp>
#include <Unreal/UPackage.hpp>
#include <Unreal/Property/FNumericProperty.h>
#include <Unreal/Property/NumericPropertyTypes.h>
#include <Unreal/Property/FObjectProperty.hpp>
#include <Unreal/Property/FWeakObjectProperty.hpp>
#include <Unreal/Property/FLazyObjectProperty.hpp>
#include <Unreal/Property/FSoftObjectProperty.hpp>
#include <Unreal/Property/FClassProperty.hpp>
#include <Unreal/Property/FSoftClassProperty.hpp>
#include <Unreal/Property/FArrayProperty.hpp>
#include <Unreal/Property/FMapProperty.hpp>
#include <Unreal/Property/FBoolProperty.h>
#include <Unreal/Property/FNameProperty.hpp>
#include <Unreal/Property/FStructProperty.hpp>
#include <Unreal/Property/FEnumProperty.hpp>
#include <Unreal/Property/FTextProperty.hpp>
#include <Unreal/Property/FStrProperty.hpp>
#include <Unreal/Property/FDelegateProperty.hpp>
#include <Unreal/Property/FMulticastInlineDelegateProperty.hpp>
#include <Unreal/Property/FMulticastSparseDelegateProperty.hpp>
#include <Unreal/Property/FInterfaceProperty.hpp>
#include <Unreal/Property/FFieldPathProperty.hpp>
#include <Unreal/Property/FSetProperty.hpp>
#include <Unreal/UnrealVersion.hpp>

namespace RC::Unreal
{
    std::unordered_map<std::wstring, FName> TypeChecker::m_core_object_names{};
    std::unordered_map<std::wstring, void*> TypeChecker::m_core_object_pointers{};
    std::unordered_map<void*, TypeChecker::ObjectToStringDecl> TypeChecker::m_object_to_string_functions{};
    std::unordered_map<void*, TypeChecker::ObjectToStringComplexDecl> TypeChecker::m_object_to_string_complex_functions{};
    FName TypeChecker::m_core_world_name{};
    FName TypeChecker::m_core_level_name{};
    FName TypeChecker::m_core_package_name{};
    FName TypeChecker::m_core_enum_name{};
    FName TypeChecker::m_core_object_name{};
    FName TypeChecker::m_core_engine_name{};
    FName TypeChecker::m_property_name{};
    FName TypeChecker::m_boolproperty_name{};
    FName TypeChecker::m_int8property_name{};
    FName TypeChecker::m_int16property_name{};
    FName TypeChecker::m_intproperty_name{};
    FName TypeChecker::m_int64property_name{};
    FName TypeChecker::m_byteproperty_name{};
    FName TypeChecker::m_uint16property_name{};
    FName TypeChecker::m_uint32property_name{};
    FName TypeChecker::m_uint64property_name{};
    FName TypeChecker::m_floatproperty_name{};
    FName TypeChecker::m_doubleproperty_name{};
    FName TypeChecker::m_objectproperty_name{};
    FName TypeChecker::m_classproperty_name{};
    FName TypeChecker::m_strproperty_name{};
    FName TypeChecker::m_textproperty_name{};
    FName TypeChecker::m_structproperty_name{};
    FName TypeChecker::m_enumproperty_name{};
    FName TypeChecker::m_arrayproperty_name{};
    FName TypeChecker::m_nameproperty_name{};
    FName TypeChecker::m_scriptdelegateproperty_name{};
    FName TypeChecker::m_multicastinlinescriptdelegateproperty_name{};
    FName TypeChecker::m_multicastsparsescriptdelegateproperty_name{};
    FName TypeChecker::m_weakobjectproperty_name{};
    FName TypeChecker::m_mapproperty_name{};
    FName TypeChecker::m_function_name{};

    auto TypeChecker::get_world_name() -> FName
    {
        return m_core_world_name;
    }

    auto TypeChecker::get_level_name() -> FName
    {
        return m_core_level_name;
    }

    auto TypeChecker::store_all_object_names() -> void
    {
        //@formatter:off
        // Useful types
        m_core_object_names[L"Actor"]                                    = FName(L"Actor");
        m_core_object_names[L"Function"]                                 = FName(L"Function");
        m_core_object_names[L"Enum"]                                     = FName(L"Enum");

        // Classes
        m_core_object_names[L"Class"]                                    = FName(L"Class");
        m_core_object_names[L"BlueprintGeneratedClass"]                  = FName(L"BlueprintGeneratedClass");
        m_core_object_names[L"AnimBlueprintGeneratedClass"]              = FName(L"AnimBlueprintGeneratedClass");
        m_core_object_names[L"DynamicClass"]                             = FName(L"DynamicClass");
        m_core_object_names[L"LinkerPlaceholderClass"]                   = FName(L"LinkerPlaceholderClass");
        m_core_object_names[L"SoundClass"]                               = FName(L"SoundClass");
        m_core_object_names[L"SoundNodeSoundClass"]                      = FName(L"SoundNodeSoundClass");
        m_core_object_names[L"WidgetBlueprintGeneratedClass"]            = FName(L"WidgetBlueprintGeneratedClass");

        // Misc
        m_core_world_name                                                = FName(L"World");
        m_core_level_name                                                = FName(L"Level");
        m_core_package_name                                              = FName(L"Package");
        m_property_name                                                  = FName(L"Property");
        m_core_enum_name                                                 = FName(L"Enum");
        m_core_object_name                                               = FName(L"/Script/CoreUObject");
        m_core_engine_name                                               = FName(L"/Script/Engine");

        // Properties
        m_boolproperty_name                                              = FName(L"BoolProperty");
        m_int8property_name                                              = FName(L"Int8Property");
        m_int16property_name                                             = FName(L"Int16Property");
        m_intproperty_name                                               = FName(L"IntProperty");
        m_int64property_name                                             = FName(L"Int64Property");
        m_byteproperty_name                                              = FName(L"ByteProperty");
        m_uint16property_name                                            = FName(L"UInt16Property");
        m_uint32property_name                                            = FName(L"UInt32Property");
        m_uint64property_name                                            = FName(L"UInt64Property");
        m_floatproperty_name                                             = FName(L"FloatProperty");
        m_doubleproperty_name                                            = FName(L"DoubleProperty");
        m_objectproperty_name                                            = FName(L"ObjectProperty");
        m_classproperty_name                                             = FName(L"ClassProperty");
        m_strproperty_name                                               = FName(L"StrProperty");
        m_textproperty_name                                              = FName(L"TextProperty");
        m_structproperty_name                                            = FName(L"StructProperty");
        m_enumproperty_name                                              = FName(L"EnumProperty");
        m_arrayproperty_name                                             = FName(L"ArrayProperty");
        m_nameproperty_name                                              = FName(L"NameProperty");
        m_scriptdelegateproperty_name                                    = FName(L"DelegateProperty");
        m_multicastinlinescriptdelegateproperty_name                     = FName(L"MulticastInlineDelegateProperty");
        m_multicastsparsescriptdelegateproperty_name                     = FName(L"MulticastSparseDelegateProperty");
        m_weakobjectproperty_name                                        = FName(L"WeakObjectProperty");
        m_mapproperty_name                                               = FName(L"MapProperty");

        m_function_name                                                  = FName(L"Function");
        //@formatter:on
    }

    auto TypeChecker::store_all_object_types() -> bool
    {
        UObject::m_static_class = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Object");
        UStruct::m_static_class = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Struct");
        AActor::m_static_class = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/Engine.Actor");

        UClass* enum_ptr = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Enum");
        UEnum::m_static_class = enum_ptr;
        //m_object_to_string_functions[enum_ptr] = &UEnum::to_string;

        UClass* user_defined_enum_ptr = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/Engine.UserDefinedEnum");
        UUserDefinedEnum::m_static_class = user_defined_enum_ptr;
        //m_object_to_string_functions[user_defined_enum_ptr] = &UEnum::to_string;

        UClass* class_ptr = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Class");
        UClass::m_static_class = class_ptr;
        //m_object_to_string_functions[class_ptr] = &UClass::to_string;

        UClass* bp_generated_class_ptr = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/Engine.BlueprintGeneratedClass");
        UBlueprintGeneratedClass::m_static_class = bp_generated_class_ptr;
        //m_object_to_string_functions[bp_generated_class_ptr] = &UClass::to_string;

        UClass* anim_bp_generated_class_ptr = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/Engine.AnimBlueprintGeneratedClass");
        UAnimBlueprintGeneratedClass::m_static_class = anim_bp_generated_class_ptr;
        //m_object_to_string_functions[anim_bp_generated_class_ptr] = &UClass::to_string;

        UClass* widget_bp_generated_class_ptr = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/Engine.WidgetBlueprintGeneratedClass");
        UAnimBlueprintGeneratedClass::m_static_class = widget_bp_generated_class_ptr;
        //m_object_to_string_functions[widget_bp_generated_class_ptr] = &UClass::to_string;

        // Not available in 4.12 (I've not checked exactly when it starts being available)
        UClass* asset_data_ptr = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, STR("/Script/AssetRegistry.AssetData"));
        if (!asset_data_ptr)
        {
            // In 4.26, they moved it from the 'AssetRegistry' package to the 'CoreUObject' package
            asset_data_ptr = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, STR("/Script/CoreUObject.AssetData"));
        }
        FAssetData::m_static_class = asset_data_ptr;

        UPackage::m_static_class = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Package");
        UInterface::m_static_class = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Interface");
        UActorComponent::m_static_class = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/Engine.ActorComponent");
        USceneComponent::m_static_class = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/Engine.SceneComponent");

        auto register_function_derivative = [](const wchar_t* function_object_string) -> UClass* {
            UClass* ptr = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, function_object_string);
            if (ptr)
            {
                // The following objects are derived from UFunction and therefor can safely use the UClass version of to_string & the UFunction version of to_string_complex
                //m_object_to_string_functions[ptr] = &UFunction::to_string;
            }
            else
            {
                throw std::runtime_error{fmt("[TypeChecker::store_all_object_types] Was unable to find %S\n", function_object_string)};
            }
            return ptr;
        };

        UFunction::m_static_class = register_function_derivative(L"/Script/CoreUObject.Function");
        register_function_derivative(L"/Script/CoreUObject.DelegateFunction");

        if (Version::is_atleast(4, 23))
        {
            register_function_derivative(L"/Script/CoreUObject.SparseDelegateFunction");
        }

        UClass* script_struct_ptr = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.ScriptStruct");
        UScriptStruct::m_static_class = script_struct_ptr;
        //m_object_to_string_functions[script_struct_ptr] = &UScriptStruct::to_string;
        //m_object_to_string_complex_functions[script_struct_ptr] = &UScriptStruct::to_string_complex;

        /*
        FProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Property");
        FNumericProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.NumericProperty");
        FInt8Property::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Int8Property");
        FInt16Property::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Int16Property");
        FIntProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.IntProperty");
        FInt64Property::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Int64Property");
        FFloatProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.FloatProperty");
        FDoubleProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.DoubleProperty");
        FByteProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.ByteProperty");
        FUInt16Property::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.UInt16Property");
        FUInt32Property::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.UInt32Property");
        FUInt64Property::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.UInt64Property");
        FObjectProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.ObjectProperty");
        FClassProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.ClassProperty");
        FSoftClassProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.SoftClassProperty");
        FEnumProperty<uint8_t>::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.EnumProperty");
        FArrayProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.ArrayProperty");
        FStructProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.StructProperty");
        FNameProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.NameProperty");
        XBoolProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.BoolProperty");
        FMapProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.MapProperty");
        FWeakObjectProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.WeakObjectProperty");
        FLazyObjectProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.LazyObjectProperty");
        FSoftObjectProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.SoftObjectProperty");
        FTextProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.TextProperty");
        FStrProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.StrProperty");
        FInterfaceProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.InterfaceProperty");
        FFieldPathProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.FieldPathProperty");
        //*/

        // FField / FProperty Types
        if (Version::is_atleast(4, 25))
        {
            auto find_all_property_types = [](const std::wstring& obj_string) -> void {
                UClass* actor_obj = static_cast<UClass*>(UObjectGlobals::static_find_object(nullptr, nullptr, obj_string));
                if (!actor_obj) { return; }

                actor_obj->for_each_property([&](FProperty* child) {
                    // TODO: This is hard-coded for now... perhaps come up with a solution that isn't hardcoded
                    // This could break in the future if FFieldClass* is no longer at offset 0x8 in the FField struct
                    FFieldClass* ffield_class = Helper::Casting::offset_deref<FFieldClass*>(child, ffield_class_offset);
                    if (!ffield_class) { return LoopAction::Continue; }

                    FName type_name = Helper::Casting::offset_deref<FName>(ffield_class, 0);

                    // TODO: Look at the dumped objects and put every single FField type in here
                    // At the moment there are probably some missing types

                    // Fully supported
                    if (type_name == FName(L"ObjectProperty"))
                    {
                        m_core_object_pointers[L"ObjectProperty"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &FObjectProperty::to_string;
                        FObjectProperty::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"Int8Property"))
                    {
                        m_core_object_pointers[L"Int8Property"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &XInt8Property::to_string;
                        FInt8Property::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"Int16Property"))
                    {
                        m_core_object_pointers[L"Int16Property"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &XInt16Property::to_string;
                        FInt16Property::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"IntProperty"))
                    {
                        m_core_object_pointers[L"IntProperty"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &XIntProperty::to_string;
                        FIntProperty::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"Int64Property"))
                    {
                        m_core_object_pointers[L"Int64Property"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &XInt64Property::to_string;
                        FInt64Property::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"NameProperty"))
                    {
                        m_core_object_pointers[L"NameProperty"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &FNameProperty::to_string;
                        FNameProperty::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"FloatProperty"))
                    {
                        m_core_object_pointers[L"FloatProperty"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &XFloatProperty::to_string;
                        FFloatProperty::m_static_class = static_cast<FFieldClass*>(ffield_class);

                        // 'NumericProperty' doesn't appear as the direct type for any properties
                        // Because of this, we need to use a property that we know has 'NumericProperty' in its super chain
                        // We're using 'FFloatProperty' here for no particular reason other than that it has 'NumericProperty' as it's super
                        FNumericProperty::m_static_class = ffield_class->SuperClass;
                    }
                    if (type_name == FName(L"DoubleProperty"))
                    {
                        m_core_object_pointers[L"DoubleProperty"] = ffield_class;
                        FDoubleProperty::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"ByteProperty"))
                    {
                        m_core_object_pointers[L"ByteProperty"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &XByteProperty::to_string;
                        FByteProperty::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"UInt16Property"))
                    {
                        m_core_object_pointers[L"UInt16Property"] = ffield_class;
                        FUInt16Property::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"UInt32Property"))
                    {
                        m_core_object_pointers[L"UInt32Property"] = ffield_class;
                        FUInt32Property::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"UInt64Property"))
                    {
                        m_core_object_pointers[L"UInt64Property"] = ffield_class;
                        FUInt64Property::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"BoolProperty"))
                    {
                        m_core_object_pointers[L"BoolProperty"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &XBoolProperty::to_string;
                        FBoolProperty::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"ArrayProperty"))
                    {
                        m_core_object_pointers[L"ArrayProperty"] = ffield_class;
                        // TODO: Deal with the fact that we don't know the templated type here
                        // Using XIntProperty for testing but it's possible we can use it this way because
                        // to_string is going to need to dynamically (at runtime) figure out what the derived type is regardless
                        // The templated type would be effectively be useless here but is required since the whole class is templated
                        //m_object_to_string_functions[ffield_class] = &FArrayProperty::to_string;
                        //m_object_to_string_complex_functions[ffield_class] = &FArrayProperty::to_string_complex;
                        FArrayProperty::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"MapProperty"))
                    {
                        m_core_object_pointers[L"MapProperty"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &FMapProperty::to_string;
                        //m_object_to_string_complex_functions[ffield_class] = &FMapProperty::to_string_complex;
                        FMapProperty::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"StructProperty"))
                    {
                        m_core_object_pointers[L"StructProperty"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &FStructProperty::to_string;
                        FStructProperty::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"ClassProperty"))
                    {
                        m_core_object_pointers[L"ClassProperty"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &FClassProperty::to_string;
                        FClassProperty::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"SoftClassProperty"))
                    {
                        m_core_object_pointers[L"SoftClassProperty"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &FClassProperty::to_string;
                        FSoftClassProperty::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"WeakObjectProperty"))
                    {
                        m_core_object_pointers[L"WeakObjectProperty"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &FWeakObjectProperty::to_string;
                        FWeakObjectProperty::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"LazyObjectProperty"))
                    {
                        m_core_object_pointers[L"LazyObjectProperty"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &FObjectProperty::to_string;
                        FLazyObjectProperty::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"SoftObjectProperty"))
                    {
                        m_core_object_pointers[L"SoftObjectProperty"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &FObjectProperty::to_string;
                        FSoftObjectProperty::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"EnumProperty"))
                    {
                        m_core_object_pointers[L"EnumProperty"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &FEnumProperty<uint8_t>::to_string;
                        FEnumProperty::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"TextProperty"))
                    {
                        m_core_object_pointers[L"TextProperty"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &FTextProperty::to_string;
                        FTextProperty::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"StrProperty"))
                    {
                        m_core_object_pointers[L"StrProperty"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &FStrProperty::to_string;
                        FStrProperty::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"DelegateProperty"))
                    {
                        m_core_object_pointers[L"DelegateProperty"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &FDelegateProperty::to_string;
                        FDelegateProperty::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"MulticastInlineDelegateProperty"))
                    {
                        m_core_object_pointers[L"MulticastInlineDelegateProperty"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &FMulticastInlineDelegateProperty::to_string;
                        FMulticastInlineDelegateProperty::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"MulticastSparseDelegateProperty"))
                    {
                        m_core_object_pointers[L"MulticastSparseDelegateProperty"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &FMulticastSparseDelegateProperty::to_string;
                        FMulticastSparseDelegateProperty::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"InterfaceProperty"))
                    {
                        m_core_object_pointers[L"InterfaceProperty"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &FInterfaceProperty::to_string;
                        FInterfaceProperty::m_static_class = ffield_class;
                    }
                    if (type_name == FName(L"FieldPathProperty"))
                    {
                        m_core_object_pointers[L"FieldPathProperty"] = ffield_class;
                        //m_object_to_string_functions[ffield_class] = &FFieldPathProperty::to_string;
                        FFieldPathProperty::m_static_class = ffield_class;
                    }

                    // Not yet supported, only here for completion and to prevent crashes
                    // Not all of these will be found right now, need to call this function with more UObjects that actually have these properties
                    if (type_name == FName(L"SetProperty")) { m_core_object_pointers[L"SetProperty"] = ffield_class; }

                    // Find the root 'Property' FField. This may not be necessary but I'm leaving it here for now, might be useful later.
                    if (!m_core_object_pointers.contains(L"Property"))
                    {
                        // TODO: This is hard-coded for now... perhaps come up with a solution that isn't hardcoded
                        // This could break in the future if FFieldClass* is no longer at offset 0x8 in the FField struct
                        void* ffield_super_class = Helper::Casting::offset_deref<void*>(ffield_class, super_class_offset);
                        if (ffield_super_class)
                        {
                            do
                            {
                                FName super_type_name = Helper::Casting::offset_deref<FName>(ffield_super_class, 0);
                                if (super_type_name == FName(L"Property"))
                                {
                                    m_core_object_pointers[L"Property"] = ffield_super_class;
                                    FProperty::m_static_class = ffield_class;
                                    break;
                                }

                                ffield_super_class = Helper::Casting::offset_deref<void*>(ffield_super_class, super_class_offset);
                            } while (ffield_super_class);
                        }
                    }

                    return LoopAction::Continue;
                });
            };

            // TODO: Maybe replace all of the calls to 'find_all_property_types' with a GUObjectArray iteration with an early exit once everything's been found ?

            // Bool, Enum, Byte, Struct, Float, Object, Name, Int, Array
            // WeakObject, MulticastSparseDelegate
            find_all_property_types(L"/Script/Engine.Actor");
            // Class
            find_all_property_types(L"/Script/Engine.Pawn");
            // Str, Map
            find_all_property_types(L"/Script/Engine.NetDriver");
            // Double
            find_all_property_types(L"/Script/Engine.NetConnection");
            // Text
            find_all_property_types(L"/Script/Engine.GameModeBase");
            // Int8, LazyObject
            find_all_property_types(L"/Script/Landscape.LandscapeComponent");
            // UInt16, UInt32, UInt64, Int8, Int16, Int64
            find_all_property_types(L"/Script/Engine.IntSerialization");
            // Set, MulticastInlineDelegate, Delegate
            find_all_property_types(L"/Script/Engine.AudioComponent");
            // MulticastInlineDelegate, Interface
            find_all_property_types(L"/Script/MovieScene.MovieSceneSequencePlayer");
            // SoftObject
            find_all_property_types(L"/Script/Engine.LevelStreaming");
            // SoftClass
            find_all_property_types(L"/Script/Engine.PrimaryAssetTypeInfo");
            // FieldPath
            find_all_property_types(L"/Script/Engine.InterpTrackInstProperty");

            for (auto&[object_string, object_ptr] : m_core_object_pointers)
            {
                if (!object_ptr) { return false; }
            }
        }
        else
        {
            // 4.24 and earlier, just use StaticFindObject to find all of the property type pointers

            struct ToStringFuncs
            {
                ObjectToStringDecl normal;
                ObjectToStringComplexDecl complex;
            };

            auto add_property = []<typename PropertyType>(const wchar_t* full_property_name, const wchar_t* name, /*ToStringFuncs to_string_funcs,*/ [[maybe_unused]]PropertyType property_type_object) {
                UObject* property = UObjectGlobals::static_find_object(nullptr, nullptr, full_property_name);
                if (!property)
                {
                    // TODO: Put this error back when you've confirmed when every property type was introduced and you're only trying to find it in those versions and later
                    /*
                    std::wstring error = std::format(L"Was unable to find pointer to '{}'", full_property_name);
                    std::string error_ansi = std::string(error.begin(), error.end());
                    throw std::runtime_error{error_ansi};
                    //*/
                    return;
                }

                m_core_object_pointers[name] = property;

                if constexpr (!std::is_same_v<PropertyType, int32_t>)
                {
                    PropertyType::m_static_class = static_cast<UClass*>(property);
                }

                /*
                if (to_string_funcs.normal)
                {
                    m_object_to_string_functions[property] = to_string_funcs.normal;
                }

                if (to_string_funcs.complex)
                {
                    m_object_to_string_complex_functions[property] = to_string_funcs.complex;
                }
                //*/
            };

            add_property(L"/Script/CoreUObject.ObjectProperty", L"ObjectProperty", /*{&FObjectProperty::to_string},*/ FObjectProperty{});
            add_property(L"/Script/CoreUObject.NumericProperty", L"NumericProperty", /*{&FNumericProperty::to_string},*/ FNumericProperty{});
            add_property(L"/Script/CoreUObject.Int8Property", L"Int8Property", /*{&FInt8Property::to_string},*/ FInt8Property{});
            add_property(L"/Script/CoreUObject.Int16Property", L"Int16Property", /*{&FInt16Property::to_string},*/ FInt16Property{});
            add_property(L"/Script/CoreUObject.IntProperty", L"IntProperty", /*{&FIntProperty::to_string},*/ FIntProperty{});
            add_property(L"/Script/CoreUObject.Int64Property", L"Int64Property", /*{&FInt64Property::to_string},*/ FInt64Property{});
            add_property(L"/Script/CoreUObject.NameProperty", L"NameProperty", /*{&FNameProperty::to_string},*/ FNameProperty{});
            add_property(L"/Script/CoreUObject.FloatProperty", L"FloatProperty", /*{&FFloatProperty::to_string},*/ FFloatProperty{});
            add_property(L"/Script/CoreUObject.DoubleProperty", L"DoubleProperty", /*{&FDoubleProperty::to_string},*/ FDoubleProperty{});
            add_property(L"/Script/CoreUObject.ByteProperty", L"ByteProperty", /*{&FByteProperty::to_string},*/ FByteProperty{});
            add_property(L"/Script/CoreUObject.UInt16Property", L"UInt16Property", /*{&FUInt16Property::to_string},*/ FUInt16Property{});
            add_property(L"/Script/CoreUObject.UInt32Property", L"UInt32Property", /*{&FUInt32Property::to_string},*/ FUInt32Property{});
            add_property(L"/Script/CoreUObject.UInt64Property", L"UInt64Property", /*{&FUInt64Property::to_string},*/ FUInt64Property{});
            add_property(L"/Script/CoreUObject.BoolProperty", L"BoolProperty", /*{&FBoolProperty::to_string},*/ FBoolProperty{});
            add_property(L"/Script/CoreUObject.ArrayProperty", L"ArrayProperty", /*{&FArrayProperty::to_string, &FArrayProperty::to_string_complex},*/ FArrayProperty{});
            add_property(L"/Script/CoreUObject.MapProperty", L"MapProperty", /*{&FMapProperty::to_string, &FMapProperty::to_string_complex},*/ FMapProperty{});
            add_property(L"/Script/CoreUObject.StructProperty", L"StructProperty", /*{&FStructProperty::to_string},*/ FStructProperty{});
            add_property(L"/Script/CoreUObject.ClassProperty", L"ClassProperty", /*{&FClassProperty::to_string},*/ FClassProperty{});
            add_property(L"/Script/CoreUObject.SoftClassProperty", L"SoftClassProperty", /*{&FClassProperty::to_string},*/ FSoftClassProperty{});
            add_property(L"/Script/CoreUObject.WeakObjectProperty", L"WeakObjectProperty", /*{&FWeakObjectProperty::to_string},*/ FWeakObjectProperty{});
            add_property(L"/Script/CoreUObject.LazyObjectProperty", L"LazyObjectProperty", /*{&FObjectProperty::to_string},*/ FLazyObjectProperty{});
            add_property(L"/Script/CoreUObject.SoftObjectProperty", L"SoftObjectProperty", /*{&FObjectProperty::to_string},*/ FSoftObjectProperty{});
            if (Version::is_atleast(4, 15)) { add_property(L"/Script/CoreUObject.EnumProperty", L"EnumProperty", /*{&FEnumProperty::to_string},*/ FEnumProperty{}); }
            add_property(L"/Script/CoreUObject.TextProperty", L"TextProperty", /*{&FTextProperty::to_string},*/ FTextProperty{});
            add_property(L"/Script/CoreUObject.StrProperty", L"StrProperty", /*{&FStrProperty::to_string},*/ FStrProperty{});
            add_property(L"/Script/CoreUObject.DelegateProperty", L"DelegateProperty", /*{&FDelegateProperty::to_string},*/ FDelegateProperty{});
            add_property(L"/Script/CoreUObject.MulticastDelegateProperty", L"MulticastDelegateProperty", /*{&FMulticastInlineDelegateProperty::to_string},*/ FMulticastInlineDelegateProperty{});
            add_property(L"/Script/CoreUObject.MulticastInlineDelegateProperty", L"MulticastInlineDelegateProperty", /*{&FMulticastInlineDelegateProperty::to_string},*/ FMulticastInlineDelegateProperty{});
            add_property(L"/Script/CoreUObject.MulticastSparseDelegateProperty", L"MulticastSparseDelegateProperty", /*{&FMulticastSparseDelegateProperty::to_string},*/ FMulticastSparseDelegateProperty{});
            add_property(L"/Script/CoreUObject.InterfaceProperty", L"InterfaceProperty", /*{&FInterfaceProperty::to_string},*/ FInterfaceProperty{});

            // Not yet supported, only here for completion and to prevent crashes
            // Not all of these will be found right now, need to call this function with more UObjects that actually have these properties
            add_property(L"/Script/CoreUObject.SetProperty", L"SetProperty", /*{},*/ FSetProperty{});

            // Find the root 'Property' FField. This may not be necessary but I'm leaving it here for now, might be useful later.
            add_property(L"/Script/CoreUObject.Property", L"Property", /*{},*/ FProperty{});
        }

        // Verifying that all the core objects were successfully retrieved
        // Note: This verification will fail if the object legitimately has an FName comparison index of 0
        // But that's never going to be the case for the objects that we're interested in here
        for (auto&[object_string, object_fname] : m_core_object_names)
        {
            if (object_fname.get_comparison_index() == 0)
            {
                Output::send(STR("FName '{}' is zero."), object_string);
                return false;
            }
        }

        return true;
    }

    auto TypeChecker::get_fname(const std::wstring& object_name) -> FName
    {
        if (m_core_object_names.contains(object_name))
        {
            return m_core_object_names[object_name];
        }
        else
        {
            return {};
        }
    }

    auto TypeChecker::is_ffield(const void* p_this) -> bool
    {
        // This function relies on all of the FField types actually being present in m_core_object_pointers
        void* ffield_class = Helper::Casting::offset_deref<void*>(p_this, ffield_class_offset);
        return std::ranges::any_of(m_core_object_pointers, [&](const auto& pair) { return ffield_class == pair.second; });
    }

    auto TypeChecker::is_property(FField* field) -> bool
    {
        UObject* object = std::bit_cast<UObject*>(field);
        return is_property(object);
    }

    auto TypeChecker::is_property(UObject* object) -> bool
    {
        UClass* obj_class = object->get_uclass();
        if (!obj_class) { return false; }

        if (obj_class->get_fname() == m_property_name) { return true; }

        UStruct* super_struct = obj_class->get_super_struct();
        while (super_struct)
        {
            if (super_struct->get_fname() == m_property_name)
            {
                return true;
            }

            super_struct = super_struct->get_super_struct();
        }

        return false;
    }
}