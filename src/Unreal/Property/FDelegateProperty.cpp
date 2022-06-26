#include <Unreal/Property/FDelegateProperty.hpp>
#include <Unreal/UFunction.hpp>
#include <Helpers/Casting.hpp>

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FDelegateProperty);

    auto FDelegateProperty::GetFunctionSignature() const -> UFunction*
    {
        return Helper::Casting::offset_deref<UFunction*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::DelegateProperty_SignatureFunction));
    }
}
