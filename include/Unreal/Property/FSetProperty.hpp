#ifndef RC_UNREAL_FSETPROPERTY_HPP
#define RC_UNREAL_FSETPROPERTY_HPP

#include <Unreal/FProperty.hpp>

namespace RC::Unreal
{
    class RC_UE_API FSetProperty : public FProperty
    {
        DECLARE_FIELD_CLASS(FSetProperty);
        DECLARE_VIRTUAL_TYPE(FSetProperty);

    public:
        FProperty* GetElementProp();

        // Uncomment when and if we need these functions
        /*
        using FScriptSetLayout = void;
        FScriptSetLayout GetSetLayout();
        //*/
    };
}


#endif //RC_UNREAL_FSETPROPERTY_HPP
