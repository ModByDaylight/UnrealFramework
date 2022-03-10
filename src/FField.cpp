#include <Unreal/FField.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UFunction.hpp>
#include <Unreal/FProperty.hpp>
#include <Unreal/VersionedContainer/Container.hpp>

#define IMPLEMENT_FFIELD_VIRTUAL_WRAPPER(class_name, function_name, return_type, params, args) \
if (Version::is_below(4, 25)) \
{ \
    throw std::runtime_error{"FField virtual called in <4.25 and it has no equivalent virtual in <4.25"}; \
} \
IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(class_name, function_name, return_type, params, args)

#define IMPLEMENT_FFIELD_VIRTUAL_WRAPPER_NO_PARAMS(class_name, function_name, return_type) \
if (Version::is_below(4, 25)) \
{ \
    throw std::runtime_error{"FField virtual called in <4.25 and it has no equivalent virtual in <4.25"}; \
} \
IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(class_name, function_name, return_type)

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

    File::StringType FField::GetFullName()
    {
        if (Version::is_atleast(4, 25 ))
        {
            File::StringType FullName = GetClass().GetName();
            FullName += STR(" ");
            FullName += GetPathName();
            return FullName;
        }
        else
        {
            return AsUFieldUnsafe()->get_full_name();
        }
    }

    File::StringType FField::GetPathName(UObject* StopOuter)
    {
        if (Version::is_atleast(4, 25))
        {
            File::StringType PathName;
            for (FFieldVariant TempOwner = GetOwnerVariant(); TempOwner.IsValid(); TempOwner = TempOwner.GetOwnerVariant())
            {
                if (!TempOwner.IsUObject())
                {
                    FField* FieldOwner = TempOwner.ToField();
                    PathName = FieldOwner->GetName() + STR(".") + PathName;
                }
                else
                {
                    UObject* ObjectOwner = TempOwner.ToUObject();
                    PathName += ObjectOwner->get_path_name(StopOuter);
                    PathName += SUBOBJECT_DELIMITER_CHAR;
                    break;
                }
            }
            PathName += GetName();
            return PathName;
        }
        else
        {
            return AsUFieldUnsafe()->get_path_name();
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

    UObject* FField::GetOutermostOwner()
    {
        auto CurrentVariant = GetOwnerVariant();

        while (!CurrentVariant.IsUObject())
        {
            CurrentVariant = CurrentVariant.GetOwnerVariant();
        }

        return CurrentVariant.ToUObject();
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

    bool FField::HasNext()
    {
        if (Version::is_below(4, 25))
        {
            return AsUFieldUnsafe()->get_next_ufield();
        }
        else
        {
            return GetNextFFieldUnsafe();
        }
    }

    FProperty* FField::GetNextFieldAsProperty()
    {
        // In <4.25, we can cast to a property because all properties inherits from UField
        // In <4.25, this FField struct represents UField
        // In 4.25+, properties inherit from FField so there's no problem with this cast
        auto* Next = Helper::Casting::ptr_cast_deref<FField*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::FField_Next));
        return CastField<FProperty>(Next);
    }

    auto FField::AsUFieldUnsafe() -> UField*
    {
        if (!Version::is_below(4, 25))
        {
            throw std::runtime_error("FField does not inherit from UObject in UE4.25+");
        }
        return std::bit_cast<UField*>(this);
    }


    auto FField::AsUFieldUnsafe() const -> const UField*
    {
        if (!Version::is_below(4, 25))
        {
            throw std::runtime_error("FField does not inherit from UObject in UE4.25+");
        }
        return std::bit_cast<const UField*>(this);
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
        if (Version::is_atleast(4, 25))
        {
            IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FField, Serialize, void, PARAMS(FArchive & ), ARGS(Ar))
        }
        else
        {
            // In <4.25, all FField instances in UE4SS is actually UField
            // Since UProperty inherits from UField, we must redirect virtual calls to UField and UObject
            AsUFieldUnsafe()->Serialize(Ar);
        }
    }

    auto FField::PostLoad() -> void
    {
        if (Version::is_atleast(4, 25))
        {
            IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FField, PostLoad, void)
        }
        else
        {
            // In <4.25, all FField instances in UE4SS is actually UField
            // Since UProperty inherits from UField, we must redirect virtual calls to UField and UObject
            AsUFieldUnsafe()->PostLoad();
        }
    }

    auto FField::GetPreloadDependencies(TArray<UObject*>& OutDeps) -> void
    {
        if (Version::is_atleast(4, 25))
        {
            IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FField, GetPreloadDependencies, void, PARAMS(TArray<UObject*> & ), ARGS(OutDeps))
        }
        else
        {
            AsUFieldUnsafe()->GetPreloadDependencies(OutDeps);
        }
    }

    auto FField::BeginDestroy() -> void
    {
        if (Version::is_atleast(4, 25))
        {
            IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FField, BeginDestroy, void)
        }
        else
        {
            AsUFieldUnsafe()->BeginDestroy();
        }
    }

    using FReferenceCollector = void*; // Remove if/when we have an FArchive implementation, for now, probably a bad idea to call
    auto FField::AddReferencedObjects(FReferenceCollector& Collector) -> void
    {
        IMPLEMENT_FFIELD_VIRTUAL_WRAPPER(FField, AddReferencedObjects, void, PARAMS(FReferenceCollector&), ARGS(Collector))
    }

    auto FField::AddCppProperty(FProperty* Property) -> void
    {
        if (Version::is_atleast(4, 25))
        {
            IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FField, AddCppProperty, void, PARAMS(FProperty*), ARGS(Property))
        }
        else
        {
            // In <4.25, all FField instances in UE4SS is actually UField
            // Since UProperty inherits from UField, we must redirect virtual calls to UField and UObject
            AsUFieldUnsafe()->AddCppProperty(Property);
        }
    }

    auto FField::Bind() -> void
    {
        if (Version::is_atleast(4, 25))
        {
            IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FField, Bind, void)
        }
        else
        {
            // In <4.25, all FField instances in UE4SS is actually UField
            // Since UProperty inherits from UField, we must redirect virtual calls to UField and UObject
            AsUFieldUnsafe()->Bind();
        }
    }

    auto FField::PostDuplicate(const FField& InField) -> void
    {
        IMPLEMENT_FFIELD_VIRTUAL_WRAPPER(FField, PostDuplicate, void, PARAMS(const FField&), ARGS(InField))
    }

    auto FField::GetInnerFieldByName(const FName& InName) -> FField*
    {
        IMPLEMENT_FFIELD_VIRTUAL_WRAPPER(FField, GetInnerFieldByName, FField*, PARAMS(const FName&), ARGS(InName))
    }

    auto FField::GetInnerFields(TArray<FField*>& OutFields) -> void
    {
        IMPLEMENT_FFIELD_VIRTUAL_WRAPPER(FField, GetInnerFields, void, PARAMS(TArray<FField*>&), ARGS(OutFields))
    }

    void FField::PostDuplicate(bool bDuplicateForPIE)
    {
        if (Version::is_atleast(4, 25))
        {
            throw std::runtime_error{"FField::PostDuplicate overload with bool param is only allowed to be called in <4.25"};
        }
        else
        {
            AsUFieldUnsafe()->PostDuplicate(bDuplicateForPIE);
        }
    }

    bool FField::NeedsLoadForClient() const
    {
        if (Version::is_atleast(4, 25))
        {
            throw std::runtime_error{"FField::NeedsLoadForClient is only allowed to be called in <4.25"};
        }
        else
        {
            return AsUFieldUnsafe()->NeedsLoadForClient();
        }
    };

    bool FField::NeedsLoadForServer() const
    {
        if (Version::is_atleast(4, 25))
        {
            throw std::runtime_error{"FField::NeedsLoadForServer is only allowed to be called in <4.25"};
        }
        else
        {
            return AsUFieldUnsafe()->NeedsLoadForServer();
        }
    };

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

    auto FFieldVariant::HashObject() -> size_t
    {
        if (!IsValid())
        {
            return 0;
        }
        if (IsUObject())
        {
            return reinterpret_cast<size_t>(ToUObject());
        }
        else
        {
            return reinterpret_cast<size_t>(ToField());
        }
    }
}
