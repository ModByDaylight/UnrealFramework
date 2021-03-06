#include <Unreal/UEnum.hpp>
#include <Unreal/UClass.hpp>
#include <Helpers/Casting.hpp>

namespace RC::Unreal
{
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UEnum);
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UUserDefinedEnum);

    using MemberOffsets = ::RC::Unreal::StaticOffsetFinder::MemberOffsets;

    auto UEnum::GetEnumNames() -> FEnumNameArray&
    {
        return *Helper::Casting::ptr_cast<FEnumNameArray*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UEnum_Names));
    }

    auto UEnum::GetCppForm() -> ECppForm
    {
        // The offset of 'Names' + the size of 'Names' = offset of 'CppForm'
        return Helper::Casting::offset_deref<ECppForm>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UEnum_Names) + sizeof(FEnumNameArray));
    }

    auto UEnum::GetEnumFlags() -> EEnumFlags
    {
        if constexpr(Version::IsBelow(4, 26))
        {
            // EEnumFlags was added in 4.26
            // Let's assume the 'None' flag is valid for whatever code calls this function in <4.26
            return EEnumFlags::None;
        }
        else
        {
            // The offset of 'Names' + the size of 'Names' + the size of 'CppForm' = offset of 'EnumFlags'
            return Helper::Casting::offset_deref<EEnumFlags>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UEnum_Names) + sizeof(FEnumNameArray) + sizeof(ECppForm));
        }
    }
}
