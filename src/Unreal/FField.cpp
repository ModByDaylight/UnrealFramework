#include <Unreal/FField.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UFunction.hpp>
#include <Unreal/FProperty.hpp>
#include <Unreal/VersionedContainer/Container.hpp>
#include <Helpers/Casting.hpp>

#define IMPLEMENT_FFIELD_VIRTUAL_WRAPPER(class_name, function_name, return_type, params, args) \
if constexpr(Version::IsBelow(4, 25)) \
{ \
    throw std::runtime_error{"FField virtual called in <4.25 and it has no equivalent virtual in <4.25"}; \
} \
IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(class_name, function_name, return_type, params, args)

#define IMPLEMENT_FFIELD_VIRTUAL_WRAPPER_NO_PARAMS(class_name, function_name, return_type) \
if constexpr(Version::is_below(4, 25)) \
{ \
    throw std::runtime_error{"FField virtual called in <4.25 and it has no equivalent virtual in <4.25"}; \
} \
IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(class_name, function_name, return_type)

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FField);

    using MemberOffsets = ::RC::Unreal::StaticOffsetFinder::MemberOffsets;

    bool Internal::FFieldTypeAccessor::TypeSystemInitialized = false;
    std::vector<void(*)()> Internal::FFieldTypeAccessor::LateBindCallbacks{};
    std::unordered_map<std::wstring, uint32_t> FField::VTableLayoutMap{};

#include <MemberVariableLayout_SrcWrapper_FField.hpp>

    auto Internal::FFieldTypeAccessor::GetObjectClass(FField* Field) -> FFieldClassVariant
    {
        return Field->GetClass();
    }

    auto Internal::FFieldTypeAccessor::GetClassSuperClass(FFieldClassVariant FieldClass) -> FFieldClassVariant
    {
        return FieldClass.GetSuperClass();
    }

    auto Internal::FFieldTypeAccessor::IsClassValid(FFieldClassVariant FieldClass) -> bool
    {
        return FieldClass.IsValid();
    }

    auto Internal::FFieldTypeAccessor::RegisterLateBindCallback(void (*Callback)()) -> void
    {
        if (!TypeSystemInitialized)
        {
            LateBindCallbacks.push_back(Callback);
        }
        else
        {
            Callback();
        }
    }

    auto Internal::FFieldTypeAccessor::OnTypeSystemInitialized() -> void
    {
        if (TypeSystemInitialized)
        {
            return;
        }
        TypeSystemInitialized = true;

        for (void (*callback)() : LateBindCallbacks)
        {
            callback();
        }
    }

    auto FField::GetClass() -> FFieldClassVariant
    {
        if constexpr(Version::IsBelow(4, 25))
        {
            return AsUFieldUnsafe()->GetClassPrivate();
        }
        else
        {
            return GetFFieldClassUnsafe();
        }
    }

    auto FField::GetFName() -> FName
    {
        if constexpr(Version::IsBelow(4, 25))
        {
            return AsUFieldUnsafe()->GetNamePrivate();
        }
        else
        {
            return GetFFieldFNameUnsafe();
        }
    }

    std::wstring FField::GetFullName()
    {
        if constexpr(Version::IsAtLeast(4, 25 ))
        {
            std::wstring FullName = GetClass().GetName();
            FullName += STR(" ");
            FullName += GetPathName();
            return FullName;
        }
        else
        {
            return AsUFieldUnsafe()->GetFullName();
        }
    }

    std::wstring FField::GetPathName(UObject* StopOuter)
    {
        if constexpr(Version::IsAtLeast(4, 25))
        {
            std::wstring PathName;
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
                    PathName += ObjectOwner->GetPathName(StopOuter);
                    PathName += SUBOBJECT_DELIMITER_CHAR;
                    break;
                }
            }
            PathName += GetName();
            return PathName;
        }
        else
        {
            return AsUFieldUnsafe()->GetPathName();
        }
    }

    auto FField::IsA(const FFieldClassVariant& UClass) -> bool
    {
        return GetClass().IsChildOf(UClass);
    }

    auto FField::GetOwnerVariant() -> FFieldVariant
    {
        if constexpr(Version::IsBelow(4, 25))
        {
            return AsUFieldUnsafe()->GetOuterPrivate();
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
        FFieldVariant CurrentVariant = GetOwnerVariant();

        while (CurrentVariant.IsValid()) {
            if (CurrentVariant.IsUObject())
            {
                return CurrentVariant.ToUObject()->GetTypedOuter(OwnerType);
            }
            CurrentVariant = CurrentVariant.GetOwnerVariant();
        }
        return nullptr;
    }

    bool FField::HasNext()
    {
        if constexpr(Version::IsBelow(4, 25))
        {
            return AsUFieldUnsafe()->GetNext();
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
        auto* Next = GetNext();
        return CastField<FProperty>(Next);
    }

    auto FField::AsUFieldUnsafe() -> UField*
    {
        if (!Version::IsBelow(4, 25))
        {
            throw std::runtime_error("FField does not inherit from UObject in UE4.25+");
        }
        return std::bit_cast<UField*>(this);
    }


    auto FField::AsUFieldUnsafe() const -> const UField*
    {
        if (!Version::IsBelow(4, 25))
        {
            throw std::runtime_error("FField does not inherit from UObject in UE4.25+");
        }
        return std::bit_cast<const UField*>(this);
    }

    auto FField::GetFFieldClassUnsafe() -> FFieldClass*
    {
        if constexpr(Version::IsBelow(4, 25))
        {
            throw std::runtime_error("FFieldClass is not available in UE versions below 4.25");
        }
        return GetClassPrivate();
    }

    auto FField::GetFFieldOwnerUnsafe() -> FFieldVariant
    {
        if constexpr(Version::IsBelow(4, 25))
        {
            throw std::runtime_error("FField::Owner is not available in UE versions below 4.25");
        }
        return GetOwner();
    }

    auto FField::GetFFieldFNameUnsafe() -> FName
    {
        if constexpr(Version::IsBelow(4, 25))
        {
            throw std::runtime_error("FField::FName is not available in UE versions below 4.25");
        }
        return GetNamePrivate();
    }

    auto FField::Serialize(FArchive& Ar) -> void
    {
        if constexpr(Version::IsAtLeast(4, 25))
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
        if constexpr(Version::IsAtLeast(4, 25))
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
        if constexpr(Version::IsAtLeast(4, 25))
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
        if constexpr(Version::IsAtLeast(4, 25))
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
        if constexpr(Version::IsAtLeast(4, 25))
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
        if constexpr(Version::IsAtLeast(4, 25))
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
        if constexpr(Version::IsAtLeast(4, 25))
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
        if constexpr(Version::IsAtLeast(4, 25))
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
        if constexpr(Version::IsAtLeast(4, 25))
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
        if constexpr(Version::IsBelow(4, 25))
        {
            throw std::runtime_error("FField::Next is not available in UE versions below 4.25");
        }
        return GetNext();
    }

    FFieldClassVariant::FFieldClassVariant(FFieldClass* Field) : IsObject(false)
    {
        Container.Field = Field;
    }

    FFieldClassVariant::FFieldClassVariant(UClass* Object) : IsObject(true)
    {
        Container.Object = Object;
    }

    FFieldClassVariant::FFieldClassVariant(const UClass* Object) : IsObject(true)
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

    auto FFieldClassVariant::ToUClass() const -> const UClass*
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
            return ToUClass()->GetNamePrivate();
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
            return ToUClass()->GetSuperClass();
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
            return ToUClass()->IsChildOf(UClass.ToUClass());
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

    auto FFieldClass::IsChildOf(FFieldClass* FieldClass) const -> bool
    {
        const FFieldClass* CurrentClass = this;
        do {
            if (CurrentClass == FieldClass)
            {
                return true;
            }
            CurrentClass = CurrentClass->GetSuperClass();
        }
        while (CurrentClass);
        return false;
    }

    auto FFieldVariant::GetOwnerVariant() -> FFieldVariant
    {
        if (IsUObject())
        {
            return Container.Object->GetOuterPrivate();
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
