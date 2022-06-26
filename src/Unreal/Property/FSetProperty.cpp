#include <Unreal/Property/FSetProperty.hpp>
#include <Helpers/Casting.hpp>

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FSetProperty);

    FProperty* FSetProperty::GetElementProp() const
    {
        return Helper::Casting::offset_deref<FProperty*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::SetProperty_ElementProp));
    }

    /*
    FSetProperty::FScriptSetLayout FSetProperty::GetSetLayout()
    {
        static_assert(false, "FSetProperty::GetSetLayout not implemented");
    }
    //*/

    FString FSetProperty::GetCPPTypeCustom(FString* ExtendedTypeSTR, uint32 CPPExportFlags, const FString& ElementTypeSTR, const FString& InElemenSTRendedTypeSTR) const {
        if (ExtendedTypeSTR) {
            // if property type is a template class, add a space between the closing brackets
            FString ElemenSTRendedTypeSTR = InElemenSTRendedTypeSTR;
            if ((ElemenSTRendedTypeSTR.Len() && ElemenSTRendedTypeSTR.Right(1) == STR(">"))
                || (!ElemenSTRendedTypeSTR.Len() && ElementTypeSTR.Len() && ElementTypeSTR.Right(1) == STR(">"))) {
                ElemenSTRendedTypeSTR += STR(" ");
            }

            *ExtendedTypeSTR = FString::Printf(STR("<%s%s>"), *ElementTypeSTR, *ElemenSTRendedTypeSTR);
        }
        return STR("TSet");
    }
}
