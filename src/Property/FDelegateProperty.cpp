#include <Unreal/Property/FDelegateProperty.hpp>

namespace RC::Unreal
{
    auto FDelegateProperty::GetFunctionSignature() -> UFunction*
    {
        return Helper::Casting::offset_deref<UFunction*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::DelegateProperty_SignatureFunction));
    }
}
