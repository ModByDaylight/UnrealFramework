#include <Unreal/Property/FArrayProperty.hpp>
#include <Helpers/Casting.hpp>

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FArrayProperty);

#include <MemberVariableLayout_SrcWrapper_FArrayProperty.hpp>

    FString FArrayProperty::GetCPPTypeCustom(FString* ExtendedTypeSTR, uint32 CPPExportFlags, const FString& InnerTypeSTR, const FString& InInnerExtendedTypeSTR) const {
        if (ExtendedTypeSTR != NULL)
        {
            FString InnerExtendedTypeSTR = InInnerExtendedTypeSTR;
            if (InnerExtendedTypeSTR.Len() && InnerExtendedTypeSTR.Right(1) == STR(">"))
            {
                // if our internal property type is a template class, add a space between the closing brackets b/c VS.NET cannot parse this correctly
                InnerExtendedTypeSTR += STR(" ");
            }
            else if (!InnerExtendedTypeSTR.Len() && InnerTypeSTR.Len() && InnerTypeSTR.Right(1) == STR(">"))
            {
                // if our internal property type is a template class, add a space between the closing brackets b/c VS.NET cannot parse this correctly
                InnerExtendedTypeSTR += STR(" ");
            }
            *ExtendedTypeSTR = FString::Printf(STR("<%s%s>"), *InnerTypeSTR, *InnerExtendedTypeSTR);
        }
        return STR("TArray");
    }
}
