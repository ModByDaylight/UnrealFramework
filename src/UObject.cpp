#include <Unreal/TypeChecker.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UScriptStruct.hpp>

namespace RC::Unreal
{
    auto UObject::trivial_dump_to_string(void* p_this, std::wstring& out_line, const wchar_t* post_delimiter) -> void
    {
        UObject* p_typed_this = static_cast<UObject*>(p_this);

        out_line.append(std::format(L"[{:016X}] ", reinterpret_cast<uintptr_t>(p_this)));
        p_typed_this->get_full_name(out_line);
        out_line.append(std::format(L" [n: {:X}] [c: {:016X}] [or: {:016X}]",
                                    p_typed_this->get_fname().get_comparison_index(),
                                    reinterpret_cast<uintptr_t>(p_typed_this->get_uclass()),
                                    reinterpret_cast<uintptr_t>(p_typed_this->get_outer())));
    }

    auto UObject::to_string(void* p_this, std::wstring& out_line) -> void
    {
        trivial_dump_to_string(p_this, out_line);
    }

    auto UObject::get_uclass() const -> UClass*
    {
        return Helper::Casting::offset_deref<UClass*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UObject_ClassPrivate));
    }

    auto UObject::get_outer() -> UObject*
    {
        return Helper::Casting::offset_deref<UObject*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UObject_OuterPrivate));
    }
    auto UObject::get_outer() const -> UObject*
    {
        return Helper::Casting::offset_deref<UObject*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UObject_OuterPrivate));
    }

    auto UObject::get_fname() -> FName
    {
        return Helper::Casting::offset_deref<FName>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UObject_NamePrivate));
    }

    auto UObject::get_fname() const -> FName
    {
        return Helper::Casting::offset_deref<FName>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UObject_NamePrivate));
    }

    auto UObject::get_type() -> UObjectType
    {
        return UObjectType{this};
    }

    auto UObject::get_name() -> std::wstring
    {
        return get_fname().to_string();
    }

    auto UObject::get_name() const -> std::wstring
    {
        return get_fname().to_string();
    }

    auto UObject::get_flags() const -> int32_t
    {
        // TODO: Hard-coding this for now. Consider not hard-coding this.
        return Container::m_unreal_object_base->UObject_get_flags(this);
    }

    auto UObject::set_flags(const EObjectFlags new_flag) -> void
    {
        Container::m_unreal_object_base->UObject_set_flags(this, {new_flag});
    }

    auto UObject::set_flags(const std::array<EObjectFlags, EObjectFlags_Max>& new_flags) -> void
    {
        Container::m_unreal_object_base->UObject_set_flags(this, new_flags);
    }

    auto UObject::has_any_flag(const EObjectFlags flags_to_check) -> bool
    {
        return Container::m_unreal_object_base->UObject_has_any_flag(this, {flags_to_check});
    }

    auto UObject::has_any_flag(const std::array<EObjectFlags, EObjectFlags_Max>& flags_to_check) -> bool
    {
        return Container::m_unreal_object_base->UObject_has_any_flag(this, flags_to_check);
    }

    auto UObject::has_all_flags(const EObjectFlags flags_to_check) -> bool
    {
        return Container::m_unreal_object_base->UObject_has_all_flags(this, {flags_to_check});
    }

    auto UObject::has_all_flags(const std::array<EObjectFlags, EObjectFlags_Max>& flags_to_check) -> bool
    {
        return Container::m_unreal_object_base->UObject_has_all_flags(this, flags_to_check);
    }

    auto UObject::get_object_item() -> FUObjectItem*
    {
        return static_cast<FUObjectItem*>(Container::m_unreal_vc_base->UObjectArray_index_to_object(get_internal_index()));
    }

    auto UObject::is_function() -> bool
    {
        return get_uclass()->get_fname() == TypeChecker::get_fname(L"Function");
    }

    auto UObject::is_derived_from_struct() -> bool
    {
        for (UStruct* obj = get_uclass(); obj; obj = obj->get_super_struct())
        {
            if (obj == UStruct::static_class()) { return true; }
        }

        return false;
    }

    auto UObject::is_script_struct() -> bool
    {
        return get_uclass() == UScriptStruct::static_class();
    }

    auto UObject::is_actor() -> bool
    {
        UClass* obj_class = get_uclass();
        if (!obj_class) { return false; }

        FName actor_name = TypeChecker::get_fname(L"Actor");

        if (get_fname().equals(actor_name)) { return true; }

        if (is_any_class())
        {
            UStruct* object_super_struct = static_cast<UClass*>(this)->get_super_struct();
            if (object_super_struct && object_super_struct->get_fname().equals(actor_name)) { return true; }
        }

        if (obj_class->get_fname().equals(actor_name)) { return true; }

        UStruct* super_struct = obj_class->get_super_struct();
        while (super_struct)
        {
            if (super_struct->get_fname().equals(actor_name))
            {
                return true;
            }

            super_struct = super_struct->get_super_struct();
        }

        return false;
    }

    auto UObject::is_widget() -> bool
    {
        UClass* obj_class = get_uclass();
        if (!obj_class) { return false; }

        FName widget_name = TypeChecker::get_fname(L"Widget");

        if (obj_class->get_fname() == widget_name) { return true; }

        UStruct* super_struct = obj_class->get_super_struct();
        while (super_struct)
        {
            if (super_struct->get_fname() == widget_name)
            {
                return true;
            }

            super_struct = super_struct->get_super_struct();
        }

        return false;
    }

    auto UObject::is_class() -> bool
    {
        return get_uclass()->get_fname() == TypeChecker::get_fname(L"Class");
    }

    auto UObject::is_any_class() -> bool
    {
        FName obj_name = get_uclass()->get_fname();
        if (obj_name == TypeChecker::get_fname(L"Class")) { return true; }
        if (obj_name == TypeChecker::get_fname(L"BlueprintGeneratedClass")) { return true; }
        if (obj_name == TypeChecker::get_fname(L"AnimBlueprintGeneratedClass")) { return true; }
        if (obj_name == TypeChecker::get_fname(L"DynamicClass")) { return true; }
        if (obj_name == TypeChecker::get_fname(L"LinkerPlaceholderClass")) { return true; }
        if (obj_name == TypeChecker::get_fname(L"AsyncActionLoadPrimaryAssetClass")) { return true; }
        if (obj_name == TypeChecker::get_fname(L"SoundClass")) { return true; }
        if (obj_name == TypeChecker::get_fname(L"SoundNodeSoundClass")) { return true; }
        if (obj_name == TypeChecker::get_fname(L"WidgetBlueprintGeneratedClass")) { return true; }

        return false;
    }

    auto UObject::is_enum() -> bool
    {
        return get_uclass()->get_fname() == TypeChecker::m_core_enum_name;
    }

    auto UObject::get_name_prefix() -> File::StringType
    {
        if (is_actor())
        {
            return STR("A");
        }
        else if (is_widget())
        {
            return STR("S");
        }
        else if (is_script_struct())
        {
            return STR("F");
        }
        else
        {
            return STR("U");
        }
    }

    auto UObject::find_property(FName property_name, Base::WithSideEffects with_side_effects, ExcludeSelf exclude_self) -> XProperty*
    {
        PropertyDataVC data = Container::m_unreal_vc_base->find_property_vc(this, property_name.to_string().c_str(), with_side_effects, exclude_self);
        return static_cast<XProperty*>(data.property_ptr);
    }

    auto UObject::get_property_internal(const wchar_t* property_string, class CustomProperty* custom_property) -> PropertyDataVC
    {
        PropertyDataVC data = Container::m_unreal_vc_base->read_property_vc(this, property_string, custom_property);

        return data;
    }

    auto UObject::process_event(UFunction* function, void* parms) -> void
    {
        process_event_internal(this, function, parms);
    }

    auto UObjectType::as_string() -> std::wstring
    {
        return m_object->get_uclass()->get_fname().to_string();
    }

    auto UObjectType::as_enum() -> EUObjectType
    {
        // TODO: implement enum-based type values
        // This will use either the fname or pointer of known core objects such as IntProperty
        // Pros of using fname:
        // * It's fast as it only compares two integers (possibly four when you account for both comparisonindex and number)
        // * While I'm fairly sure that these known objects don't get reallocated (which means new pointers)
        //   I also am not completely sure.
        //   Using fname should make it less likely to break as the fname won't change even if the pointer changes
        //
        // Pros of using pointer:
        // * It 's fast as it only compares two pointers
        return EUObjectType::Default;
    }

    auto UObjectType::as_fname() -> FName
    {
        return m_object->get_uclass()->get_fname();
    }
}