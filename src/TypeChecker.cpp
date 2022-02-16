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
#include <Unreal/Property/XNumericProperty.hpp>
#include <Unreal/Property/XInt8Property.hpp>
#include <Unreal/Property/XInt16Property.hpp>
#include <Unreal/Property/XIntProperty.hpp>
#include <Unreal/Property/XInt64Property.hpp>
#include <Unreal/Property/XByteProperty.hpp>
#include <Unreal/Property/XUInt16Property.hpp>
#include <Unreal/Property/XUInt32Property.hpp>
#include <Unreal/Property/XUInt64Property.hpp>
#include <Unreal/Property/XFloatProperty.hpp>
#include <Unreal/Property/XDoubleProperty.hpp>
#include <Unreal/Property/FObjectProperty.hpp>
#include <Unreal/Property/FWeakObjectProperty.hpp>
#include <Unreal/Property/FLazyObjectProperty.hpp>
#include <Unreal/Property/FSoftObjectProperty.hpp>
#include <Unreal/Property/FClassProperty.hpp>
#include <Unreal/Property/FSoftClassProperty.hpp>
#include <Unreal/Property/FArrayProperty.hpp>
#include <Unreal/Property/FMapProperty.hpp>
#include <Unreal/Property/XBoolProperty.hpp>
#include <Unreal/Property/XNameProperty.hpp>
#include <Unreal/Property/XStructProperty.hpp>
#include <Unreal/Property/FEnumProperty.hpp>
#include <Unreal/Property/XTextProperty.hpp>
#include <Unreal/Property/XStrProperty.hpp>
#include <Unreal/Property/FDelegateProperty.hpp>
#include <Unreal/Property/FMulticastInlineDelegateProperty.hpp>
#include <Unreal/Property/FMulticastSparseDelegateProperty.hpp>
#include <Unreal/Property/FInterfaceProperty.hpp>
#include <Unreal/Property/FFieldPathProperty.hpp>
#include <Unreal/UnrealVersion.hpp>

namespace RC::Unreal
{
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
        UObject::set_static_obj_ptr(UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Object"));
        UStruct::set_static_obj_ptr(UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Struct"));
        AActor::set_static_obj_ptr(UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/Engine.Actor"));

        UClass* enum_ptr = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Enum");
        UEnum::set_static_obj_ptr(enum_ptr);
        m_object_to_string_functions[enum_ptr] = &UEnum::to_string;

        UClass* user_defined_enum_ptr = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/Engine.UserDefinedEnum");
        UUserDefinedEnum::set_static_obj_ptr(user_defined_enum_ptr);
        m_object_to_string_functions[user_defined_enum_ptr] = &UEnum::to_string;

        UClass* class_ptr = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Class");
        UClass::set_static_obj_ptr(class_ptr);
        m_object_to_string_functions[class_ptr] = &UClass::to_string;

        UClass* bp_generated_class_ptr = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/Engine.BlueprintGeneratedClass");
        UBlueprintGeneratedClass::set_static_obj_ptr(bp_generated_class_ptr);
        m_object_to_string_functions[bp_generated_class_ptr] = &UClass::to_string;

        UClass* anim_bp_generated_class_ptr = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/Engine.AnimBlueprintGeneratedClass");
        UAnimBlueprintGeneratedClass::set_static_obj_ptr(anim_bp_generated_class_ptr);
        m_object_to_string_functions[anim_bp_generated_class_ptr] = &UClass::to_string;

        UClass* widget_bp_generated_class_ptr = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/Engine.WidgetBlueprintGeneratedClass");
        UAnimBlueprintGeneratedClass::set_static_obj_ptr(widget_bp_generated_class_ptr);
        m_object_to_string_functions[widget_bp_generated_class_ptr] = &UClass::to_string;

        // Not available in 4.12 (I've not checked exactly when it starts being available)
        UScriptStruct* asset_data_ptr = UObjectGlobals::static_find_object<UScriptStruct*>(nullptr, nullptr, STR("/Script/AssetRegistry.AssetData"));
        if (!asset_data_ptr)
        {
            // In 4.26, they moved it from the 'AssetRegistry' package to the 'CoreUObject' package
            asset_data_ptr = UObjectGlobals::static_find_object<UScriptStruct*>(nullptr, nullptr, STR("/Script/CoreUObject.AssetData"));
        }
        FAssetData::set_static_obj_ptr(asset_data_ptr);

        UPackage::set_static_obj_ptr(UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Package"));
        UInterface::set_static_obj_ptr(UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Interface"));
        UActorComponent::set_static_obj_ptr(UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/Engine.ActorComponent"));
        USceneComponent::set_static_obj_ptr(UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/Engine.SceneComponent"));

        auto register_function_derivative = [](const wchar_t* function_object_string) -> UClass* {
            UClass* ptr = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, function_object_string);
            if (ptr)
            {
                // The following objects are derived from UFunction and therefor can safely use the UClass version of to_string & the UFunction version of to_string_complex
                m_object_to_string_functions[ptr] = &UFunction::to_string;
            }
            else
            {
                throw std::runtime_error{fmt("[TypeChecker::store_all_object_types] Was unable to find %S\n", function_object_string)};
            }
            return ptr;
        };

        UFunction::set_static_obj_ptr(register_function_derivative(L"/Script/CoreUObject.Function"));
        register_function_derivative(L"/Script/CoreUObject.DelegateFunction");

        if (Version::is_atleast(4, 23))
        {
            register_function_derivative(L"/Script/CoreUObject.SparseDelegateFunction");
        }

        UClass* script_struct_ptr = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.ScriptStruct");
        UScriptStruct::set_static_obj_ptr(script_struct_ptr);
        m_object_to_string_functions[script_struct_ptr] = &UScriptStruct::to_string;
        m_object_to_string_complex_functions[script_struct_ptr] = &UScriptStruct::to_string_complex;

        FProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Property");
        XNumericProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.NumericProperty");
        XInt8Property::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Int8Property");
        XInt16Property::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Int16Property");
        XIntProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.IntProperty");
        XInt64Property::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Int64Property");
        XFloatProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.FloatProperty");
        XDoubleProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.DoubleProperty");
        XByteProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.ByteProperty");
        XUInt16Property::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.UInt16Property");
        XUInt32Property::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.UInt32Property");
        XUInt64Property::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.UInt64Property");
        FObjectProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.ObjectProperty");
        FClassProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.ClassProperty");
        FSoftClassProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.SoftClassProperty");
        FEnumProperty<uint8_t>::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.EnumProperty");
        FArrayProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.ArrayProperty");
        XStructProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.StructProperty");
        XNameProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.NameProperty");
        XBoolProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.BoolProperty");
        FMapProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.MapProperty");
        FWeakObjectProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.WeakObjectProperty");
        FLazyObjectProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.LazyObjectProperty");
        FSoftObjectProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.SoftObjectProperty");
        XTextProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.TextProperty");
        XStrProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.StrProperty");
        FInterfaceProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.InterfaceProperty");
        FFieldPathProperty::m_static_obj = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.FieldPathProperty");

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
                        m_object_to_string_functions[ffield_class] = &FObjectProperty::to_string;
                        FObjectProperty::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"Int8Property"))
                    {
                        m_core_object_pointers[L"Int8Property"] = ffield_class;
                        m_object_to_string_functions[ffield_class] = &XInt8Property::to_string;
                        XInt8Property::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"Int16Property"))
                    {
                        m_core_object_pointers[L"Int16Property"] = ffield_class;
                        m_object_to_string_functions[ffield_class] = &XInt16Property::to_string;
                        XInt16Property::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"IntProperty"))
                    {
                        m_core_object_pointers[L"IntProperty"] = ffield_class;
                        m_object_to_string_functions[ffield_class] = &XIntProperty::to_string;
                        XIntProperty::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"Int64Property"))
                    {
                        m_core_object_pointers[L"Int64Property"] = ffield_class;
                        m_object_to_string_functions[ffield_class] = &XInt64Property::to_string;
                        XInt64Property::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"NameProperty"))
                    {
                        m_core_object_pointers[L"NameProperty"] = ffield_class;
                        m_object_to_string_functions[ffield_class] = &XNameProperty::to_string;
                        XNameProperty::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"FloatProperty"))
                    {
                        m_core_object_pointers[L"FloatProperty"] = ffield_class;
                        m_object_to_string_functions[ffield_class] = &XFloatProperty::to_string;
                        XFloatProperty::m_static_obj_variant = static_cast<FFieldClass*>(ffield_class);

                        // 'NumericProperty' doesn't appear as the direct type for any properties
                        // Because of this, we need to use a property that we know has 'NumericProperty' in its super chain
                        XNumericProperty::m_static_obj_variant = static_cast<FFieldClass*>(ffield_class)->super_class;
                    }
                    if (type_name == FName(L"DoubleProperty"))
                    {
                        m_core_object_pointers[L"DoubleProperty"] = ffield_class;
                        XDoubleProperty::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"ByteProperty"))
                    {
                        m_core_object_pointers[L"ByteProperty"] = ffield_class;
                        m_object_to_string_functions[ffield_class] = &XByteProperty::to_string;
                        XByteProperty::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"UInt16Property"))
                    {
                        m_core_object_pointers[L"UInt16Property"] = ffield_class;
                        XUInt16Property::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"UInt32Property"))
                    {
                        m_core_object_pointers[L"UInt32Property"] = ffield_class;
                        XUInt32Property::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"UInt64Property"))
                    {
                        m_core_object_pointers[L"UInt64Property"] = ffield_class;
                        XUInt64Property::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"BoolProperty"))
                    {
                        m_core_object_pointers[L"BoolProperty"] = ffield_class;
                        m_object_to_string_functions[ffield_class] = &XBoolProperty::to_string;
                        XBoolProperty::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"ArrayProperty"))
                    {
                        m_core_object_pointers[L"ArrayProperty"] = ffield_class;
                        // TODO: Deal with the fact that we don't know the templated type here
                        // Using XIntProperty for testing but it's possible we can use it this way because
                        // to_string is going to need to dynamically (at runtime) figure out what the derived type is regardless
                        // The templated type would be effectively be useless here but is required since the whole class is templated
                        m_object_to_string_functions[ffield_class] = &FArrayProperty::to_string;
                        m_object_to_string_complex_functions[ffield_class] = &FArrayProperty::to_string_complex;
                        FArrayProperty::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"MapProperty"))
                    {
                        m_core_object_pointers[L"MapProperty"] = ffield_class;
                        m_object_to_string_functions[ffield_class] = &FMapProperty::to_string;
                        m_object_to_string_complex_functions[ffield_class] = &FMapProperty::to_string_complex;
                        FMapProperty::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"StructProperty"))
                    {
                        m_core_object_pointers[L"StructProperty"] = ffield_class;
                        m_object_to_string_functions[ffield_class] = &XStructProperty::to_string;
                        XStructProperty::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"ClassProperty"))
                    {
                        m_core_object_pointers[L"ClassProperty"] = ffield_class;
                        m_object_to_string_functions[ffield_class] = &FClassProperty::to_string;
                        FClassProperty::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"SoftClassProperty"))
                    {
                        m_core_object_pointers[L"SoftClassProperty"] = ffield_class;
                        m_object_to_string_functions[ffield_class] = &FClassProperty::to_string;
                        FSoftClassProperty::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"WeakObjectProperty"))
                    {
                        m_core_object_pointers[L"WeakObjectProperty"] = ffield_class;
                        m_object_to_string_functions[ffield_class] = &FWeakObjectProperty::to_string;
                        FWeakObjectProperty::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"LazyObjectProperty"))
                    {
                        m_core_object_pointers[L"LazyObjectProperty"] = ffield_class;
                        m_object_to_string_functions[ffield_class] = &FObjectProperty::to_string;
                        FLazyObjectProperty::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"SoftObjectProperty"))
                    {
                        m_core_object_pointers[L"SoftObjectProperty"] = ffield_class;
                        m_object_to_string_functions[ffield_class] = &FObjectProperty::to_string;
                        FSoftObjectProperty::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"EnumProperty"))
                    {
                        m_core_object_pointers[L"EnumProperty"] = ffield_class;
                        m_object_to_string_functions[ffield_class] = &FEnumProperty<uint8_t>::to_string;
                        FEnumProperty<uint8_t>::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"TextProperty"))
                    {
                        m_core_object_pointers[L"TextProperty"] = ffield_class;
                        m_object_to_string_functions[ffield_class] = &XTextProperty::to_string;
                        XTextProperty::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"StrProperty"))
                    {
                        m_core_object_pointers[L"StrProperty"] = ffield_class;
                        m_object_to_string_functions[ffield_class] = &XStrProperty::to_string;
                        XStrProperty::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"DelegateProperty"))
                    {
                        m_core_object_pointers[L"DelegateProperty"] = ffield_class;
                        m_object_to_string_functions[ffield_class] = &FDelegateProperty::to_string;
                        FDelegateProperty::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"MulticastInlineDelegateProperty"))
                    {
                        m_core_object_pointers[L"MulticastInlineDelegateProperty"] = ffield_class;
                        m_object_to_string_functions[ffield_class] = &FMulticastInlineDelegateProperty::to_string;
                        FMulticastInlineDelegateProperty::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"MulticastSparseDelegateProperty"))
                    {
                        m_core_object_pointers[L"MulticastSparseDelegateProperty"] = ffield_class;
                        m_object_to_string_functions[ffield_class] = &FMulticastSparseDelegateProperty::to_string;
                        FMulticastSparseDelegateProperty::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"InterfaceProperty"))
                    {
                        m_core_object_pointers[L"InterfaceProperty"] = ffield_class;
                        m_object_to_string_functions[ffield_class] = &FInterfaceProperty::to_string;
                        FInterfaceProperty::m_static_obj_variant = ffield_class;
                    }
                    if (type_name == FName(L"FieldPathProperty"))
                    {
                        m_core_object_pointers[L"FieldPathProperty"] = ffield_class;
                        m_object_to_string_functions[ffield_class] = &FFieldPathProperty::to_string;
                        FFieldPathProperty::m_static_obj_variant = ffield_class;
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
                                    FProperty::m_static_obj_variant = ffield_class;
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

            auto add_property = []<typename PropertyType>(const wchar_t* full_property_name, const wchar_t* name, ToStringFuncs to_string_funcs, [[maybe_unused]]PropertyType property_type_object) {
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
                    PropertyType::m_static_obj_variant = static_cast<UClass*>(property);
                }

                if (to_string_funcs.normal)
                {
                    m_object_to_string_functions[property] = to_string_funcs.normal;
                }

                if (to_string_funcs.complex)
                {
                    m_object_to_string_complex_functions[property] = to_string_funcs.complex;
                }
            };

            add_property(L"/Script/CoreUObject.ObjectProperty", L"ObjectProperty", {&FObjectProperty::to_string}, FObjectProperty{});
            add_property(L"/Script/CoreUObject.NumericProperty", L"NumericProperty", {&XNumericProperty::to_string}, XNumericProperty{});
            add_property(L"/Script/CoreUObject.Int8Property", L"Int8Property", {&XInt8Property::to_string}, XInt8Property{});
            add_property(L"/Script/CoreUObject.Int16Property", L"Int16Property", {&XInt16Property::to_string}, XInt16Property{});
            add_property(L"/Script/CoreUObject.IntProperty", L"IntProperty", {&XIntProperty::to_string}, XIntProperty{});
            add_property(L"/Script/CoreUObject.Int64Property", L"Int64Property", {&XInt64Property::to_string}, XInt64Property{});
            add_property(L"/Script/CoreUObject.NameProperty", L"NameProperty", {&XNameProperty::to_string}, XNameProperty{});
            add_property(L"/Script/CoreUObject.FloatProperty", L"FloatProperty", {&XFloatProperty::to_string}, XFloatProperty{});
            add_property(L"/Script/CoreUObject.DoubleProperty", L"DoubleProperty", {&XDoubleProperty::to_string}, XDoubleProperty{});
            add_property(L"/Script/CoreUObject.ByteProperty", L"ByteProperty", {&XByteProperty::to_string}, XByteProperty{});
            add_property(L"/Script/CoreUObject.UInt16Property", L"UInt16Property", {&XUInt16Property::to_string}, XUInt16Property{});
            add_property(L"/Script/CoreUObject.UInt32Property", L"UInt32Property", {&XUInt32Property::to_string}, XUInt32Property{});
            add_property(L"/Script/CoreUObject.UInt64Property", L"UInt64Property", {&XUInt64Property::to_string}, XUInt64Property{});
            add_property(L"/Script/CoreUObject.BoolProperty", L"BoolProperty", {&XBoolProperty::to_string}, XBoolProperty{});
            add_property(L"/Script/CoreUObject.ArrayProperty", L"ArrayProperty", {&FArrayProperty::to_string, &FArrayProperty::to_string_complex}, FArrayProperty{});
            add_property(L"/Script/CoreUObject.MapProperty", L"MapProperty", {&FMapProperty::to_string, &FMapProperty::to_string_complex}, FMapProperty{});
            add_property(L"/Script/CoreUObject.StructProperty", L"StructProperty", {&XStructProperty::to_string}, XStructProperty{});
            add_property(L"/Script/CoreUObject.ClassProperty", L"ClassProperty", {&FClassProperty::to_string}, FClassProperty{});
            add_property(L"/Script/CoreUObject.SoftClassProperty", L"SoftClassProperty", {&FClassProperty::to_string}, FSoftClassProperty{});
            add_property(L"/Script/CoreUObject.WeakObjectProperty", L"WeakObjectProperty", {&FWeakObjectProperty::to_string}, FWeakObjectProperty{});
            add_property(L"/Script/CoreUObject.LazyObjectProperty", L"LazyObjectProperty", {&FObjectProperty::to_string}, FLazyObjectProperty{});
            add_property(L"/Script/CoreUObject.SoftObjectProperty", L"SoftObjectProperty", {&FObjectProperty::to_string}, FSoftObjectProperty{});
            if (Version::is_atleast(4, 15)) { add_property(L"/Script/CoreUObject.EnumProperty", L"EnumProperty", {&FEnumProperty<uint8_t>::to_string}, FEnumProperty<uint8_t>{}); }
            add_property(L"/Script/CoreUObject.TextProperty", L"TextProperty", {&XTextProperty::to_string}, XTextProperty{});
            add_property(L"/Script/CoreUObject.StrProperty", L"StrProperty", {&XStrProperty::to_string}, XStrProperty{});
            add_property(L"/Script/CoreUObject.DelegateProperty", L"DelegateProperty", {&FDelegateProperty::to_string}, FDelegateProperty{});
            add_property(L"/Script/CoreUObject.MulticastDelegateProperty", L"MulticastDelegateProperty", {&FMulticastInlineDelegateProperty::to_string}, FMulticastInlineDelegateProperty{});
            add_property(L"/Script/CoreUObject.MulticastInlineDelegateProperty", L"MulticastInlineDelegateProperty", {&FMulticastInlineDelegateProperty::to_string}, FMulticastInlineDelegateProperty{});
            add_property(L"/Script/CoreUObject.MulticastSparseDelegateProperty", L"MulticastSparseDelegateProperty", {&FMulticastSparseDelegateProperty::to_string}, FMulticastSparseDelegateProperty{});
            add_property(L"/Script/CoreUObject.InterfaceProperty", L"InterfaceProperty", {&FInterfaceProperty::to_string}, FInterfaceProperty{});

            // Not yet supported, only here for completion and to prevent crashes
            // Not all of these will be found right now, need to call this function with more UObjects that actually have these properties
            add_property(L"/Script/CoreUObject.SetProperty", L"SetProperty", {}, int32_t{});

            // Find the root 'Property' FField. This may not be necessary but I'm leaving it here for now, might be useful later.
            add_property(L"/Script/CoreUObject.Property", L"Property", {}, int32_t{});
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