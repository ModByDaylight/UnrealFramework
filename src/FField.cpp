#include <Unreal/FField.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UFunction.hpp>
#include <Unreal/VersionedContainer/Container.hpp>

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FField);

    using MemberOffsets = ::RC::Unreal::StaticOffsetFinder::MemberOffsets;

    bool Internal::FFieldTypeAccessor::type_system_initialized = false;
    std::vector<void(*)()> Internal::FFieldTypeAccessor::late_bind_callbacks{};

    auto Internal::FFieldTypeAccessor::get_object_class(FField* field) -> FFieldClassVariant
    {
        return field->get_class();
    }

    auto Internal::FFieldTypeAccessor::get_class_super_class(FFieldClassVariant field_class) -> FFieldClassVariant
    {
        return field_class.get_super_class();
    }

    auto Internal::FFieldTypeAccessor::is_class_valid(FFieldClassVariant field_class) -> bool
    {
        return field_class.is_valid();
    }

    auto Internal::FFieldTypeAccessor::register_late_bind_callback(void (*callback)()) -> void
    {
        if (!type_system_initialized)
        {
            late_bind_callbacks.push_back(callback);
        }
        else
        {
            callback();
        }
    }

    auto Internal::FFieldTypeAccessor::on_type_system_initialized() -> void
    {
        if (type_system_initialized)
        {
            return;
        }
        type_system_initialized = true;

        for (void (*callback)() : late_bind_callbacks)
        {
            callback();
        }
    }

    auto FField::get_class() -> FFieldClassVariant
    {
        if (Version::is_below(4, 25))
        {
            return as_ufield_unsafe()->get_uclass();
        }
        else
        {
            return get_ffieldclass_unsafe();
        }
    }

    auto FField::get_fname() -> FName
    {
        if (Version::is_below(4, 25))
        {
            return as_ufield_unsafe()->get_fname();
        }
        else
        {
            return get_ffield_fname_unsafe();
        }
    }

    auto FField::is_a(const FFieldClassVariant& uclass) -> bool
    {
        return get_class().is_child_of(uclass);
    }

    auto FField::get_owner_variant() -> FFieldVariant
    {
        if (Version::is_below(4, 25))
        {
            return as_ufield_unsafe()->get_outer();
        }
        else
        {
            return get_ffield_owner_unsafe();
        }
    }

    auto FField::get_typed_owner(UClass* owner_type) -> UObject*
    {
        FFieldVariant current_variant = get_owner_variant();

        while (current_variant.is_valid()) {
            if (current_variant.is_uobject())
            {
                return current_variant.to_uobject()->get_typed_outer(owner_type);
            }
            current_variant = current_variant.get_owner_variant();
        }
        return nullptr;
    }

    auto FField::as_ufield_unsafe() -> UField*
    {
        if (!Version::is_below(4, 25))
        {
            throw std::runtime_error("FField does not inherit from UObject in UE4.25+");
        }
        return std::bit_cast<UField*>(this);
    }

    auto FField::get_ffieldclass_unsafe() -> FFieldClass*
    {
        if (Version::is_below(4, 25))
        {
            throw std::runtime_error("FFieldClass is not available in UE versions below 4.25");
        }
        return Helper::Casting::offset_deref<FFieldClass*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::FField_Class));
    }

    auto FField::get_ffield_owner_unsafe() -> FFieldVariant
    {
        if (Version::is_below(4, 25))
        {
            throw std::runtime_error("FField::Owner is not available in UE versions below 4.25");
        }
        return Helper::Casting::offset_deref<FFieldVariant>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::FField_Owner));
    }

    auto FField::get_ffield_fname_unsafe() -> FName
    {
        if (Version::is_below(4, 25))
        {
            throw std::runtime_error("FField::Owner is not available in UE versions below 4.25");
        }
        return Helper::Casting::offset_deref<FName>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::FField_NamePrivate));
    }

    auto FField::serialize(FArchive& ar) -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FField, Serialize, void, PARAMS(FArchive&), ARGS(ar))
    }

    auto FField::post_load() -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FField, PostLoad, void)
    }

    auto FField::get_preload_dependencies(TArray<UObject*>& out_deps) -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FField, GetPreloadDependencies, void, PARAMS(TArray<UObject*>&), ARGS(out_deps))
    }

    auto FField::begin_destroy() -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FField, BeginDestroy, void)
    }

    using FReferenceCollector = void*; // Remove if/when we have an FArchive implementation, for now, probably a bad idea to call
    auto FField::add_referenced_objects(FReferenceCollector& collector) -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FField, AddReferencedObjects, void, PARAMS(FReferenceCollector&), ARGS(collector))
    }

    auto FField::add_cpp_property(FProperty* property) -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FField, AddCppProperty, void, PARAMS(FProperty*), ARGS(property))
    }

    auto FField::bind() -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FField, Bind, void)
    }

    auto FField::post_duplicate(const FField& in_field) -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FField, PostDuplicate, void, PARAMS(const FField&), ARGS(in_field))
    }

    auto FField::get_inner_field_by_name(const FName& in_name) -> FField*
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FField, GetInnerFieldByName, FField*, PARAMS(const FName&), ARGS(in_name))
    }

    auto FField::get_inner_fields(TArray<FField*>& out_fields) -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FField, GetInnerFields, void, PARAMS(TArray<FField*>&), ARGS(out_fields))
    }

    auto FField::get_next_ffield_unsafe() -> FField*
    {
        if (Version::is_below(4, 25))
        {
            throw std::runtime_error("FField::Next is not available in UE versions below 4.25");
        }
        return Helper::Casting::offset_deref<FField*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::FField_Next));
    }

    FFieldClassVariant::FFieldClassVariant(FFieldClass* field) : is_object(false)
    {
        container.field = field;
    }

    FFieldClassVariant::FFieldClassVariant(UClass* object) : is_object(true)
    {
        container.object = object;
    }

    FFieldClassVariant::FFieldClassVariant() : is_object(false)
    {
        container.field = nullptr;
    }

    auto FFieldClassVariant::is_valid() const -> bool
    {
        return container.object;
    }

    auto FFieldClassVariant::is_uclass() const -> bool
    {
        return is_valid() && is_object;
    }

    auto FFieldClassVariant::is_field_class() const -> bool
    {
        return is_valid() && !is_object;
    }

    auto FFieldClassVariant::to_field_class() const -> FFieldClass*
    {
        if (is_field_class())
        {
            return container.field;
        }
        else
        {
            throw std::runtime_error("FFieldClassVariant does not represent a FFieldClass");
        }
    }

    auto FFieldClassVariant::to_uclass() const -> UClass*
    {
        if (is_uclass())
        {
            return container.object;
        }
        else
        {
            throw std::runtime_error("FFieldClassVariant does not represent a UClass");
        }
    }

    auto FFieldClassVariant::get_fname() const -> FName {
        if (is_uclass())
        {
            return to_uclass()->get_fname();
        }
        else
        {
            return to_field_class()->get_fname();
        }
    }

    auto FFieldClassVariant::get_super_class() const -> FFieldClassVariant
    {
        if (is_uclass())
        {
            return to_uclass()->get_super_class();
        }
        else
        {
            return to_field_class()->get_super_class();
        }
    }

    auto FFieldClassVariant::is_child_of(FFieldClassVariant uclass) const -> bool
    {
        //Comparisons between UClass and FFieldClass never return true
        if (is_uclass() != uclass.is_uclass())
        {
            return false;
        }

        if (is_uclass())
        {
            return to_uclass()->is_child_of(uclass.to_uclass());
        }
        else
        {
            return to_field_class()->is_child_of(uclass.to_field_class());
        }
    }

    auto FFieldClassVariant::operator==(const RC::Unreal::FFieldClassVariant& rhs) const -> bool {
        if (!is_valid() || !rhs.is_valid())
        {
            return false;
        }
        if (is_uclass() != rhs.is_uclass())
        {
            return false;
        }
        if (is_uclass())
        {
            return to_uclass() == rhs.to_uclass();
        }
        else
        {
            return to_field_class() == rhs.to_field_class();
        }
    }

    auto FFieldClassVariant::hash_object() const -> size_t
    {
        if (!is_valid())
        {
            return 0;
        }
        if (is_uclass())
        {
            return reinterpret_cast<size_t>(to_uclass());
        }
        else
        {
            return reinterpret_cast<size_t>(to_field_class());
        }
    }

    auto FFieldClass::get_fname() const -> FName
    {
        return name;
    }

    auto FFieldClass::get_super_class() const -> FFieldClass*
    {
        return super_class;
    }

    auto FFieldClass::is_child_of(FFieldClass* field_class) const -> bool
    {
        const FFieldClass* current_class = this;
        do {
            if (current_class == field_class)
            {
                return true;
            }
            current_class = field_class->get_super_class();
        }
        while (current_class);
        return false;
    }
}
