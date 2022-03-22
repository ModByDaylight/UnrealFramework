#include <Unreal/UField.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UFunction.hpp>
#include <Unreal/VersionedContainer/Container.hpp>
#include <Helpers/Casting.hpp>

namespace RC::Unreal
{
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UField);

    using MemberOffsets = ::RC::Unreal::StaticOffsetFinder::MemberOffsets;

    auto UField::GetNextField() -> UField*
    {
        return Helper::Casting::offset_deref<UField*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UField_Next));
    }

    auto UField::AsFFieldUnsafe() -> FField*
    {
        if (!Version::IsBelow(4, 25))
        {
            throw std::runtime_error("Cannot convert UField to FField in UE versions above 4.25");
        }
        return std::bit_cast<FField*>(this);
    }

    void UField::AddCppProperty(FProperty* Property)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(UField, AddCppProperty, void, PARAMS(FProperty*), ARGS(Property))
    }

    void UField::Bind()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(UField, Bind, void)
    }
}
