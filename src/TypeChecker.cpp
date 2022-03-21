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
#include <Unreal/UGameViewportClient.hpp>
#include <Unreal/UPackage.hpp>
#include <Unreal/Property/FNumericProperty.hpp>
#include <Unreal/Property/NumericPropertyTypes.hpp>
#include <Unreal/Property/FObjectProperty.hpp>
#include <Unreal/Property/FWeakObjectProperty.hpp>
#include <Unreal/Property/FLazyObjectProperty.hpp>
#include <Unreal/Property/FSoftObjectProperty.hpp>
#include <Unreal/Property/FClassProperty.hpp>
#include <Unreal/Property/FSoftClassProperty.hpp>
#include <Unreal/Property/FArrayProperty.hpp>
#include <Unreal/Property/FMapProperty.hpp>
#include <Unreal/Property/FBoolProperty.hpp>
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
    std::unordered_map<void*, TypeChecker::ObjectToStringDecl> TypeChecker::m_object_to_string_functions{};
    std::unordered_map<void*, TypeChecker::ObjectToStringComplexDecl> TypeChecker::m_object_to_string_complex_functions{};
    FName GWorldName{};
    FName GLevelName{};
    FName GPackageName{};
    FName GPropertyName{};
    FName GFunctionName{};

    auto TypeChecker::get_world_name() -> FName
    {
        return GWorldName;
    }

    auto TypeChecker::get_level_name() -> FName
    {
        return GLevelName;
    }

    auto TypeChecker::store_all_object_names() -> void
    {
        //@formatter:off
        // Misc
        GWorldName    = FName(L"World");
        GLevelName    = FName(L"Level");
        GPackageName  = FName(L"Package");
        GPropertyName = FName(L"Property");
        GFunctionName = FName(L"Function");
        //@formatter:on
    }

    auto TypeChecker::store_all_object_types() -> bool
    {
        UObject::StaticClassStorage = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Object");
        UStruct::StaticClassStorage = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Struct");
        AActor::StaticClassStorage = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, L"/Script/Engine.Actor");

        UClass* enum_ptr = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Enum");
        UEnum::StaticClassStorage = enum_ptr;

        UClass* user_defined_enum_ptr = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, L"/Script/Engine.UserDefinedEnum");
        UUserDefinedEnum::StaticClassStorage = user_defined_enum_ptr;

        UClass* class_ptr = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Class");
        UClass::StaticClassStorage = class_ptr;

        UClass* bp_generated_class_ptr = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, L"/Script/Engine.BlueprintGeneratedClass");
        UBlueprintGeneratedClass::StaticClassStorage = bp_generated_class_ptr;

        UClass* anim_bp_generated_class_ptr = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, L"/Script/Engine.AnimBlueprintGeneratedClass");
        UAnimBlueprintGeneratedClass::StaticClassStorage = anim_bp_generated_class_ptr;

        //UClass* widget_bp_generated_class_ptr = UObjectGlobals::static_find_object<UClass*>(nullptr, nullptr, L"/Script/Engine.WidgetBlueprintGeneratedClass");
        //UAnimBlueprintGeneratedClass::StaticClassStorage = widget_bp_generated_class_ptr;

        // Not available in 4.12 (I've not checked exactly when it starts being available)
        UClass* asset_data_ptr = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, STR("/Script/AssetRegistry.AssetData"));
        if (!asset_data_ptr)
        {
            // In 4.26, they moved it from the 'AssetRegistry' package to the 'CoreUObject' package
            asset_data_ptr = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, STR("/Script/CoreUObject.AssetData"));
        }
        FAssetData::StaticClassStorage = asset_data_ptr;

        UPackage::StaticClassStorage = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Package");
        UInterface::StaticClassStorage = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Interface");
        UActorComponent::StaticClassStorage = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, L"/Script/Engine.ActorComponent");
        USceneComponent::StaticClassStorage = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, L"/Script/Engine.SceneComponent");
        UGameViewportClient::StaticClassStorage = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, L"/Script/Engine.GameViewportClient");

        UFunction::StaticClassStorage = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, STR("/Script/CoreUObject.Function"));
        //UDelegateFunction::StaticClassStorage = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, STR("/Script/CoreUObject.DelegateFunction"));

        if (Version::IsAtLeast(4, 23))
        {
            //USparseDelegateFunction::StaticClassStorage = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, STR("/Script/CoreUObject.SparseDelegateFunction"));
        }

        UClass* script_struct_ptr = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.ScriptStruct");
        UScriptStruct::StaticClassStorage = script_struct_ptr;

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
        if (Version::IsAtLeast(4, 25))
        {
            auto find_all_property_types = [](const std::wstring& obj_string) -> void {
                UClass* actor_obj = static_cast<UClass*>(UObjectGlobals::StaticFindObject(nullptr, nullptr, obj_string));
                if (!actor_obj) { return; }

                // Manually iterating fields here because 'ForEachProperty' isn't ready until after this function is done
                FField* field = actor_obj->GetChildProperties();
                while (field)
                {
                    // Hard-coded offset cast here because 'FField::GetClass' is not ready until after this function is done
                    FFieldClass* ffield_class = Helper::Casting::ptr_cast_deref<FFieldClass*>(field, FFieldClassOffset);
                    if (!ffield_class)
                    {
                        field = field->GetNextFFieldUnsafe();
                        continue;
                    }

                    FName type_name = ffield_class->GetFName();

                    // TODO: Look at the dumped objects and put every single FField type in here
                    // At the moment there are probably some missing types

                    // Fully supported
                    if (type_name == FName(L"ObjectProperty"))
                    {
                        FObjectProperty::StaticClassStorage = ffield_class;

                        // This doesn't need to be in 'ObjectProperty', it can be in any property
                        FFieldClass* ffield_super_class = ffield_class->GetSuperClass();
                        if (ffield_super_class)
                        {
                            do
                            {
                                FName super_type_name = ffield_super_class->GetFName();
                                if (super_type_name == GPropertyName)
                                {
                                    FProperty::StaticClassStorage = ffield_super_class;
                                    break;
                                }

                                ffield_super_class = ffield_super_class->GetSuperClass();
                            } while (ffield_super_class);
                        }
                    }
                    if (type_name == FName(L"Int8Property"))
                    {
                        FInt8Property::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"Int16Property"))
                    {
                        FInt16Property::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"IntProperty"))
                    {
                        FIntProperty::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"Int64Property"))
                    {
                        FInt64Property::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"NameProperty"))
                    {
                        FNameProperty::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"FloatProperty"))
                    {
                        FFloatProperty::StaticClassStorage = static_cast<FFieldClass*>(ffield_class);

                        // 'NumericProperty' doesn't appear as the direct type for any properties
                        // Because of this, we need to use a property that we know has 'NumericProperty' in its super chain
                        // We're using 'FFloatProperty' here for no particular reason other than that it has 'NumericProperty' as it's super
                        FNumericProperty::StaticClassStorage = ffield_class->SuperClass;
                    }
                    if (type_name == FName(L"DoubleProperty"))
                    {
                        FDoubleProperty::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"ByteProperty"))
                    {
                        FByteProperty::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"UInt16Property"))
                    {
                        FUInt16Property::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"UInt32Property"))
                    {
                        FUInt32Property::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"UInt64Property"))
                    {
                        FUInt64Property::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"BoolProperty"))
                    {
                        FBoolProperty::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"ArrayProperty"))
                    {
                        FArrayProperty::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"MapProperty"))
                    {
                        FMapProperty::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"StructProperty"))
                    {
                        FStructProperty::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"ClassProperty"))
                    {
                        FClassProperty::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"SoftClassProperty"))
                    {
                        FSoftClassProperty::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"WeakObjectProperty"))
                    {
                        FWeakObjectProperty::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"LazyObjectProperty"))
                    {
                        FLazyObjectProperty::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"SoftObjectProperty"))
                    {
                        FSoftObjectProperty::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"EnumProperty"))
                    {
                        FEnumProperty::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"TextProperty"))
                    {
                        FTextProperty::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"StrProperty"))
                    {
                        FStrProperty::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"DelegateProperty"))
                    {
                        FDelegateProperty::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"MulticastInlineDelegateProperty"))
                    {
                        FMulticastInlineDelegateProperty::StaticClassStorage = ffield_class;
                        FMulticastDelegateProperty::StaticClassStorage = ffield_class->GetSuperClass();
                    }
                    if (type_name == FName(L"MulticastSparseDelegateProperty"))
                    {
                        FMulticastSparseDelegateProperty::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"InterfaceProperty"))
                    {
                        FInterfaceProperty::StaticClassStorage = ffield_class;
                    }
                    if (type_name == FName(L"FieldPathProperty"))
                    {
                        FFieldPathProperty::StaticClassStorage = ffield_class;
                    }

                    field = field->GetNextFFieldUnsafe();
                }
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
            // Delegate
            find_all_property_types(L"/Script/UMG.Widget");
            // SoftObject
            find_all_property_types(L"/Script/Engine.LevelStreaming");
            // SoftClass
            find_all_property_types(L"/Script/Engine.PrimaryAssetTypeInfo");
            // FieldPath
            find_all_property_types(L"/Script/Engine.InterpTrackInstProperty");
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
                UObject* property = UObjectGlobals::StaticFindObject(nullptr, nullptr, full_property_name);
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

                if constexpr (!std::is_same_v<PropertyType, int32_t>)
                {
                    PropertyType::StaticClassStorage = static_cast<UClass*>(property);
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
            if (Version::IsAtLeast(4, 15)) { add_property(L"/Script/CoreUObject.EnumProperty", L"EnumProperty", /*{&FEnumProperty::to_string},*/ FEnumProperty{}); }
            add_property(L"/Script/CoreUObject.TextProperty", L"TextProperty", /*{&FTextProperty::to_string},*/ FTextProperty{});
            add_property(L"/Script/CoreUObject.StrProperty", L"StrProperty", /*{&FStrProperty::to_string},*/ FStrProperty{});
            add_property(L"/Script/CoreUObject.DelegateProperty", L"DelegateProperty", /*{&FDelegateProperty::to_string},*/ FDelegateProperty{});
            add_property(L"/Script/CoreUObject.MulticastDelegateProperty", L"MulticastDelegateProperty", /*{&FMulticastInlineDelegateProperty::to_string},*/ FMulticastDelegateProperty{});
            add_property(L"/Script/CoreUObject.MulticastInlineDelegateProperty", L"MulticastInlineDelegateProperty", /*{&FMulticastInlineDelegateProperty::to_string},*/ FMulticastInlineDelegateProperty{});
            add_property(L"/Script/CoreUObject.MulticastSparseDelegateProperty", L"MulticastSparseDelegateProperty", /*{&FMulticastSparseDelegateProperty::to_string},*/ FMulticastSparseDelegateProperty{});
            add_property(L"/Script/CoreUObject.InterfaceProperty", L"InterfaceProperty", /*{&FInterfaceProperty::to_string},*/ FInterfaceProperty{});

            // Not yet supported, only here for completion and to prevent crashes
            // Not all of these will be found right now, need to call this function with more UObjects that actually have these properties
            add_property(L"/Script/CoreUObject.SetProperty", L"SetProperty", /*{},*/ FSetProperty{});

            // Find the root 'Property' FField. This may not be necessary but I'm leaving it here for now, might be useful later.
            add_property(L"/Script/CoreUObject.Property", L"Property", /*{},*/ FProperty{});
        }

        return true;
    }

    auto TypeChecker::is_property(FField* field) -> bool
    {
        UObject* object = std::bit_cast<UObject*>(field);
        return is_property(object);
    }

    auto TypeChecker::is_property(UObject* object) -> bool
    {
        UClass* obj_class = object->GetClass();
        if (!obj_class) { return false; }

        if (obj_class->GetFName() == GPropertyName) { return true; }

        UStruct* super_struct = obj_class->GetSuperStruct();
        while (super_struct)
        {
            if (super_struct->GetFName() == GPropertyName)
            {
                return true;
            }

            super_struct = super_struct->GetSuperStruct();
        }

        return false;
    }
}