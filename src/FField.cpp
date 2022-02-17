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
        return field->GetClass();
    }

    auto Internal::FFieldTypeAccessor::get_class_super_class(FFieldClassVariant field_class) -> FFieldClassVariant
    {
        return field_class.GetSuperClass();
    }

    auto Internal::FFieldTypeAccessor::is_class_valid(FFieldClassVariant field_class) -> bool
    {
        return field_class.IsValid();
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

    auto FField::GetClass() -> FFieldClassVariant
    {
        if (Version::is_below(4, 25))
        {
            return AsUFieldUnsafe()->get_uclass();
        }
        else
        {
            return GetFFieldClassUnsafe();
        }
    }

    auto FField::GetFName() -> FName
    {
        if (Version::is_below(4, 25))
        {
            return AsUFieldUnsafe()->get_fname();
        }
        else
        {
            return GetFFieldFNameUnsafe();
        }
    }

    auto FField::IsA(const FFieldClassVariant& UClass) -> bool
    {
        return GetClass().IsChildOf(UClass);
    }

    auto FField::GetOwnerVariant() -> FFieldVariant
    {
        if (Version::is_below(4, 25))
        {
            return AsUFieldUnsafe()->get_outer();
        }
        else
        {
            return GetFFieldOwnerUnsafe();
        }
    }

    auto FField::GetTypedOwner(UClass* OwnerType) -> UObject*
    {
        FFieldVariant current_variant = GetOwnerVariant();

        while (current_variant.IsValid()) {
            if (current_variant.IsUObject())
            {
                return current_variant.ToUObject()->get_typed_outer(OwnerType);
            }
            current_variant = current_variant.GetOwnerVariant();
        }
        return nullptr;
    }

    auto FField::AsUFieldUnsafe() -> UField*
    {
        if (!Version::is_below(4, 25))
        {
            throw std::runtime_error("FField does not inherit from UObject in UE4.25+");
        }
        return std::bit_cast<UField*>(this);
    }

    auto FField::GetFFieldClassUnsafe() -> FFieldClass*
    {
        if (Version::is_below(4, 25))
        {
            throw std::runtime_error("FFieldClass is not available in UE versions below 4.25");
        }
        return Helper::Casting::offset_deref<FFieldClass*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::FField_Class));
    }

    auto FField::GetFFieldOwnerUnsafe() -> FFieldVariant
    {
        if (Version::is_below(4, 25))
        {
            throw std::runtime_error("FField::Owner is not available in UE versions below 4.25");
        }
        return Helper::Casting::offset_deref<FFieldVariant>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::FField_Owner));
    }

    auto FField::GetFFieldFNameUnsafe() -> FName
    {
        if (Version::is_below(4, 25))
        {
            throw std::runtime_error("FField::FName is not available in UE versions below 4.25");
        }
        return Helper::Casting::offset_deref<FName>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::FField_NamePrivate));
    }

    auto FField::Serialize(FArchive& Ar) -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FField, Serialize, void, PARAMS(FArchive&), ARGS(Ar))
    }

    auto FField::PostLoad() -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FField, PostLoad, void)
    }

    auto FField::GetPreloadDependencies(TArray<UObject*>& OutDeps) -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FField, GetPreloadDependencies, void, PARAMS(TArray<UObject*>&), ARGS(OutDeps))
    }

    auto FField::BeginDestroy() -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FField, BeginDestroy, void)
    }

    using FReferenceCollector = void*; // Remove if/when we have an FArchive implementation, for now, probably a bad idea to call
    auto FField::AddReferencedObjects(FReferenceCollector& Collector) -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FField, AddReferencedObjects, void, PARAMS(FReferenceCollector&), ARGS(Collector))
    }

    auto FField::AddCppProperty(FProperty* Property) -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FField, AddCppProperty, void, PARAMS(FProperty*), ARGS(Property))
    }

    auto FField::Bind() -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FField, Bind, void)
    }

    auto FField::PostDuplicate(const FField& InField) -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FField, PostDuplicate, void, PARAMS(const FField&), ARGS(InField))
    }

    auto FField::GetInnerFieldByName(const FName& InName) -> FField*
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FField, GetInnerFieldByName, FField*, PARAMS(const FName&), ARGS(InName))
    }

    auto FField::GetInnerFields(TArray<FField*>& OutFields) -> void
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FField, GetInnerFields, void, PARAMS(TArray<FField*>&), ARGS(OutFields))
    }

    auto FField::GetNextFFieldUnsafe() -> FField*
    {
        if (Version::is_below(4, 25))
        {
            throw std::runtime_error("FField::Next is not available in UE versions below 4.25");
        }
        return Helper::Casting::offset_deref<FField*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::FField_Next));
    }

    FFieldClassVariant::FFieldClassVariant(FFieldClass* Field) : IsObject(false)
    {
        Container.Field = Field;
    }

    FFieldClassVariant::FFieldClassVariant(UClass* Object) : IsObject(true)
    {
        Container.Object = Object;
    }

    FFieldClassVariant::FFieldClassVariant() : IsObject(false)
    {
        Container.Field = nullptr;
    }

    auto FFieldClassVariant::IsValid() const -> bool
    {
        return Container.Object;
    }

    auto FFieldClassVariant::IsUClass() const -> bool
    {
        return IsValid() && IsObject;
    }

    auto FFieldClassVariant::IsFieldClass() const -> bool
    {
        return IsValid() && !IsObject;
    }

    auto FFieldClassVariant::ToFieldClass() const -> FFieldClass*
    {
        if (IsFieldClass())
        {
            return Container.Field;
        }
        else
        {
            throw std::runtime_error("FFieldClassVariant does not represent a FFieldClass");
        }
    }

    auto FFieldClassVariant::ToUClass() const -> UClass*
    {
        if (IsUClass())
        {
            return Container.Object;
        }
        else
        {
            throw std::runtime_error("FFieldClassVariant does not represent a UClass");
        }
    }

    auto FFieldClassVariant::GetFName() const -> FName {
        if (IsUClass())
        {
            return ToUClass()->get_fname();
        }
        else
        {
            return ToFieldClass()->GetFName();
        }
    }

    auto FFieldClassVariant::GetSuperClass() const -> FFieldClassVariant
    {
        if (IsUClass())
        {
            return ToUClass()->get_super_class();
        }
        else
        {
            return ToFieldClass()->GetSuperClass();
        }
    }

    auto FFieldClassVariant::IsChildOf(FFieldClassVariant UClass) const -> bool
    {
        //Comparisons between UClass and FFieldClass never return true
        if (IsUClass() != UClass.IsUClass())
        {
            return false;
        }

        if (IsUClass())
        {
            return ToUClass()->is_child_of(UClass.ToUClass());
        }
        else
        {
            return ToFieldClass()->IsChildOf(UClass.ToFieldClass());
        }
    }

    auto FFieldClassVariant::operator==(const RC::Unreal::FFieldClassVariant& Rhs) const -> bool {
        if (!IsValid() || !Rhs.IsValid())
        {
            return false;
        }
        if (IsUClass() != Rhs.IsUClass())
        {
            return false;
        }
        if (IsUClass())
        {
            return ToUClass() == Rhs.ToUClass();
        }
        else
        {
            return ToFieldClass() == Rhs.ToFieldClass();
        }
    }

    auto FFieldClassVariant::HashObject() const -> size_t
    {
        if (!IsValid())
        {
            return 0;
        }
        if (IsUClass())
        {
            return reinterpret_cast<size_t>(ToUClass());
        }
        else
        {
            return reinterpret_cast<size_t>(ToFieldClass());
        }
    }

    auto FFieldClass::GetFName() const -> FName
    {
        return Name;
    }

    auto FFieldClass::GetSuperClass() const -> FFieldClass*
    {
        return SuperClass;
    }

    auto FFieldClass::IsChildOf(FFieldClass* field_class) const -> bool
    {
        const FFieldClass* current_class = this;
        do {
            if (current_class == field_class)
            {
                return true;
            }
            current_class = current_class->GetSuperClass();
        }
        while (current_class);
        return false;
    }

    auto FFieldVariant::GetOwnerVariant() -> FFieldVariant
    {
        if (IsUObject())
        {
            return Container.Object->get_outer();
        }
        else
        {
            return Container.Field->GetOwnerVariant();
        }
    }
}
