#include <Unreal/Property/FMapProperty.hpp>
#include <Helpers/Casting.hpp>

namespace RC::Unreal
{
    IMPLEMENT_FIELD_CLASS(FMapProperty);

#include <MemberVariableLayout_SrcWrapper_FMapProperty.hpp>

    FString FMapProperty::GetCPPTypeCustom(FString* ExtendedTypeText, uint32 CPPExportFlags, const FString& KeyTypeText, const FString& InKeyExtendedTypeText, const FString& ValueTypeText, const FString& InValueExtendedTypeText) const {
        if (ExtendedTypeText) {
            // if property type is a template class, add a space between the closing brackets
            FString KeyExtendedTypeText = InKeyExtendedTypeText;

            if ((KeyExtendedTypeText.Len() && KeyExtendedTypeText.Right(1) == STR(">"))
                || (!KeyExtendedTypeText.Len() && KeyTypeText.Len() && KeyTypeText.Right(1) == STR(">"))) {
                KeyExtendedTypeText += STR(" ");
            }

            // if property type is a template class, add a space between the closing brackets
            FString ValueExtendedTypeText = InValueExtendedTypeText;

            if ((ValueExtendedTypeText.Len() && ValueExtendedTypeText.Right(1) == STR(">"))
                || (!ValueExtendedTypeText.Len() && ValueTypeText.Len() && ValueTypeText.Right(1) == STR(">"))) {
                ValueExtendedTypeText += STR(" ");
            }

            *ExtendedTypeText = FString::Printf(STR("<%s%s,%s%s>"), *KeyTypeText, *KeyExtendedTypeText, *ValueTypeText, *ValueExtendedTypeText);
        }
        return STR("TMap");
    }
}
