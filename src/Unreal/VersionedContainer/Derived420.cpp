#include <Unreal/VersionedContainer/Derived420.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/FProperty.hpp>
#include <Helpers/Casting.hpp>

namespace RC::Unreal
{
    auto Derived420::Field_get_type_fname(FField* p_this) -> FName
    {
        return p_this->GetClass().GetFName();
    }

    auto Derived420::Field_get_ffield_owner([[maybe_unused]]FField* p_this) -> void*
    {
        // FField does not exist before 4.25 so therefor FField::Owner also does not exist
        // It's up to the caller to make sure the returned value isn't nullptr
        return nullptr;
    }

    auto Derived420::FFieldClass_get_fname(FFieldClass* p_this) -> FName
    {
        return Helper::Casting::offset_deref<FName>(p_this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UObject_NamePrivate));
    }
}
