#include <algorithm>
#include <stdexcept>
#include <format>

#include <Unreal/StaticOffsetFinder.hpp>
#include <Helpers/Casting.hpp>
#include <Helpers/Integer.hpp>
#include <Unreal/UObjectGlobals.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UStruct.hpp>
#include <Unreal/FField.hpp>
#include <Unreal/UFunction.hpp>
#include <Unreal/FProperty.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/NameTypes.hpp>
#include <Unreal/TypeChecker.hpp>
#include <Unreal/UnrealVersion.hpp>
#include <DynamicOutput/DynamicOutput.hpp>

namespace RC::Unreal
{
    std::array<int32_t, static_cast<std::underlying_type<MemberOffsets>::type>(MemberOffsets::Max)> StaticOffsetFinder::m_static_offsets{};
    std::array<const wchar_t*, static_cast<std::underlying_type<MemberOffsets>::type>(MemberOffsets::Max)> StaticOffsetFinder::m_debug_static_offset_names{};
    bool StaticOffsetFinder::m_initialized{false};
    bool StaticOffsetFinder::UE_BLUEPRINT_EVENTGRAPH_FASTCALLS{};

    auto static throw_missed_offset(const std::vector<StaticOffsetFinder::MemberOffsets>& member_offsets, File::StringViewType message) -> void
    {
        File::StringType error{STR("Offsets not found, you'll have to override it in the ini file.\nThe keys in UE4SS-settings.ini are:\n")};
        for (const auto& member_offset : member_offsets)
        {
            error.append(std::format(STR("{}\n"), StaticOffsetFinder::member_offset_to_string(member_offset)));
        }
        error.append(std::format(STR("Message: {}\n"), message));

        throw std::runtime_error{fmt("%S", error.c_str())};
    }
    auto static throw_missed_offset(StaticOffsetFinder::MemberOffsets member_offset, File::StringViewType message) -> void
    {
        //throw std::runtime_error{fmt("Offset not found, you'll have to override it in the ini file.\nThe key in UE4SS-settings.ini is: %S\nMessage: %S\n", StaticOffsetFinder::member_offset_to_string(member_offset), message.data())};
        throw_missed_offset(std::vector<StaticOffsetFinder::MemberOffsets>{member_offset}, message);
    }

    auto StaticOffsetFinder::initialize() -> void
    {
        if (is_initialized())
        {
            throw std::runtime_error{"[StaticOffsetFinder::initialize] Attempted to initialize twice"};
        }

        // Fill arrays with default values
        m_static_offsets.fill(-1);
        m_debug_static_offset_names.fill(STR("Unset"));
        m_initialized = true;
    }

    auto StaticOffsetFinder::is_initialized() -> bool
    {
        return m_initialized;
    }

    auto StaticOffsetFinder::add_static_offset(MemberOffsets member, int32_t offset, const wchar_t* debug_name) -> void
    {
        if (!is_initialized())
        {
            throw std::runtime_error{"[StaticOffsetFinder::add_static_offset] StaticOffsetFinder was uninitialized"};
        }

        auto member_underlying = static_cast<std::underlying_type<MemberOffsets>::type>(member);
        m_static_offsets[member_underlying] = offset;
        m_debug_static_offset_names[member_underlying] = debug_name;
    }

    auto StaticOffsetFinder::retrieve_static_offset(MemberOffsets member) -> int32_t
    {
        if (!is_initialized())
        {
            throw std::runtime_error{"[StaticOffsetFinder::retrieve_static_offset] StaticOffsetFinder was uninitialized"};
        }

        return m_static_offsets[static_cast<std::underlying_type<MemberOffsets>::type>(member)];
    }

    auto StaticOffsetFinder::output_all_member_offsets() -> void
    {
        if (!is_initialized())
        {
            throw std::runtime_error{"[StaticOffsetFinder::output_all_member_offsets] StaticOffsetFinder was uninitialized"};
        }

        Output::send(STR("\n##### STATIC MEMBER OFFSETS START #####\n\n"));
        for (int32_t i = 0; i < static_cast<std::underlying_type<MemberOffsets>::type>(MemberOffsets::Max); i++)
        {
            const wchar_t* debug_name = m_debug_static_offset_names[i];
            Output::send(STR("Key: {} | Value: 0x{:X}\n"), debug_name ? debug_name : L"NoName", m_static_offsets[i]);
        }
        Output::send(STR("\n##### STATIC MEMBER OFFSETS END #####\n\n"));
    }

    auto StaticOffsetFinder::get_property_from_uobject(const wchar_t* uclass_string, const wchar_t* property_string) -> void*
    {
        UClass* uclass = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, uclass_string);
        if (!uclass) { return nullptr; }

        void* prop{};
        FName prop_name = FName(property_string);

        uclass->ForEachProperty([&](FProperty* child) {
            if (child->GetFName() == prop_name)
            {
                prop = child;
                return LoopAction::Break;
            }
            else
            {
                return LoopAction::Continue;
            }
        });

        return prop;
    }

    auto StaticOffsetFinder::find_independent_offsets() -> void
    {
        if (!is_initialized())
        {
            initialize();
        }

        find_class_private();
        find_outer_private();
        find_uobject_fname();
    }

    auto StaticOffsetFinder::find_offsets() -> void
    {
        find_super_struct();

        Output::send(STR("engine major: {} | engine minor: {}\n"), Version::Major, Version::Minor);

        find_children();
        find_ustruct_next();

        if (Version::IsAtLeast(4, 25))
        {
            find_child_properties();
            find_ffield_owner();
            find_ffield_next();
        }

        // Need to gain access to type information
        // So setup the type checker here
        if (!TypeChecker::store_all_object_types())
        {
            throw std::runtime_error{"TypeChecker: Was unable to find some or all of the required core objects"};
        }

        if (Version::IsAtLeast(4, 25))
        {
            find_ffield_class();
            find_ffield_fname();

            find_fieldpathproperty_property_class();
        }

        if (Version::IsAtLeast(4, 13))
        {
            find_setproperty_elementprop();
        }


        find_structproperty_struct();
        find_offset_internal();

        find_ustruct_properties_size_and_min_alignment();
        find_num_parms_and_return_value_offset();
        find_function_func_offset();

        find_bool_member_variables();
        find_class_flags();
        find_class_default_object();
        find_property_flags();
        find_property_class_and_meta_class();

        if (Version::IsAtLeast(4, 18))
        {
            find_softclassproperty_meta_class();
        }

        // EnumProperty does not exist in <4.15
        // However it's still required to find some enum related things
        // This is because ByteProperty also uses it
        find_enum_members();
        find_byteproperty_enum();

        find_mapproperty_key_value_prop();
        find_delegateproperty_signature_function();
        find_multicastdelegateproperty_signature_function();
        find_interfaceproperty_interface_class();
        find_ustruct_reflink();
        find_uscriptstruct_structflags();
        find_uclass_interfaces();
    }

    auto StaticOffsetFinder::find_class_private() -> void
    {
        // Hard-coded because the full implementation requires StaticFindObject and StaticFindObject requires the ClassPrivate offset
        // The offset for ClassPrivate has never changed, and I don't think it's likely to change
        constexpr static int32_t uobject_class_private_offset = 0x10;

        if (retrieve_static_offset(MemberOffsets::UObject_ClassPrivate) == -1)
        {
            add_static_offset(MemberOffsets::UObject_ClassPrivate, uobject_class_private_offset, STR("ClassPrivate"));
        }

        if (Version::IsBelow(4, 25) && retrieve_static_offset(MemberOffsets::FField_Class) == -1)
        {
            add_static_offset(MemberOffsets::FField_Class, uobject_class_private_offset, STR("FFieldClass"));
        }
    }

    auto StaticOffsetFinder::find_uobject_fname() -> void
    {
        // Hard-coded because FName interaction is required very early on in initialization, too early for full implementation
        // Similarly to ClassPrivate, the offset for NamePrivate has never changed, and I therefore don't think it's likely to change
        if (retrieve_static_offset(MemberOffsets::UObject_NamePrivate) != -1 &&
            retrieve_static_offset(MemberOffsets::FField_NamePrivate) != -1)
        {
            return;
        }

        if (retrieve_static_offset(MemberOffsets::UObject_NamePrivate) == -1)
        {
            add_static_offset(MemberOffsets::UObject_NamePrivate, 0x18, STR("NamePrivate_UObject"));
        }

        // Add an identical offset for FField::FName also but only if the UE version is <4.24
        // This enables the use of XField::get_fname() to work properly in <4.25 without checking at runtime if the object is an FField
        // If the UE version is >=4.25 then this offset will be set by StaticOffsetFinder::find_ffield_fname()
        if (Version::IsBelow(4, 25) && retrieve_static_offset(MemberOffsets::FField_NamePrivate) == -1)
        {
            add_static_offset(MemberOffsets::FField_NamePrivate, 0x18, STR("NamePrivate_FField"));
        }
    }

    auto StaticOffsetFinder::find_outer_private() -> void
    {
        // Hard-coded because OuterPrivate interaction is required very early on in initialization, too early for full implementation
        // Similarly to ClassPrivate, the offset for OuterPrivate has never changed, and I therefore don't think it's likely to change
#ifdef WITH_CASE_PRESERVING_NAME
        constexpr static int32_t uobject_outer_private_offset = 0x28;
#else
        constexpr static int32_t uobject_outer_private_offset = 0x20;
#endif

        if (retrieve_static_offset(MemberOffsets::UObject_OuterPrivate) == -1)
        {
            add_static_offset(MemberOffsets::UObject_OuterPrivate, uobject_outer_private_offset, STR("OuterPrivate"));
        }
    }

    auto StaticOffsetFinder::find_super_struct() -> void
    {
        if (retrieve_static_offset(MemberOffsets::UStruct_SuperStruct) != -1) { return; }

        void* core_struct = UObjectGlobals::StaticFindObject(nullptr, nullptr, L"/Script/CoreUObject.Struct");
        if (!core_struct)
        {
            throw_missed_offset(
                    MemberOffsets::UStruct_SuperStruct,
                    STR("Was unable to locate CoreUObject.Struct")
            );
        }

        void* core_class = UObjectGlobals::StaticFindObject(nullptr, nullptr, L"/Script/CoreUObject.Class");
        if (!core_class)
        {
            throw_missed_offset(
                    MemberOffsets::UStruct_SuperStruct,
                    STR("Was unable to find /Script/CoreUObject.Class")
            );
        }

        bool success = false;
        for (int i = 0x0; i < 0x60; i += 0x8)
        {
            void* unreal_ptr_to_test = Helper::Casting::offset_deref<void*>(core_class, i);
            if (unreal_ptr_to_test == core_struct)
            {
                add_static_offset(MemberOffsets::UStruct_SuperStruct, i, STR("SuperStruct"));
                success = true;
                break;
            }
        }

        if (!success)
        {
            throw_missed_offset(
                    MemberOffsets::UStruct_SuperStruct,
                    STR("Was unable to find SuperStruct")
            );
        }
    }

    auto StaticOffsetFinder::find_children() -> void
    {
        if (retrieve_static_offset(MemberOffsets::XField_Children) != -1) { return; }

        UClass* character_class = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, STR("/Script/Engine.Pawn"));
        if (!character_class)
        {
            throw_missed_offset(
                    MemberOffsets::XField_Children,
                    STR("Was unable to find Pawn")
            );
        }

        const File::CharType* child_name_to_find = []() {
            if (Version::IsBelow(4, 25))
            {
                return STR("/Script/Engine.Pawn:bUseControllerRotationPitch");
            }
            else
            {
                return STR("/Script/Engine.Pawn:SpawnDefaultController");
            }
        }();

        UObject* child_to_find = UObjectGlobals::StaticFindObject(nullptr, nullptr, child_name_to_find);
        if (!child_to_find)
        {
            throw_missed_offset(
                    MemberOffsets::XField_Children,
                    STR("Was unable to find object to compare with i")
            );
        }

        bool success = false;
        for (int i = 0x0; i < 0x90; i += 0x8)
        {
            UFunction* unreal_ptr_to_test = Helper::Casting::offset_deref<UFunction*>(character_class, i);
            if (unreal_ptr_to_test == child_to_find)
            {
                add_static_offset(MemberOffsets::XField_Children, i, STR("Children"));
                if (Version::IsBelow(4, 25))
                {
                    add_static_offset(MemberOffsets::XField_ChildProperties, i, STR("XField_ChildProperties"));
                }
                success = true;
                break;
            }
        }

        if (!success)
        {
            throw_missed_offset(
                    MemberOffsets::XField_Children,
                    STR("Was unable to find Children")
            );
        }
    }

    auto StaticOffsetFinder::find_ustruct_next() -> void
    {
        if (retrieve_static_offset(MemberOffsets::UField_Next) != -1) { return; }

        UFunction* function = UObjectGlobals::StaticFindObject<UFunction*>(nullptr, nullptr, STR("/Script/Engine.Character:UnCrouch"));
        if (!function)
        {
            throw_missed_offset(
                    MemberOffsets::UField_Next,
                    STR("Was unable to find Character:UnCrouch")
            );
        }

        UFunction* next_function = UObjectGlobals::StaticFindObject<UFunction*>(nullptr, nullptr, STR("/Script/Engine.Character:StopJumping"));
        if (!next_function)
        {
            throw_missed_offset(
                    MemberOffsets::UField_Next,
                    STR("Was unable to find Character:StopJumping")
            );
        }

        bool success = false;
        for (int i = 0x0; i < 0x90; i += 0x8)
        {
            UFunction* unreal_ptr_to_test = Helper::Casting::offset_deref<UFunction*>(function, i);
            if (unreal_ptr_to_test == next_function)
            {
                add_static_offset(MemberOffsets::UField_Next, i, STR("UField_Next"));
                if (Version::IsBelow(4, 25))
                {
                    add_static_offset(MemberOffsets::FField_Next, i, STR("FField_Next"));
                    add_static_offset(MemberOffsets::XField_Next, i, STR("XField_Next"));
                }
                success = true;
                break;
            }
        }

        if (!success)
        {
            throw_missed_offset(
                    MemberOffsets::UField_Next,
                    STR("Was unable to find Next")
            );
        }
    }

    auto StaticOffsetFinder::find_child_properties() -> void
    {
        if (retrieve_static_offset(MemberOffsets::XField_ChildProperties) != -1)
        {
            return;
        }

        // Find AActor UClass which should always contain some properties
        UClass* aactor = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, L"/Script/Engine.Actor");
        if (!aactor)
        {
            throw_missed_offset(
                    MemberOffsets::XField_ChildProperties,
                    STR("Was unable to find /Script/Engine.Actor")
            );
        }

        // Iterate every 8 bytes of AActor
        bool success = false;
        for (int i = 0x0; i < 0x90; i += 0x8)
        {
            // Add 'i' to AActor, deref and cast to UClass*
            // This value may or may not be a pointer, we check this below
            UClass* potential_child_properties_ptr = Helper::Casting::offset_deref<UClass*>(aactor, i);

            // Hard-coded offset here unfortunately
            // The 'find_ffield_owner()' function requires the 'ChildProperties' offset
            // The 'find_child_properties()' function requires the 'FField.Owner' offset
            // They depend on each other and that can't work so for now I'll just hard-code the 'FField.Owner' offset just this once
            // This isn't super likely to change but it will change if the UE devs makes certain changes in the 'FField' class
            constexpr int32_t ffield_owner_offset = 0x10;

            // Safely attempt to dereference the value at 'AActor+i'
            // If the value is non-pointer then the return value of 'offset_deref_safe' will be nullptr
            // This line also adds the 'Owner' offset and dereferences the pointer if it's not a non-pointer value
            UClass* potential_ffield_owner_ptr = Helper::Casting::offset_deref_safe<UClass*>(potential_child_properties_ptr, ffield_owner_offset);

            // If the value is a pointer
            if (potential_ffield_owner_ptr)
            {
                // If the value at the 'Owner' offset is a pointer back to 'AActor' then we've succeeded in finding the 'ChildProperties' offset
                if (potential_ffield_owner_ptr == aactor)
                {
                    if (retrieve_static_offset(MemberOffsets::XField_ChildProperties) == -1)
                    {
                        add_static_offset(MemberOffsets::XField_ChildProperties, i, STR("ChildProperties"));
                    }

                    success = true;
                    break;
                }
            }
        }

        if (!success)
        {
            throw_missed_offset(
                    MemberOffsets::XField_ChildProperties,
                    STR("Was unable to find ChildProperties")
            );
        }
    }

    auto StaticOffsetFinder::find_ffield_owner() -> void
    {
        if (retrieve_static_offset(MemberOffsets::FField_Owner) != -1) { return; }

        UStruct* aactor = UObjectGlobals::StaticFindObject<UStruct*>(nullptr, nullptr, L"/Script/Engine.Actor");
        if (!aactor)
        {
            throw_missed_offset(
                    MemberOffsets::FField_Owner,
                    STR("Was unable to locate AActor")
            );
        }

        // ChildProperties offset is required so fail if it doesn't exist
        if (retrieve_static_offset(MemberOffsets::XField_ChildProperties) == -1)
        {
            throw_missed_offset(
                    MemberOffsets::FField_Owner,
                    STR("ChildProperties offset was zero. Please execute find_child_properties()")
            );
        }

        void* ffield_children = aactor->GetChildProperties();

        bool success = false;
        for (int i = 0x0; i < 0x90; i += 0x8)
        {
            UStruct* unreal_class = Helper::Casting::offset_deref<UStruct*>(ffield_children, i);

            if (unreal_class == aactor)
            {
                add_static_offset(MemberOffsets::FField_Owner, i, STR("FFieldOwner"));
                success = true;
                break;
            }
        }

        if (!success)
        {
            throw_missed_offset(
                    MemberOffsets::FField_Owner,
                    STR("Was unable to find FField.Owner")
            );
        }
    }

    auto StaticOffsetFinder::find_ffield_next() -> void
    {
        if (retrieve_static_offset(MemberOffsets::FField_Next) != -1 &&
            retrieve_static_offset(MemberOffsets::XField_Next) != -1)
        {
            return;
        }

        UClass* aactor = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, L"/Script/Engine.Actor");
        if (!aactor)
        {
            throw_missed_offset(
                    {
                            MemberOffsets::FField_Next,
                            MemberOffsets::XField_Next,
                    },
                    STR("Was unable to find Was unable to locate AActor")
            );
        }

        if (retrieve_static_offset(MemberOffsets::XField_ChildProperties) == -1 ||
            retrieve_static_offset(MemberOffsets::FField_Owner) == -1
                )
        {
            throw_missed_offset(
                    {
                            MemberOffsets::FField_Next,
                            MemberOffsets::XField_Next,
                    },
                    STR("ChildProperties offset or FFieldOwner offset was zero. Please execute find_child_properties() and find_ffield_owner()")
            );
        }

        void* child_properties = aactor->GetChildProperties();

        bool success = false;
        for (int i = 0x0; i < 0x90; i += 0x8)
        {
            // UClass of child_properties
            UObject* unreal_class = Helper::Casting::offset_deref<UObject*>(child_properties, i);

            UObject* unreal_class2 = Helper::Casting::offset_deref_safe<UObject*>(unreal_class, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::FField_Owner));

            if (unreal_class2 == aactor)
            {
                if (retrieve_static_offset(MemberOffsets::FField_Next) == -1)
                {
                    add_static_offset(MemberOffsets::FField_Next, i, STR("FField_Next"));
                }
                if (retrieve_static_offset(MemberOffsets::XField_Next) == -1)
                {
                    add_static_offset(MemberOffsets::XField_Next, i, STR("XField_Next"));
                }

                success = true;
                break;
            }
        }

        if (!success)
        {
            throw_missed_offset(
                    {
                            MemberOffsets::FField_Next,
                            MemberOffsets::XField_Next,
                    },
                    STR("Was unable to find FField.Next")
            );
        }
    }

    auto StaticOffsetFinder::find_ffield_class() -> void
    {
        if (retrieve_static_offset(MemberOffsets::FField_Class) != -1) { return; }

        // Hardcoded for now
        // TODO: Do proper implementation
        add_static_offset(MemberOffsets::FField_Class, 0x8, STR("FFieldClass"));
    }

    auto StaticOffsetFinder::find_ffield_fname() -> void
    {
        if (retrieve_static_offset(MemberOffsets::FField_NamePrivate) != -1) { return; }

        if (retrieve_static_offset(MemberOffsets::XField_ChildProperties) == -1)
        {
            throw_missed_offset(
                    MemberOffsets::FField_NamePrivate,
                    STR("find_child_properties() must be run before find_ffield_fname()")
            );
        }

        if (retrieve_static_offset(MemberOffsets::FField_Next) == -1)
        {
            throw_missed_offset(
                    MemberOffsets::FField_NamePrivate,
                    STR("find_ffield_next() must be run before find_ffield_fname()")
            );
        }

        if (!FName::ConstructorInternal.is_ready())
        {
            throw_missed_offset(
                    MemberOffsets::FField_NamePrivate,
                    STR("An address to FName::FName is required (none was found)")
            );
        }

        UClass* actorClass = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, L"/Script/Engine.Actor");
        if (!actorClass)
        {
            throw_missed_offset(
                    MemberOffsets::FField_NamePrivate,
                    STR("Was unable to find /Script/Engine.Actor")
            );
        }

        FName knownCoreFName = FName(L"bCanBeDamaged");

        FField* property = actorClass->GetChildProperties();
        while (property)
        {
            for (int i = 0x8; i < 0x50; i += 0x8)
            {
                FName nameToTest = Helper::Casting::offset_deref<FName>(property, i);

                if (nameToTest == knownCoreFName)
                {
                    add_static_offset(MemberOffsets::FField_NamePrivate, i, STR("NamePrivate_FField"));
                    return;
                }
            }

            property = property->GetNextFFieldUnsafe();
        }

        throw_missed_offset(
                MemberOffsets::FField_NamePrivate,
                STR("Was unable to find FField_NamePrivate")
        );
    }

    auto StaticOffsetFinder::find_structproperty_struct() -> void
    {
        if (retrieve_static_offset(MemberOffsets::StructProperty_Struct) != -1) { return; }

        void* fvector = UObjectGlobals::StaticFindObject(nullptr, nullptr, L"/Script/CoreUObject.Vector");
        if (!fvector)
        {
            throw_missed_offset(
                    {
                            MemberOffsets::StructProperty_Struct,
                            MemberOffsets::ArrayProperty_Inner,
                    },
                    STR("Was unable to find CoreUObject.Vector")
            );
        }

        void* prop_velocity = get_property_from_uobject(L"/Script/Engine.MovementComponent", L"Velocity");
        if (!prop_velocity)
        {
            throw_missed_offset(
                    {
                            MemberOffsets::StructProperty_Struct,
                            MemberOffsets::ArrayProperty_Inner,
                    },
                    STR("Was unable to find 'StructProperty /Script/Engine.MovementComponent:Velocity'")
            );
        }

        bool success{};
        for (int32_t i = 0x0; i < 0x130; i += 0x8)
        {
            void* uclass = Helper::Casting::offset_deref<void*>(prop_velocity, i);
            if (uclass == fvector)
            {
                add_static_offset(MemberOffsets::StructProperty_Struct, i, STR("StructProperty_Struct"));

                // TODO: Move ArrayProperty_Inner to its own function
                add_static_offset(MemberOffsets::ArrayProperty_Inner, i, STR("ArrayProperty_Inner"));
                success = true;
                break;
            }
        }

        if (!success)
        {
            throw_missed_offset(
                    {
                            MemberOffsets::StructProperty_Struct,
                            MemberOffsets::ArrayProperty_Inner,
                    },
                    STR("Was unable to find StructProperty.Struct")
            );
        }
    }

    auto StaticOffsetFinder::find_offset_internal() -> void
    {
        // The following functions should be called before this function is called
        // find_child_properties
        // find_ffield_next
        // find_class_private
        // find_super_struct

        if (retrieve_static_offset(MemberOffsets::XProperty_Offset_Internal) != -1) { return; }

        UObject* default_pawn = UObjectGlobals::StaticFindObject(nullptr, nullptr, L"/Script/Engine.Default__DefaultPawn");
        if (!default_pawn)
        {
            throw_missed_offset(
                    MemberOffsets::XProperty_Offset_Internal,
                    STR("Was unable to find DefaultPawn")
            );
        }

        void* default_pawn_mesh_component = get_property_from_uobject(STR("/Script/Engine.DefaultPawn"), STR("MeshComponent"));
        // If UE 4.25+ or UE 5.0+

        if (!default_pawn_mesh_component)
        {
            throw_missed_offset(
                    MemberOffsets::XProperty_Offset_Internal,
                    STR("Was unable to find DefaultPawn:MeshComponent")
            );
        }

        bool success = false;
        for (int i = 0x0; i < 0x70; i += 0x4)
        {
            int32_t try_offset = Helper::Casting::offset_deref<int32_t>(default_pawn_mesh_component, i);

            // Limiting the range to between 1 and 0x4000 because it cannot be Offset_Internal at that point (one could debate the maximum value though, I chose 4k sort of randomly)
            if (try_offset <= 0 || try_offset >= 0x4000)
                continue;

            UObject* try_object = Helper::Casting::offset_deref<UObject*>(default_pawn, try_offset);
            if (try_object)
            {
                if (UObject::IsReal(try_object))
                {
                    std::wstring obj_name = try_object->GetFullName();
                    if (obj_name == L"StaticMeshComponent /Script/Engine.Default__DefaultPawn:MeshComponent0")
                    {
                        add_static_offset(MemberOffsets::XProperty_Offset_Internal, i, STR("XProperty_Offset_Internal"));
                        success = true;
                        break;
                    }
                }
            }
        }

        if (!success)
        {
            throw_missed_offset(
                    MemberOffsets::XProperty_Offset_Internal,
                    STR("Was unable to find Offset_Internal")
            );
        }
    }

    auto StaticOffsetFinder::find_bool_member_variables() -> void
    {
        if (retrieve_static_offset(MemberOffsets::FieldSize_BoolProperty) != -1 &&
            retrieve_static_offset(MemberOffsets::ByteOffset_BoolProperty) != -1 &&
            retrieve_static_offset(MemberOffsets::ByteMask_BoolProperty) != -1 &&
            retrieve_static_offset(MemberOffsets::FieldMask_BoolProperty) != -1)
        {
            return;
        }


        if (retrieve_static_offset(MemberOffsets::XField_ChildProperties) == -1)
        {
            throw_missed_offset(
                    {
                            MemberOffsets::FieldSize_BoolProperty,
                            MemberOffsets::ByteOffset_BoolProperty,
                            MemberOffsets::ByteMask_BoolProperty,
                            MemberOffsets::FieldMask_BoolProperty,
                    },
                    STR("find_children() must be run before find_bool_member_variables()")
            );
        }

        // Type: UScriptStruct
        UStruct* hit_result = UObjectGlobals::StaticFindObject<UStruct*>(nullptr, nullptr, L"/Script/Engine.HitResult");

        FName prop_name_to_test_against = FName(L"bBlockingHit");
        FProperty* prop_to_test{};
        hit_result->ForEachProperty([&](FProperty* child) {
            if (child->GetFName() == prop_name_to_test_against)
            {
                prop_to_test = child;
                return LoopAction::Break;
            }

            return LoopAction::Continue;
        });

        if (!prop_to_test)
        {
            throw_missed_offset(
                    {
                            MemberOffsets::FieldSize_BoolProperty,
                            MemberOffsets::ByteOffset_BoolProperty,
                            MemberOffsets::ByteMask_BoolProperty,
                            MemberOffsets::FieldMask_BoolProperty,
                    },
                    STR("Was unable to find 'HitResult:bBlockingHit'")
            );
        }

        constexpr uint32_t value_to_compare_to = 0x01010001;

        // Starting at 0x40 because it's exceedingly unlikely that BoolProperty member variables are this low in the structure
        // Because the structure inheritance is UObject->UField->UProperty->BoolProperty
        // BoolProperty starts at 0x70 in all UE4 versions that I've checked (4.17 to 4.26)
        bool success{};
        for (int32_t offset = 0x40; offset < 0x120; offset += 0x8)
        {
            uint32_t value_to_compare = Helper::Casting::offset_deref<uint32_t>(prop_to_test, offset);

            if (value_to_compare == value_to_compare_to)
            {
                constexpr uint8_t field_size_offset = 0x0;
                constexpr uint8_t byte_offset_offset = 0x1;
                constexpr uint8_t byte_mask_offset = 0x2;
                constexpr uint8_t field_mask_offset = 0x3;

                if (retrieve_static_offset(MemberOffsets::FieldSize_BoolProperty) == -1)
                {
                    add_static_offset(MemberOffsets::FieldSize_BoolProperty, offset + field_size_offset, STR("BoolProperty_FieldSize"));
                }
                if (retrieve_static_offset(MemberOffsets::ByteOffset_BoolProperty) == -1)
                {
                    add_static_offset(MemberOffsets::ByteOffset_BoolProperty, offset + byte_offset_offset, STR("BoolProperty_ByteOffset"));
                }
                if (retrieve_static_offset(MemberOffsets::ByteMask_BoolProperty) == -1)
                {
                    add_static_offset(MemberOffsets::ByteMask_BoolProperty, offset + byte_mask_offset, STR("BoolProperty_ByteMask"));
                }
                if (retrieve_static_offset(MemberOffsets::FieldMask_BoolProperty) == -1)
                {
                    add_static_offset(MemberOffsets::FieldMask_BoolProperty, offset + field_mask_offset, STR("BoolProperty_FieldMask"));
                }

                success = true;
                break;
            }
        }

        if (!success)
        {
            throw_missed_offset(
                    {
                            MemberOffsets::FieldSize_BoolProperty,
                            MemberOffsets::ByteOffset_BoolProperty,
                            MemberOffsets::ByteMask_BoolProperty,
                            MemberOffsets::FieldMask_BoolProperty,
                    },
                    STR("Was unable to find member variables of BoolProperty")
            );
        }
    }

    auto StaticOffsetFinder::find_class_flags() -> void
    {
        if (retrieve_static_offset(MemberOffsets::UClass_ClassFlags) != -1) { return; }

        int32_t super_struct_offset = retrieve_static_offset(MemberOffsets::UStruct_SuperStruct);
        if (super_struct_offset == -1)
        {
            throw_missed_offset(
                    MemberOffsets::UClass_ClassFlags,
                    STR("find_super_struct() must be run before find_class_flags()")
            );
        }

        UClass* known_class_within = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, STR("/Script/CoreUObject.Object"));
        if (!known_class_within)
        {
            throw_missed_offset(
                    MemberOffsets::UClass_ClassFlags,
                    STR("Was unable to find /Script/CoreUObject.Object")
            );
        }

        UClass* engine_class = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, STR("/Script/Engine.Engine"));
        if (!engine_class)
        {
            throw_missed_offset(
                    MemberOffsets::UClass_ClassFlags,
                    STR("Was unable to find /Script/Engine.Engine")
            );
        }

        bool success{};
        for (int32_t i = super_struct_offset + 0x8; i < 0x160; i += 0x8)
        {
            UClass* class_within_to_test = Helper::Casting::offset_deref<UClass*>(engine_class, i);
            if (class_within_to_test == known_class_within)
            {
                add_static_offset(MemberOffsets::UClass_ClassWithin, i, STR("UClass_ClassWithin"));
                add_static_offset(MemberOffsets::UClass_ClassFlags, i - 0xC, STR("UClass_ClassFlags"));
                success = true;
                break;
            }
        }

        if (!success)
        {
            throw_missed_offset(
                    MemberOffsets::UClass_ClassFlags,
                    STR("Was unable to find UClass.ClassFlags")
            );
        }
    }

    auto StaticOffsetFinder::find_class_default_object() -> void
    {
        if (retrieve_static_offset(MemberOffsets::UClass_ClassDefaultObject) != -1) { return; }

        if (retrieve_static_offset(MemberOffsets::UObject_ClassPrivate) == -1)
        {
            throw_missed_offset(
                    MemberOffsets::UClass_ClassDefaultObject,
                    STR("find_class_private() must be run before find_class_default_object()")
            );
        }

        constexpr wchar_t classDefaultObjectName[] = L"/Script/Engine.Default__MaterialExpression";

        UObject* cdo = UObjectGlobals::StaticFindObject(nullptr, nullptr, classDefaultObjectName);
        if (!cdo)
        {
            throw_missed_offset(
                    MemberOffsets::UClass_ClassDefaultObject,
                    STR("Was unable to find /Script/Engine.Default__MaterialExpression")
            );
        }

        void* class_to_test = cdo->GetClass();

        bool success{};
        for (int32_t iteratorOffset = 0x50; iteratorOffset < 0x160; iteratorOffset += 0x8)
        {
            void* class_default_object_to_test = Helper::Casting::offset_deref<void*>(class_to_test, iteratorOffset);

            if (class_default_object_to_test == cdo)
            {
                add_static_offset(MemberOffsets::UClass_ClassDefaultObject, iteratorOffset, STR("UClass_ClassDefaultObject"));
                UClass::Offset::class_config_name = iteratorOffset - 0x30;

#ifdef WITH_CASE_PRESERVING_NAME
                // Compensate for the extra size of FName if it's case preserving
                // This includes padding
                // TODO: This needs to be redone properly, either statically for every supported engine version or by using StaticOffsetFinder to properly find these offsets
                // TODO: This for sure needs to be done properly, the current setup is very temporary

                UClass::Offset::class_config_name -= 0x8;
#endif

                success = true;
                break;
            }
        }

        if (!success)
        {
            throw_missed_offset(
                    MemberOffsets::UClass_ClassDefaultObject,
                    STR("Was unable to find UClass.ClassDefaultObject")
            );
        }
    }

    auto StaticOffsetFinder::find_property_flags() -> void
    {
        if (retrieve_static_offset(MemberOffsets::Property_PropertyFlags) != -1) { return; }

        if (retrieve_static_offset(MemberOffsets::XField_ChildProperties) == -1)
        {
            throw_missed_offset(
                    MemberOffsets::Property_PropertyFlags,
                    STR("find_children() must be run before find_property_flags()")
            );
        }

        void* prop_velocity = get_property_from_uobject(L"/Script/Engine.MovementComponent", L"Velocity");
        if (!prop_velocity)
        {
            throw_missed_offset(
                    MemberOffsets::Property_PropertyFlags,
                    STR("Was unable to find 'StructProperty /Script/Engine.MovementComponent:Velocity'")
            );
        }

        // TODO: Improved this because right now this is a very bad method because of the many versions checks required
        uint64_t flags_to_check_for{};

        if (Version::Major == 4)
        {
            if (Version::Minor <= 12)
            {
                // Flags
                // CPF_Edit | CPF_BlueprintVisible | CPF_ZeroConstructor | CPF_IsPlainOldData | CPF_NoDestructor | CPF_NativeAccessSpecifierPublic
                flags_to_check_for = 0x0010001040000205;
            }
            else if (Version::Minor <= 18)
            {
                // Flags
                // CPF_Edit | CPF_BlueprintVisible | CPF_HasGetValueTypeHash | CPF_IsPlainOldData | CPF_NoDestructor | CPF_NativeAccessSpecifierPublic
                flags_to_check_for = 0x0018001040000005;
            }
            else
            {
                // Flags
                // CPF_Edit | CPF_BlueprintVisible | CPF_ZeroConstructor | CPF_IsPlainOldData | CPF_NoDestructor | CPF_HasGetValueTypeHash | CPF_NativeAccessSpecifierPublic
                flags_to_check_for = 0x0018001040000205;
            }
        }
        else if (Version::Major < 4)
        {
            throw_missed_offset(
                    MemberOffsets::Property_PropertyFlags,
                    STR("Unreal Engine version must be 4 or higher")
            );
        }
        else
        {
            // Flags
            // CPF_Edit | CPF_BlueprintVisible | CPF_ZeroConstructor | CPF_IsPlainOldData | CPF_NoDestructor | CPF_HasGetValueTypeHash | CPF_NativeAccessSpecifierPublic
            flags_to_check_for = 0x0018001040000205;
        }

        bool success{};
        for (int32_t offset = 0; offset < 0x80; offset += 0x8)
        {
            uint64_t flags_to_check = Helper::Casting::offset_deref<uint64_t>(prop_velocity, offset);

            if (flags_to_check == flags_to_check_for)
            {
                add_static_offset(MemberOffsets::Property_PropertyFlags, offset, STR("Property_PropertyFlags"));
                success = true;
                break;
            }
        }

        if (!success)
        {
            throw_missed_offset(
                    MemberOffsets::Property_PropertyFlags,
                    STR("Was unable to find PropertyFlags")
            );
        }
    }

    auto StaticOffsetFinder::find_ustruct_properties_size_and_min_alignment() -> void
    {
        if (retrieve_static_offset(MemberOffsets::UStruct_PropertiesSize) != -1 &&
            retrieve_static_offset(MemberOffsets::UStruct_MinAlignment) != -1)
        {
            return;
        }

        constexpr uint32_t num_required_matches = 3;
        struct ObjectInfo
        {
            const wchar_t* object_name{};
            uint64_t expected_value{};
        };
        std::vector<ObjectInfo> objects_to_test;

        struct Match
        {
            int32_t offset{};
        };
        std::vector<Match> matches{};

        objects_to_test.emplace_back(
                ObjectInfo{
                        L"/Script/Engine.ActorComponent:Activate",
                        0x0000000100000001,
                }
        );

        objects_to_test.emplace_back(
                ObjectInfo{
                        L"/Script/CoreUObject.OrientedBox",
                        0x000000040000003C,
                }
        );

        objects_to_test.emplace_back(
                ObjectInfo{
                        L"/Script/CoreUObject.Object:ExecuteUbergraph",
                        0x0000000100000004,
                }
        );

        objects_to_test.emplace_back(
                ObjectInfo{
                        L"/Script/Engine.MovementComponent:ConstrainDirectionToPlane",
                        0x0000000100000018,
                }
        );

        objects_to_test.emplace_back(
                ObjectInfo{
                        L"/Script/Engine.Pawn:AddControllerPitchInput",
                        0x0000000100000004,
                }
        );

        for (auto& object_info : objects_to_test)
        {
            void* object = UObjectGlobals::StaticFindObject(nullptr, nullptr, object_info.object_name);
            if (!object) { continue; }

            for (int32_t i = 0x0; i < 0x100; i += 0x8)
            {
                uint64_t value = Helper::Casting::offset_deref<uint64_t>(object, i);

                if (value == object_info.expected_value)
                {
                    matches.emplace_back(i);
                }
            }
        }

        if (matches.empty() || matches.size() < num_required_matches)
        {
            throw_missed_offset(
                    {
                            MemberOffsets::UStruct_PropertiesSize,
                            MemberOffsets::UStruct_MinAlignment,
                    },
                    STR("Not enough matches")
            );
        }

        if (!std::equal(matches.begin(), matches.end(), matches.begin(), [](auto& a, auto& b) { return a.offset == b.offset; }))
        {
            throw_missed_offset(
                    {
                            MemberOffsets::UStruct_PropertiesSize,
                            MemberOffsets::UStruct_MinAlignment,
                    },
                    STR("Was unable to determine the offset of UStruct.PropertiesSize & UStruct.MinAlignment")
            );
        }

        if (retrieve_static_offset(MemberOffsets::UStruct_PropertiesSize) == -1)
        {
            add_static_offset(MemberOffsets::UStruct_PropertiesSize, matches[0].offset, STR("UStruct_PropertiesSize"));
        }
        if (retrieve_static_offset(MemberOffsets::UStruct_MinAlignment) == -1)
        {
            add_static_offset(MemberOffsets::UStruct_MinAlignment, matches[0].offset + 0x4, STR("UStruct_MinAlignment"));
        }
    }

    auto StaticOffsetFinder::find_num_parms_and_return_value_offset() -> void
    {
        if (retrieve_static_offset(MemberOffsets::UFunction_NumParms) != -1 &&
            retrieve_static_offset(MemberOffsets::UFunction_ReturnValueOffset) != -1 &&
            retrieve_static_offset(MemberOffsets::UFunction_FunctionFlags) != -1)
        {
            return;
        }
        // The following UFunctions and their specs
        /*
            /Script/Engine.Pawn:AddMovementInput {
                Params: 3
                Return: None
            }
            /Script/Engine.HUD:AddDebugText {
                Params: 12
                Return: None
            }
            /Script/Engine.HUD:AddHitBox {
                Params: 5
                Return: None
            }
            /Script/Engine.HUD:DrawLine {
                Params: 6
                Return: None
            }
        */

        constexpr uint32_t required_matches{5};

        struct FunctionToTest
        {
            const wchar_t* name{};
            const uint8_t num_parms{};
            const uint16_t return_value_offset{};
        };
        std::vector<FunctionToTest> functions_to_test;
        functions_to_test.emplace_back(FunctionToTest{L"/Script/Engine.Pawn:AddControllerPitchInput", 1, 0xFFFF});
        functions_to_test.emplace_back(FunctionToTest{L"/Script/Engine.Pawn:AddMovementInput", 3, 0xFFFF});
        functions_to_test.emplace_back(FunctionToTest{L"/Script/Engine.HUD:AddDebugText", 12, 0xFFFF});
        functions_to_test.emplace_back(FunctionToTest{L"/Script/Engine.HUD:AddHitBox", 5, 0xFFFF});
        functions_to_test.emplace_back(FunctionToTest{L"/Script/Engine.HUD:DrawLine", 6, 0xFFFF});

        uint32_t num_matches{};
        int32_t num_parms_offset{};
        int32_t return_value_offset_offset{};
        int32_t function_flags_offset{};

        for (auto& function_to_test : functions_to_test)
        {
            UFunction* func = UObjectGlobals::StaticFindObject<UFunction*>(nullptr, nullptr, function_to_test.name);

            // All functions must exist or increase the list of functions and require a minimum amount to exist & pass all tests
            // For now I will require all functions to exist
            if (!func)
            {
                throw_missed_offset(
                        {
                                MemberOffsets::UFunction_NumParms,
                                MemberOffsets::UFunction_ReturnValueOffset,
                                MemberOffsets::UFunction_FunctionFlags,
                        },
                        STR("All functions to test with must exist")
                );
            }

            // Loop through uint16_t from 0x0 to 0x130
            for (int32_t i = 0x0; i < 0x130; i += sizeof(uint16_t))
            {
                // Deref the address to get the value
                uint16_t return_value_offset = Helper::Casting::offset_deref<uint16_t>(func, i);

                // If the dereferenced value is what we expect it to be then proceed
                if (return_value_offset == function_to_test.return_value_offset)
                {
                    // Deref the address - 4 bytes to get what should be NumParms
                    uint8_t num_parms = Helper::Casting::offset_deref<uint8_t>(func, i - 0x4);

                    // If the dereferenced value is what we expect it to be then we have a match
                    if (num_parms == function_to_test.num_parms)
                    {
                        ++num_matches;

                        num_parms_offset = i - 0x4;
                        return_value_offset_offset = i;
                        // FunctionFlags offset can be wrong and is in 4.12
                        // I think it has to do with the struct being different and as a result the alignment is different
                        // Luckily FunctionFlags is not being used anywhere in this code-base so it might not be important to keep this offset
                        function_flags_offset = i - 0x8;
                    }
                }
            }
        }

        if (num_matches < required_matches)
        {
            throw_missed_offset(
                    {
                            MemberOffsets::UFunction_NumParms,
                            MemberOffsets::UFunction_ReturnValueOffset,
                            MemberOffsets::UFunction_FunctionFlags,
                    },
                    STR("Was unable to find NumParms, ReturnValueOffset & FunctionFlags (insufficient matches)")
            );
        }
        else
        {
            if (retrieve_static_offset(MemberOffsets::UFunction_NumParms) == -1)
            {
                add_static_offset(MemberOffsets::UFunction_NumParms, num_parms_offset, STR("UFunction_NumParms"));
            }
            if (retrieve_static_offset(MemberOffsets::UFunction_ReturnValueOffset) == -1)
            {
                add_static_offset(MemberOffsets::UFunction_ReturnValueOffset, return_value_offset_offset, STR("UFunction_ReturnValueOffset"));
            }
            if (retrieve_static_offset(MemberOffsets::UFunction_FunctionFlags) == -1)
            {
                add_static_offset(MemberOffsets::UFunction_FunctionFlags, function_flags_offset, STR("UFunction_FunctionFlags"));
            }
        }
    }

    auto StaticOffsetFinder::find_function_func_offset() -> void
    {
        if (retrieve_static_offset(MemberOffsets::UFunction_Func) != -1) { return; }

        int32_t rvo_offset = retrieve_static_offset(MemberOffsets::UFunction_ReturnValueOffset);
        if (rvo_offset == -1)
        {
            throw_missed_offset(
                    MemberOffsets::UFunction_Func,
                    STR("UFunction_ReturnValueOffset offset was zero. Please execute find_num_parms_and_return_value_offset()")
            );
        }

        struct TestResult
        {
            unsigned char* address;
            long long offset;
            bool eventgraph_fastcalls_defined;
        };
        std::vector<TestResult> test_results;

        auto do_function_test = [&](UObject* test_function_typed) {
            unsigned char* test_function = static_cast<unsigned char*>(static_cast<void*>(test_function_typed));
            unsigned char* rvo_ptr = test_function + rvo_offset;

            // Make sure 'rvo_ptr' is aligned by 0x8, if it's not, align it
            constexpr int32_t align_by = 0x8;

            auto offset_to_previous_aligned_address = reinterpret_cast<uintptr_t>(rvo_ptr) % align_by;
            if (offset_to_previous_aligned_address > align_by)
            {
                throw_missed_offset(
                        MemberOffsets::UFunction_Func,
                        std::format(STR("offset_to_previous_aligned_address was less than '{}', could not subtract"), align_by)
                );
            }

            auto offset_to_next_aligned_address = align_by - offset_to_previous_aligned_address;
            unsigned char* next_ptr = rvo_ptr + offset_to_next_aligned_address;

            // Skipping the range that contains 'RPCid' & 'RPCResponseId', going straight to where 'FirstPropertyToInit' is stored
            unsigned char* first_property_to_init = next_ptr + 0x10;

            // Skipping another 0x8 bytes, this is either 'EventGraphFunction' or 'Func'
            // This is 'EventGraphFunction' if 'UE_BLUEPRINT_EVENTGRAPH_FASTCALLS' is defined
            // or 'Func' if 'UE_BLUEPRINT_EVENTGRAPH_FASTCALLS' is not defined
            unsigned char* event_graph_function_or_func = first_property_to_init + 0x8;

            if (!event_graph_function_or_func)
            {
                throw_missed_offset(
                        MemberOffsets::UFunction_Func,
                        STR("Pointer to 'event_graph_function_or_func' was nullptr")
                );
            }

            // If pointer is not valid, then this cannot possibly be the 'Func' pointer because 'Func' cannot be nullptr as far as I know
            // If pointer is valid, then this could be 'Func' but we cannot confirm it yet because 'EventGraphFunction' is also a ptr type that may be non-nullptr
            bool was_event_graph_function_or_func_valid_ptr;
            unsigned char* even_graph_function_or_func_dbl_ptr = static_cast<unsigned char*>(*static_cast<void**>(static_cast<void*>(event_graph_function_or_func)));
            if (Helper::Casting::offset_deref_safe<unsigned char*>(even_graph_function_or_func_dbl_ptr, 0) != 0)
            {
                was_event_graph_function_or_func_valid_ptr = true;
            }
            else
            {
                was_event_graph_function_or_func_valid_ptr = false;
            }

            // Add 0x8 bytes
            // If 'UE_BLUEPRINT_EVENTGRAPH_FASTCALLS' is defined then this gets us to 'EventGraphCallOffset'
            // Otherwise we're out of bounds
            unsigned char* event_graph_call_offset_or_oob = event_graph_function_or_func + 0x8;

            // Add another 0x4 bytes
            // This puts us at 'Func' if 'UE_BLUEPRINT_EVENTGRAPH_FASTCALLS' is defined
            // Otherwise it puts us further out of bounds
            unsigned char* func_or_oob = event_graph_call_offset_or_oob + 0x4;

            // Check if the address is aligned
            // If it's not then 'UE_BLUEPRINT_EVENTGRAPH_FASTCALLS' is not defined and 'Func' is where 'EventGraphFunction' would be otherwise
            auto offset_to_next_aligned_ptr = reinterpret_cast<uintptr_t>(func_or_oob) % align_by;
            if (offset_to_next_aligned_ptr != 0)
            {
                // The next aligned pointer is 0 bytes away, so this address is aligned
                test_results.emplace_back(TestResult{
                    .address = event_graph_function_or_func,
                    .offset = event_graph_function_or_func - test_function,
                    .eventgraph_fastcalls_defined = false,
                });
            }

            // If pointer is not valid, then this cannot possibly be the 'Func' pointer, see above for the reason why
            // If pointer is valid, then this could be the 'Func' pointer
            // Checking both the pointer and the pointer that pointer is pointing to, because we might be out of bounds of the struct here
            unsigned char* func_or_oob_dbl_ptr = static_cast<unsigned char*>(*static_cast<void**>(static_cast<void*>(func_or_oob)));
            if (Helper::Casting::offset_deref_safe<unsigned char*>(func_or_oob, 0) != 0 &&
                Helper::Casting::offset_deref_safe<unsigned char*>(func_or_oob_dbl_ptr, 0) != 0)
            {
                // Pointer is valid

                if (!was_event_graph_function_or_func_valid_ptr)
                {
                    // If the location where 'EventGraphFunction' should be, did not contain a valid pointer,
                    // then we know that this must be 'Func' because there are no other locations where it could be
                    test_results.emplace_back(TestResult{
                            .address = func_or_oob,
                            .offset = func_or_oob - test_function,
                            .eventgraph_fastcalls_defined = true,
                    });
                }
                else
                {
                    // Both 'Func'/'OOB' & 'EventGraphFunctionOrFunc' pointers are valid
                    // That means we've gotten no closer to finding 'Func'

                    // UE_BLUEPRINT_EVENTGRAPH_FASTCALLS may or may not be defined
                }
            }
            else
            {
                // Pointer is not valid

                if (!was_event_graph_function_or_func_valid_ptr)
                {
                    // The only other location that 'Func' could be stored at is where 'EventGraphFunction' would be if 'UE_BLUEPRINT_EVENTGRAPH_FASTCALLS' is defined
                    // If this location doesn't contain a valid pointer then we've got big problems, and we need to stop the program and output an error
                    throw_missed_offset(
                            MemberOffsets::UFunction_Func,
                            std::format(STR("Both 'EventGraphFunction'/'Func' and 'Func'/'OOB' contained invalid pointers. test_function: {}, was_event_graph_function_or_func_valid_ptr: {}"), (void*)test_function, (void*)was_event_graph_function_or_func_valid_ptr)
                    );
                }
                else
                {
                    // The only other location is valid so that must be 'Func'
                    test_results.emplace_back(TestResult{
                            .address = event_graph_function_or_func,
                            .offset = event_graph_function_or_func - test_function,
                            .eventgraph_fastcalls_defined = false,
                    });
                }
            }
        };

        constexpr size_t num_functions_to_test = 30;
        std::vector<UObject*> functions_to_test{};
        functions_to_test.reserve(num_functions_to_test);

        // Find a number of non-default UFunctions
        UObjectGlobals::FindObjects(num_functions_to_test, FName(STR("Function")), FName(0u, 0u), functions_to_test, RF_NoFlags, RF_DefaultSubObject | RF_ArchetypeObject);

        // Parse memory
        for (const auto& function_to_test : functions_to_test)
        {
            do_function_test(function_to_test);
        }

        // If we have no test results then we've failed to find 'Func'
        if (test_results.empty())
        {
            throw_missed_offset(
                    MemberOffsets::UFunction_Func,
                    STR("There were no test results")
            );
        }

        // Constructing 'final_result' with recognizable values to make it easier to debug if something goes wrong
        TestResult final_result{nullptr, -1, false};

        // Parse results
        for (size_t i = 0; const auto& test_result : test_results)
        {
            if (i == 0)
            {
                // Set the initial results so that we can compare & make sure that all the results are identical
                final_result.address = test_result.address;
                final_result.offset = test_result.offset;
                final_result.eventgraph_fastcalls_defined = test_result.eventgraph_fastcalls_defined;

                ++i;
                continue;
            }

            if (final_result.offset != test_result.offset || final_result.eventgraph_fastcalls_defined != test_result.eventgraph_fastcalls_defined)
            {
                throw_missed_offset(
                        MemberOffsets::UFunction_Func,
                        STR("Test results were inconsistent")
                );
            }

            ++i;
        }

        // If we made it this far then we've successfully found 'UFunction.Func'
        // This because it will throw if there's an error or if the test results are not consistent
        add_static_offset(MemberOffsets::UFunction_Func, Helper::Integer::to<int32_t>(final_result.offset), L"UFunction_Func");
        UE_BLUEPRINT_EVENTGRAPH_FASTCALLS = final_result.eventgraph_fastcalls_defined;
    }

    auto StaticOffsetFinder::find_property_class_and_meta_class() -> void
    {
        if (retrieve_static_offset(MemberOffsets::ObjectProperty_PropertyClass) != -1 &&
            retrieve_static_offset(MemberOffsets::ClassProperty_MetaClass) != -1)
        {
            return;
        }

        // PropertyClass: Class /Script/CoreUObject.Class
        // MetaClass: Class /Script/Engine.PlayerCameraManager
        void* controller_player_camera_manager_class = get_property_from_uobject(L"/Script/Engine.PlayerController", L"PlayerCameraManagerClass");
        void* core_class = UObjectGlobals::StaticFindObject(nullptr, nullptr, L"/Script/CoreUObject.Class");
        void* player_camera_manager_class = UObjectGlobals::StaticFindObject(nullptr, nullptr, L"/Script/Engine.PlayerCameraManager");

        if (!controller_player_camera_manager_class || !core_class || !player_camera_manager_class)
        {
            throw_missed_offset(
                    {
                            MemberOffsets::ObjectProperty_PropertyClass,
                            MemberOffsets::ClassProperty_MetaClass,
                    },
                    STR("Was unable to find the required classes")
            );
        }

        bool success{};
        bool property_class_found{};
        bool meta_class_found{};

        for (int32_t i = 0x40; i < 0x120; i += 0x8)
        {
            int32_t next_ptr_offset = i + 8;

            void* property_class = Helper::Casting::offset_deref<void*>(controller_player_camera_manager_class, i);
            void* meta_class = Helper::Casting::offset_deref<void*>(controller_player_camera_manager_class, next_ptr_offset);

            if (property_class == core_class && !property_class_found)
            {
                if (retrieve_static_offset(MemberOffsets::ObjectProperty_PropertyClass) == -1)
                {
                    add_static_offset(MemberOffsets::ObjectProperty_PropertyClass, i, L"ObjectProperty_PropertyClass");
                }

                property_class_found = true;
            }

            if (meta_class == player_camera_manager_class && !meta_class_found)
            {
                if (retrieve_static_offset(MemberOffsets::ClassProperty_MetaClass) == -1)
                {
                    add_static_offset(MemberOffsets::ClassProperty_MetaClass, next_ptr_offset, L"ClassProperty_MetaClass");
                }

                meta_class_found = true;
            }

            if (property_class_found && meta_class_found)
            {
                success = true;
                break;
            }
        }

        if (!success)
        {
            throw_missed_offset(
                    {
                            MemberOffsets::ObjectProperty_PropertyClass,
                            MemberOffsets::ClassProperty_MetaClass,
                    },
                    STR("Was unable to find PropertyClass & MetaClass")
            );
        }
    }

    auto StaticOffsetFinder::find_softclassproperty_meta_class() -> void
    {
        if (retrieve_static_offset(MemberOffsets::SoftClassProperty_MetaClass) != -1)
        {
            return;
        }

        if (retrieve_static_offset(MemberOffsets::XField_ChildProperties) == -1)
        {
            throw std::runtime_error{
                    "[StaticOffsetFinder->find_softclassproperty_meta_class] ChildProperties offset was zero. Please execute find_child_properties()."};
        }

        FProperty* softclassproperty = static_cast<FProperty*>(get_property_from_uobject(L"/Script/Engine.PrimaryAssetTypeInfo", L"AssetBaseClass"));
        UClass* meta_class = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Object");

        if (!softclassproperty || !meta_class)
        {
            throw std::runtime_error{"Was unable to find SoftClassProperty or it's meta class"};
        }

        bool success{};
        for (int32_t i = 0x40; i < 0x120; i += 0x8)
        {
            void* maybe_meta_class = Helper::Casting::offset_deref<void*>(softclassproperty, i);
            if (maybe_meta_class == meta_class)
            {
                add_static_offset(MemberOffsets::SoftClassProperty_MetaClass, i, STR("SoftClassProperty_MetaClass"));
                success = true;
                break;
            }
        }

        if (!success)
        {
            throw std::runtime_error{"Was unable to find offset for SoftClassProperty.MetaClass"};
        }
    }

    auto StaticOffsetFinder::find_enum_members() -> void
    {
        if (retrieve_static_offset(MemberOffsets::EnumProperty_UnderlyingType) != -1 &&
            retrieve_static_offset(MemberOffsets::EnumProperty_Enum) != -1 &&
            retrieve_static_offset(MemberOffsets::UEnum_Names) != -1)
        {
            return;
        }

        int32_t offset_underlying_type{};
        int32_t offset_enum{};
        int32_t offset_names{};

        // ChildProperties offset is required so fail if it doesn't exist
        if (retrieve_static_offset(MemberOffsets::XField_ChildProperties) == -1)
        {
            throw_missed_offset(
                    {
                            MemberOffsets::EnumProperty_UnderlyingType,
                            MemberOffsets::EnumProperty_Enum,
                            MemberOffsets::UEnum_Names
                    },
                    STR("ChildProperties offset was zero. Please execute find_child_properties()")
            );
        }

        constexpr uint32_t num_required_matches{1};

        struct ObjectToTest
        {
            // For <4.25
            const wchar_t* object_name{};
            const wchar_t* object_underlying_type{};
            const wchar_t* object_enum{};

            // For 4.25+
            const wchar_t* object_owner{};
            const wchar_t* prop_short_name{};

            // For all versions
            const wchar_t* enum_cpp_type{};
        };

        // Container to store data for multiple objects
        // This way we can scan many objects to verify that the offsets are correct
        std::vector<ObjectToTest> objects_to_test;

        objects_to_test.emplace_back(ObjectToTest{
                L"/Script/CoreUObject.AutomationEvent:Type",
                L"/Script/CoreUObject.AutomationEvent:Type.UnderlyingType",
                L"/Script/CoreUObject.EAutomationEventType",

                L"/Script/CoreUObject.AutomationEvent",
                L"Type",

                L"EAutomationEventType",
        });

        objects_to_test.emplace_back(ObjectToTest{
                L"/Script/CoreUObject.PolyglotTextData:Category",
                L"/Script/CoreUObject.PolyglotTextData:Category.UnderlyingType",
                L"/Script/CoreUObject.ELocalizedTextSourceCategory",

                L"/Script/CoreUObject.PolyglotTextData",
                L"Category",

                L"ELocalizedTextSourceCategory",
        });

        // ByteProperty, used in <4.15 because EnumProperty doesn't exist
        objects_to_test.emplace_back(ObjectToTest{
                L"/Script/CoreUObject.InterpCurvePointFloat:InterpMode",
                L"",
                L"/Script/CoreUObject.EInterpCurveMode",

                L"/Script/CoreUObject.InterpCurvePointFloat",
                L"InterpMode",

                L"EInterpCurveMode",
        });

        uint32_t matches{};
        bool found_underlying_type{};
        bool found_enum{};
        bool found_names{};

        for (auto& object_to_test : objects_to_test)
        {
            FProperty* prop = static_cast<FProperty*>(get_property_from_uobject(object_to_test.object_owner, object_to_test.prop_short_name));
            UObject* known_underlying_type = UObjectGlobals::StaticFindObject(nullptr, nullptr, object_to_test.object_underlying_type);
            UObject* known_enum = UObjectGlobals::StaticFindObject(nullptr, nullptr, object_to_test.object_enum);

            if (Version::IsBelow(4, 25) && !File::StringViewType{object_to_test.object_underlying_type}.empty() && !known_underlying_type)
            {
                Output::send(STR("Known underlying enum type not found: {}\n"), File::StringViewType{object_to_test.object_underlying_type});
                continue;
            }

            if (prop && known_enum)
            {
                // Loop through uint16_t from 0x0 to 0x130
                for (int32_t i = 0x0; i < 0x130; i += sizeof(void*))
                {
                    if (!found_enum)
                    {
                        UObject* obj_enum = Helper::Casting::offset_deref<UObject*>(prop, i);

                        if (obj_enum == known_enum)
                        {
                            found_enum = true;
                            offset_enum = i;

                            if (!found_names)
                            {
                                // Find UEnum.Names finding UEnum.CppType
                                // Names is 0x10 bytes after CppType
                                for (int32_t i2 = 0x0; i2 < 0x80; i2 += sizeof(void*))
                                {
                                    // Safely deref to get the data at this address
                                    wchar_t* cpp_type = Helper::Casting::offset_deref_safe<wchar_t*>(obj_enum, i2);

                                    // Safely deref again to confirm that the data is a valid pointer
                                    void* is_valid_ptr = Helper::Casting::offset_deref_safe<wchar_t*>(cpp_type, 0);

                                    // In the wcscmp call, keep the known string as param 2
                                    // This is because wcscmp only checks param 2 for a null terminator
                                    // It's entirely possible that the 'cppType' variable is not a valid string
                                    // By passing the known (and valid) string as param 2:
                                    // We can make sure it doesn't go beyond the max-length if something goes wrong with the unknown 'cppType' variable
                                    if (is_valid_ptr && wcscmp(cpp_type, object_to_test.enum_cpp_type) == 0)
                                    {
                                        found_names = true;
                                        offset_names = i2 + 0x10;
                                    }
                                }
                            }
                        }
                    }

                    // If <4.25, then we can use 'StaticFindObject' to find the underlying type
                    // We can then to a pointer comparison to find the offset of 'UEnumProperty::UnderlyingProp'
                    // If >=4.25, we cannot use 'StaticFindObject' because the underlying property wont' exist in GUObjectArray
                    // Therefore we fall back on using a relative offset to 'FEnumProperty::Enum'
                    if (Version::IsBelow(4, 25))
                    {
                        if (!found_underlying_type)
                        {
                            UObject* obj_underlying_type = Helper::Casting::offset_deref<UObject*>(prop, i);
                            if (obj_underlying_type == known_underlying_type)
                            {
                                found_underlying_type = true;
                                offset_underlying_type = i;
                            }
                        }
                    }
                    else if (found_enum)
                    {
                        found_underlying_type = true;
                        offset_underlying_type = offset_enum - sizeof(void*);
                    }

                    // If both underlying type and enum was found then we have a match
                    if (found_underlying_type && found_enum && found_names)
                    {
                        ++matches;
                        break;
                    }
                }
            }
        }

        if (matches < num_required_matches)
        {
            throw_missed_offset(
                    {
                            MemberOffsets::EnumProperty_UnderlyingType,
                            MemberOffsets::EnumProperty_Enum,
                            MemberOffsets::UEnum_Names
                    },
                    STR("Was unable to find EnumProperty_UnderlyingType or EnumProperty_Enum")
            );
        }
        else
        {
            if (retrieve_static_offset(MemberOffsets::EnumProperty_UnderlyingType) == -1)
            {
                add_static_offset(MemberOffsets::EnumProperty_UnderlyingType, offset_underlying_type, L"EnumProperty_UnderlyingType");
            }
            if (retrieve_static_offset(MemberOffsets::EnumProperty_Enum) == -1)
            {
                add_static_offset(MemberOffsets::EnumProperty_Enum, offset_enum, L"EnumProperty_Enum");
            }
            if (retrieve_static_offset(MemberOffsets::UEnum_Names) == -1)
            {
                add_static_offset(MemberOffsets::UEnum_Names, offset_names, L"UEnum_Names");
            }
        }
    }

    auto StaticOffsetFinder::find_byteproperty_enum() -> void
    {
        if (retrieve_static_offset(MemberOffsets::ByteProperty_Enum) != -1)
        {
            return;
        }

        int32_t offset_enum{};

        if (retrieve_static_offset(MemberOffsets::XField_ChildProperties) == -1)
        {
            throw std::runtime_error{
                    "[StaticOffsetFinder->find_byteproperty_enum] ChildProperties offset was zero. Please execute find_child_properties()."};
        }

        constexpr uint32_t num_required_matches{1};

        struct ObjectToTest
        {
            const wchar_t* object_name{};
            const wchar_t* object_enum{};
            const wchar_t* prop_short_name{};
        };

        std::vector<ObjectToTest> objects_to_test;

        objects_to_test.emplace_back(ObjectToTest{
                .object_name = L"/Script/Engine.Actor",
                .object_enum = L"/Script/Engine.ENetRole",
                .prop_short_name = L"Role",
        });

        uint32_t matches{};

        for (const auto& object_to_test : objects_to_test)
        {
            UObject* known_enum = UObjectGlobals::StaticFindObject(nullptr, nullptr, object_to_test.object_enum);
            FProperty* prop = static_cast<FProperty*>(get_property_from_uobject(object_to_test.object_name, object_to_test.prop_short_name));

            for (int32_t i = 0x0; i < 0x130; i += sizeof(void*))
            {
                if (prop && known_enum)
                {
                    UObject* byteproperty_enum = Helper::Casting::offset_deref<UObject*>(prop, i);

                    if (byteproperty_enum == known_enum)
                    {
                        offset_enum = i;
                        ++matches;
                    }
                }
            }
        }

        if (matches < num_required_matches)
        {
            throw std::runtime_error{"[C++][StaticOffsetFinder->find_byteproperty_enum] Was unable to find ByteProperty_Enum"};
        }
        else
        {
            add_static_offset(MemberOffsets::ByteProperty_Enum, offset_enum, L"ByteProperty_Enum");
        }
    }

    auto StaticOffsetFinder::find_mapproperty_key_value_prop() -> void
    {
        if (retrieve_static_offset(MemberOffsets::MapProperty_KeyProp) != -1 &&
            retrieve_static_offset(MemberOffsets::MapProperty_ValueProp) != -1)
        {
            return;
        }

        FProperty* map_property = static_cast<FProperty*>(get_property_from_uobject(STR("/Script/MovieScene.MovieSceneEditorData"), STR("ExpansionStates")));
        if (!map_property)
        {
            throw_missed_offset({MemberOffsets::MapProperty_KeyProp, MemberOffsets::MapProperty_ValueProp}, STR("Was unable to find the MapProperty 'ExpansionStates'"));
        }

        bool success{};
        int32_t key_prop_offset{-1};
        int32_t value_prop_offset{-1};

        FName key_name = FName(STR("ExpansionStates_Key"));
        for (int32_t i = 0x0; i < 0x120; i += 0x8)
        {
            char* pointer = Helper::Casting::offset_deref<char*>(map_property, i);

            // Check if the value at MapProperty+i is a valid pointer
            void* maybe_pointer = Helper::Casting::offset_deref_safe<void*>(pointer, 0);
            if (maybe_pointer)
            {
                // The value at MapProperty+i is a valid pointer
                FName maybe_fname = Helper::Casting::offset_deref<FName>(pointer, retrieve_static_offset(MemberOffsets::FField_NamePrivate));
                if (maybe_fname.Equals(key_name))
                {
                    key_prop_offset = i;
                    value_prop_offset = i + 0x8;
                    success = true;
                }
            }
        }

        if (!success)
        {
            throw_missed_offset({MemberOffsets::MapProperty_KeyProp, MemberOffsets::MapProperty_ValueProp}, STR("Was unable to find offsets for MapProperty.KeyProp and/or MapProperty.ValueProp"));
        }

        if (retrieve_static_offset(MemberOffsets::MapProperty_KeyProp) == -1)
        {
            add_static_offset(MemberOffsets::MapProperty_KeyProp, key_prop_offset, L"MapProperty_KeyProp");
        }

        if (retrieve_static_offset(MemberOffsets::MapProperty_ValueProp) == -1)
        {
            add_static_offset(MemberOffsets::MapProperty_ValueProp, value_prop_offset, L"MapProperty_ValueProp");
        }
    }

    auto StaticOffsetFinder::find_delegateproperty_signature_function() -> void
    {
        if (retrieve_static_offset(MemberOffsets::DelegateProperty_SignatureFunction) != -1)
        {
            return;
        }

        FProperty* delegate_property = static_cast<FProperty*>(get_property_from_uobject(STR("/Script/UMG.Widget"), STR("VisibilityDelegate")));
        if (!delegate_property)
        {
            throw_missed_offset(MemberOffsets::DelegateProperty_SignatureFunction, STR("Was unable to find a DelegateProperty in UClass 'Widget'"));
        }

        UFunction* delegate_function = UObjectGlobals::StaticFindObject<UFunction*>(nullptr, nullptr, L"/Script/UMG.Widget:GetSlateVisibility__DelegateSignature");
        if (!delegate_function)
        {
            throw_missed_offset(MemberOffsets::DelegateProperty_SignatureFunction, STR("Was unable to find UFunction 'GetSlateVisibility__DelegateSignature'"));
        }

        bool success{};

        for (int32_t i = 0x0; i < 0x120; i += 0x8)
        {
            UFunction* maybe_function_signature = Helper::Casting::offset_deref<UFunction*>(delegate_property, i);

            if (maybe_function_signature == delegate_function)
            {
                add_static_offset(MemberOffsets::DelegateProperty_SignatureFunction, i, STR("DelegateProperty_SignatureFunction"));
                success = true;
                break;
            }
        }

        if (!success)
        {
            throw_missed_offset(MemberOffsets::DelegateProperty_SignatureFunction, STR("Was unable to find offsets for DelegateProperty.FunctionSignature"));
        }
    }

    auto StaticOffsetFinder::find_multicastdelegateproperty_signature_function() -> void
    {
        // TODO: Separate these three offsets into their own functions because they might not always be at the same offset
        if (retrieve_static_offset(MemberOffsets::MulticastDelegateProperty_SignatureFunction) != -1 &&
            retrieve_static_offset(MemberOffsets::MulticastInlineDelegateProperty_SignatureFunction) != -1 &&
            retrieve_static_offset(MemberOffsets::MulticastSparseDelegateProperty_SignatureFunction) != -1)
        {
            return;
        }

        auto throw_custom_missed_offset = [](const File::CharType* message) {
            throw_missed_offset({MemberOffsets::MulticastDelegateProperty_SignatureFunction,
                                        MemberOffsets::MulticastInlineDelegateProperty_SignatureFunction,
                                        MemberOffsets::MulticastSparseDelegateProperty_SignatureFunction}, message);
        };

        FProperty* delegate_property = static_cast<FProperty*>(get_property_from_uobject(STR("/Script/UMG.ComboBoxString"), STR("OnOpening")));
        if (!delegate_property)
        {
            throw_custom_missed_offset(STR("Was unable to find a MulticastDelegateProperty in UClass 'ComboBoxString'"));
        }

        UFunction* delegate_function = UObjectGlobals::StaticFindObject<UFunction*>(nullptr, nullptr, L"/Script/UMG.ComboBoxString:OnOpeningEvent__DelegateSignature");
        if (!delegate_function)
        {
            throw_custom_missed_offset(STR("Was unable to find UFunction 'MovementModeChangedSignature__DelegateSignature'"));
        }

        bool success{};

        for (int32_t i = 0x0; i < 0x120; i += 0x8)
        {
            UFunction* maybe_function_signature = Helper::Casting::offset_deref<UFunction*>(delegate_property, i);

            if (maybe_function_signature == delegate_function)
            {
                if (retrieve_static_offset(MemberOffsets::MulticastDelegateProperty_SignatureFunction) == -1)
                {
                    add_static_offset(MemberOffsets::MulticastDelegateProperty_SignatureFunction, i, STR("MulticastDelegateProperty_SignatureFunction"));
                }

                if (retrieve_static_offset(MemberOffsets::MulticastInlineDelegateProperty_SignatureFunction) == -1)
                {
                    add_static_offset(MemberOffsets::MulticastInlineDelegateProperty_SignatureFunction, i, STR("MulticastInlineDelegateProperty_SignatureFunction"));
                }

                if (retrieve_static_offset(MemberOffsets::MulticastSparseDelegateProperty_SignatureFunction) == -1)
                {
                    add_static_offset(MemberOffsets::MulticastSparseDelegateProperty_SignatureFunction, i, STR("MulticastSparseDelegateProperty_SignatureFunction"));
                }
                success = true;
                break;
            }
        }

        if (!success)
        {
            throw_custom_missed_offset(STR("Was unable to find offsets for DelegateProperty.FunctionSignature"));
        }
    }

    auto StaticOffsetFinder::find_interfaceproperty_interface_class() -> void
    {
        if (retrieve_static_offset(MemberOffsets::InterfaceProperty_InterfaceClass) != -1)
        {
            return;
        }

        FProperty* interfaceproperty = static_cast<FProperty*>(get_property_from_uobject(L"/Script/Engine.KismetSystemLibrary:Conv_InterfaceToObject", L"Interface"));
        UClass* interface_class = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, L"/Script/CoreUObject.Interface");

        if (!interfaceproperty || !interface_class)
        {
            throw std::runtime_error{"Was unable to find InterfaceProperty or it's interface class"};
        }

        bool success{};
        for (int32_t i = 0x40; i < 0x120; i += 0x8)
        {
            void* maybe_interface_class = Helper::Casting::offset_deref<void*>(interfaceproperty, i);
            if (maybe_interface_class == interface_class)
            {
                add_static_offset(MemberOffsets::InterfaceProperty_InterfaceClass, i, STR("InterfaceProperty_InterfaceClass"));
                success = true;
                break;
            }
        }

        if (!success)
        {
            throw std::runtime_error{"Was unable to find offset for InterfaceProperty.InterfaceClass"};
        }
    }

    auto StaticOffsetFinder::find_fieldpathproperty_property_class() -> void
    {
        if (retrieve_static_offset(MemberOffsets::FieldPathProperty_PropertyClass) != -1)
        {
            return;
        }

        FProperty* fieldpathproperty = static_cast<FProperty*>(get_property_from_uobject(L"/Script/Engine.InterpTrackInstProperty", L"InterpProperty"));
        FName property_class_name = FName(STR("Property"));

        if (!fieldpathproperty || property_class_name == FName(STR("None")))
        {
            throw std::runtime_error{"Was unable to find FieldPathProperty or it's property class name"};
        }

        bool success{};
        for (int32_t i = 0x40; i < 0x120; i += 0x8)
        {
            FName* maybe_pointer = Helper::Casting::offset_deref<FName*>(fieldpathproperty, i);

            // TODO: The FFieldClass.Name offset is hardcoded to 0x0 here, it probably shouldn't be
            constexpr int32_t ffieldclass_name_offset = 0x0;

            // Check if 'maybe_pointer' is a valid pointer
            int64_t maybe_property_class_name = Helper::Casting::offset_deref_safe<int64_t>(maybe_pointer, ffieldclass_name_offset);
            if (!maybe_property_class_name) { continue; }

            if (FName(maybe_property_class_name).Equals(property_class_name))
            {
                add_static_offset(MemberOffsets::FieldPathProperty_PropertyClass, i, STR("FieldPathProperty_PropertyClass"));
                success = true;
                break;
            }
        }

        if (!success)
        {
            throw std::runtime_error{"Was unable to find offset for FieldPathProperty.PropertyClass"};
        }
    }

    auto StaticOffsetFinder::find_setproperty_elementprop() -> void
    {
        if (retrieve_static_offset(MemberOffsets::SetProperty_ElementProp) != -1)
        {
            return;
        }

        FProperty* setproperty{};
        FName element_property_name{};

        if (Version::IsEqual(4, 13))
        {
            // In 4.13, 'LandscapeInfo:Proxies' is the only SetProperty that exists
            // So we use 'LandscapeInfo:Proxies' in 4.13 (SetProperty doesn't exist in <4.13)
            setproperty = static_cast<FProperty*>(get_property_from_uobject(L"/Script/Landscape.LandscapeInfo", L"Proxies"));
            element_property_name = FName(STR("Proxies"));
        }
        else
        {
            // In 4.24, 'LandscapeInfo:Proxies' was changed from a SetProperty to an ArrayProperty, so we use a different property
            setproperty = static_cast<FProperty*>(get_property_from_uobject(L"/Script/Engine.LevelCollection", L"Levels"));
            element_property_name = FName(STR("Levels"));
        }

        if (!setproperty || element_property_name == FName(STR("None")))
        {
            throw std::runtime_error{"Was unable to find SetProperty or it's element property name"};
        }

        bool success{};
        for (int32_t i = 0x40; i < 0x120; i += 0x8)
        {
            FProperty* maybe_element_prop = Helper::Casting::offset_deref<FProperty*>(setproperty, i);

            // Check if 'maybe_element_prop' is a valid pointer
            void* is_pointer = Helper::Casting::offset_deref_safe<void*>(maybe_element_prop, 0);
            if (!is_pointer) { continue; }

            if (maybe_element_prop->GetFName().Equals(element_property_name))
            {
                add_static_offset(MemberOffsets::SetProperty_ElementProp, i, STR("SetProperty_ElementProp"));
                success = true;
                break;
            }
        }

        if (!success)
        {
            throw std::runtime_error{"Was unable to find offset for SetProperty.ElementProp"};
        }
    }

    auto StaticOffsetFinder::find_ustruct_reflink() -> void
    {
        if (retrieve_static_offset(MemberOffsets::UStruct_RefLink) != -1)
        {
            return;
        }

        if (retrieve_static_offset(MemberOffsets::XField_ChildProperties) == -1)
        {
            throw std::runtime_error("[StaticOffsetFinder->find_ustruct_reflink] find_children() must be run before find_bool_member_variables()");
        }

        void* ref_link_first_elem = get_property_from_uobject(STR("/Script/Engine.HitResult"), STR("PhysMaterial"));
        if (!ref_link_first_elem)
        {
            throw std::runtime_error("[StaticOffsetFinder->find_ustruct_reflink] Was unable to find HitResult.PhysMaterial");
        }

        UStruct* hit_result = UObjectGlobals::StaticFindObject<UStruct*>(nullptr, nullptr, L"/Script/Engine.HitResult");

        bool success{};
        for (int32_t i = 0x50; i < 0x130; i += 0x8)
        {
            void* maybe_ref_link = Helper::Casting::offset_deref<void*>(hit_result, i);
            if (maybe_ref_link == ref_link_first_elem)
            {
                add_static_offset(MemberOffsets::UStruct_RefLink, i, STR("UStruct_RefLink"));
                success = true;
                break;
            }
        }

        if (!success)
        {
            throw std::runtime_error{"[StaticOffsetFinder->find_ustruct_reflink] Was unable to find UStruct.RefLink"};
        }
    }

    auto StaticOffsetFinder::find_uscriptstruct_structflags() -> void
    {
        if (retrieve_static_offset(MemberOffsets::UScriptStruct_StructFlags) != -1)
        {
            return;
        }

        if (retrieve_static_offset(MemberOffsets::XField_ChildProperties) == -1)
        {
            throw std::runtime_error("[StaticOffsetFinder->find_uscriptstruct_structflags] find_ustruct_reflink() must be run before find_uscriptstruct_structflags()");
        }

        int32_t ref_link_offset = retrieve_static_offset(MemberOffsets::UStruct_RefLink);
        if (ref_link_offset == -1)
        {
            throw std::runtime_error("[StaticOffsetFinder->find_uscriptstruct_structflags] find_children() must be run before find_ustruct_reflink()");
        }

        // The 'HitResult' script struct is used here because the memory layout and memory values are known, including the number and locations of the zero sets.
        UStruct* hit_result = UObjectGlobals::StaticFindObject<UStruct*>(nullptr, nullptr, L"/Script/Engine.HitResult");

        constexpr static uint32_t zero_set_size = 0x8;
        uint32_t num_zero_sets{}; // A zero set is a set of 8 zeros
        int32_t struct_flags_offset{};
        for (int32_t i = ref_link_offset += 0x8; i < 0x130; i += 0x8)
        {
            void* current_location = Helper::Casting::offset_deref<void*>(hit_result, i);
            if (!current_location) { ++num_zero_sets; }

            // In 4.25+, there are the variables 'UnresolvedScriptProperties' and 'UnversionedSchema', both are pointers
            // So in 4.25+, there should be 6 sets of zeros instead of 4
            if (Version::IsBelow(4, 25) && num_zero_sets == 4)
            {
                struct_flags_offset = i + zero_set_size;
                break;
            }
            else if (Version::IsAtLeast(4, 25) && num_zero_sets == 6)
            {
                struct_flags_offset = i + zero_set_size;
                break;
            }
        }

        if (num_zero_sets == 4 || num_zero_sets == 6)
        {
            add_static_offset(MemberOffsets::UScriptStruct_StructFlags, struct_flags_offset, STR("UScriptStruct_StructFlags"));
        }
        else
        {
            throw std::runtime_error{"[StaticOffsetFinder->find_uscriptstruct_structflags] Was unable to find UScriptStruct.StructFlags"};
        }
    }

    auto StaticOffsetFinder::find_uclass_interfaces() -> void
    {
        if (retrieve_static_offset(MemberOffsets::UClass_Interfaces) != -1)
        {
            return;
        }

        UClass* pawn = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, L"/Script/Engine.Pawn");
        UClass* nav_agent_interface = UObjectGlobals::StaticFindObject<UClass*>(nullptr, nullptr, L"/Script/Engine.NavAgentInterface");

        if (!pawn || !nav_agent_interface)
        {
            throw std::runtime_error("[StaticOffsetFinder->find_uclass_interfaces] Either 'Pawn' or 'NavAgentInterface' weren't found");
        }

        bool success{};
        for (int32_t i = 0x50; i < 0x250; i += 0x8)
        {
            unsigned char* maybe_interfaces = Helper::Casting::offset_deref<unsigned char*>(pawn, i);
            if (!maybe_interfaces) { continue; }

            UClass* maybe_nav_agent_interface = Helper::Casting::offset_deref_safe<UClass*>(maybe_interfaces, 0);

            if (maybe_nav_agent_interface == nav_agent_interface)
            {
                add_static_offset(MemberOffsets::UClass_Interfaces, i, STR("UClass_Interfaces"));
                success = true;
                break;
            }
        }

        if (!success)
        {
            throw std::runtime_error{"[StaticOffsetFinder::find_uclass_interfaces] Was unable to find UClass.Interfaces"};
        }
    }
}

