#ifndef RC_UNREAL_STATIC_OFFSET_FINDER_HPP
#define RC_UNREAL_STATIC_OFFSET_FINDER_HPP

#include <cstdint>
#include <array>
#include <cstdio>

#include <Helpers/Casting.hpp>
#include <File/File.hpp>

#include <Unreal/Common.hpp>
#include <Unreal/UnrealInitializer.hpp>

#define UE4SS_ENUMERATE_MEMBER_OFFSETS \
    UE4SS_ENUMERATE_MEMBER_OFFSET(UObject_ClassPrivate) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(UObject_OuterPrivate) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(UStruct_SuperStruct) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(XField_Children) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(XField_ChildProperties) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(XField_Next) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(FField_Owner) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(FField_Next) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(FField_Class) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(UField_Next) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(StructProperty_Struct) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(ArrayProperty_Inner) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(XProperty_Offset_Internal) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(UFunction_NumParms) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(UFunction_ReturnValueOffset) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(UFunction_FunctionFlags) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(UFunction_Func) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(UStruct_PropertiesSize) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(UStruct_MinAlignment) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(UStruct_RefLink) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(UScriptStruct_StructFlags) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(UObject_NamePrivate) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(FField_NamePrivate) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(FieldSize_BoolProperty) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(ByteOffset_BoolProperty) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(ByteMask_BoolProperty) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(FieldMask_BoolProperty) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(UClass_ClassFlags) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(UClass_ClassWithin) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(UClass_ClassDefaultObject) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(UClass_Interfaces) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(Property_PropertyFlags) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(ByteProperty_Enum) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(EnumProperty_UnderlyingType) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(EnumProperty_Enum) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(UEnum_Names) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(ObjectProperty_PropertyClass) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(ClassProperty_MetaClass) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(SoftClassProperty_MetaClass) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(MapProperty_KeyProp) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(MapProperty_ValueProp) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(DelegateProperty_SignatureFunction) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(MulticastDelegateProperty_SignatureFunction) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(MulticastInlineDelegateProperty_SignatureFunction) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(MulticastSparseDelegateProperty_SignatureFunction) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(InterfaceProperty_InterfaceClass) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(FieldPathProperty_PropertyClass) \
    UE4SS_ENUMERATE_MEMBER_OFFSET(SetProperty_ElementProp)

namespace RC::Unreal
{
    class RC_API StaticOffsetFinder
    {
    public:
        enum class MemberOffsets : int32_t
        {
#define UE4SS_ENUMERATE_MEMBER_OFFSET(member_offset) member_offset,
            UE4SS_ENUMERATE_MEMBER_OFFSETS
#undef UE4SS_ENUMERATE_MEMBER_OFFSET

            Max,
        };

        auto static member_offset_to_string(MemberOffsets member_offset) -> const File::CharType*
        {
            switch (member_offset)
            {
#define UE4SS_ENUMERATE_MEMBER_OFFSET(member_offset) case MemberOffsets::member_offset: return STR(#member_offset);
                UE4SS_ENUMERATE_MEMBER_OFFSETS
#undef UE4SS_NUMERATE_MEMBER_OFFSET
            }

            throw std::runtime_error{"[member_offset_to_string] member offset outside of the range of the MemberOffsets enum"};
        }

    private:
        // Contains offsets to properties that never change after game init
        // Examples of such offsets are: UProperty.Offset_Internal, UObject.ClassPrivate & UFunction.ReturnValueOffset
        inline static std::array<int32_t, static_cast<std::underlying_type<MemberOffsets>::type>(MemberOffsets::Max)> m_static_offsets;
        inline static std::array<const wchar_t*, static_cast<std::underlying_type<MemberOffsets>::type>(MemberOffsets::Max)> m_debug_static_offset_names;

        inline static HANDLE m_process_handle;

        inline static bool m_initialized{false};
        inline static bool UE_BLUEPRINT_EVENTGRAPH_FASTCALLS{};

        static auto get_property_from_uobject(const wchar_t* uclass_string, const wchar_t* property_string) -> void*;

        static auto find_class_private() -> void; // Requires nothing
        static auto find_outer_private() -> void; // Requires ClassPrivate
        static auto find_super_struct() -> void; // Requires ClassPrivate
        static auto find_children() -> void;
        static auto find_ustruct_next() -> void;
        static auto find_child_properties() -> void;
        static auto find_ffield_owner() -> void;
        static auto find_ffield_next() -> void;
        static auto find_ffield_class() -> void;
        static auto find_ffield_fname() -> void;
        static auto find_structproperty_struct() -> void;
        static auto find_offset_internal() -> void;
        static auto find_ustruct_properties_size_and_min_alignment() -> void;
        static auto find_num_parms_and_return_value_offset() -> void;
        static auto find_function_func_offset() -> void;
        static auto find_uobject_fname() -> void;
        static auto find_bool_member_variables() -> void;
        static auto find_class_flags() -> void;
        static auto find_class_default_object() -> void;
        static auto find_uclass_interfaces() -> void;
        static auto find_property_flags() -> void;
        static auto find_property_class_and_meta_class() -> void;
        static auto find_softclassproperty_meta_class() -> void;
        static auto find_enum_members() -> void;
        static auto find_byteproperty_enum() -> void;
        static auto find_mapproperty_key_value_prop() -> void;
        static auto find_delegateproperty_signature_function() -> void;
        static auto find_multicastdelegateproperty_signature_function() -> void;
        static auto find_interfaceproperty_interface_class() -> void;
        static auto find_fieldpathproperty_property_class() -> void;
        static auto find_setproperty_elementprop() -> void;
        static auto find_ustruct_reflink() -> void;
        static auto find_uscriptstruct_structflags() -> void;

    public:
        static auto initialize(HANDLE process_handle) -> void;
        static auto is_initialized() -> bool;

        static auto add_static_offset(MemberOffsets member, int32_t offset, const wchar_t* debug_name) -> void;
        static auto retrieve_static_offset(MemberOffsets member) -> int32_t;

        static auto output_all_member_offsets() -> void;

        // Find offsets that don't depend on the existence of any objects
        static auto find_independent_offsets(HANDLE process_handle) -> void;

        // Find offsets that may or may not depend on the existence of one or more objects
        static auto find_offsets(HANDLE process_handle) -> void;
    };
}

#endif // RC_UNREAL_STATIC_OFFSET_FINDER_HPP
