#include <Unreal/FField.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UFunction.hpp>
#include <Unreal/FProperty.hpp>

namespace RC::Unreal
{
    FFieldClassVariant FField::m_static_class{(FFieldClass*) nullptr};

    auto FField::static_class() -> FFieldClassVariant
    {

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

    auto FField::get_outermost_owner() -> UObject*
    {
        FFieldVariant current_variant = get_owner_variant();

        while (!current_variant.is_uobject()) {
            current_variant = current_variant.get_owner_variant();
        }
        return current_variant.to_uobject();
    }

    auto FField::as_uobject_unsafe() -> UField*
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

    auto FField::get_next_ffield_unsafe() -> FField*
    {
        if (Version::is_below(4, 25))
        {
            throw std::runtime_error("FField::Next is not available in UE versions below 4.25");
        }
        return Helper::Casting::offset_deref<FField*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::FField_Next));
    }

    auto FFieldClassVariant::get_fname() -> FName {
        if (is_uobject())
        {
            return to_uobject()->get_fname();
        }
        else
        {
            return to_field()->get_fname();
        }
    }

    auto FFieldClassVariant::get_super_class() -> FFieldClassVariant
    {
        if (is_uobject())
        {
            return to_uobject()->get_super_class();
        }
        else
        {
            return to_field()->get_super_class();
        }
    }

    auto FFieldClassVariant::is_child_of(FFieldClassVariant uclass) -> bool
    {
        //Comparisons between UClass and FFieldClass never return true
        if (is_uobject() != uclass.is_uobject())
        {
            return false;
        }

        if (is_uobject())
        {
            return to_uobject()->is_child_of(uclass.to_uobject());
        }
        else
        {
            return to_field()->is_child_of(uclass.to_field());
        }
    }

    auto FFieldClass::get_fname() -> FName
    {
        return name;
    }

    auto FFieldClass::get_super_class() -> FFieldClass*
    {
        return super_class;
    }

    auto FFieldClass::is_child_of(FFieldClass* field_class) -> bool
    {
        FFieldClass* current_class = this;
        do {
            if (current_class == field_class)
            {
                return true;
            }
            current_class = field_class->get_super_class();
        }
        while (field_class);
        return false;
    }
}
