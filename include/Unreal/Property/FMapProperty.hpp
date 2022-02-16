#ifndef RC_UNREAL_FMAPPROPERTY_HPP
#define RC_UNREAL_FMAPPROPERTY_HPP

#include <Unreal/FProperty.hpp>

namespace RC::Unreal
{
    struct RC_UE_API MapData
    {
        void* elements_ptr;
        int32_t num_elements;
        int32_t max_elements;
    };

    class RC_UE_API FMapProperty : public FProperty
    {
        DECLARE_FIELD_CLASS(FMapProperty);
        DECLARE_VIRTUAL_TYPE(FMapProperty);

    public:
        auto GetKeyProp() -> FProperty*;
        auto GetValueProp() -> FProperty*;

        // Uncomment when and if we need these functions
        /*
        using FScriptMapLayout = void;
        FScriptMapLayout GetMapLayout();

        using EMapPropertyFlags = void;
        EMapPropertyFlags GetMapFlags();
        //*/
    };
}


#endif //RC_UNREAL_FMAPPROPERTY_HPP
