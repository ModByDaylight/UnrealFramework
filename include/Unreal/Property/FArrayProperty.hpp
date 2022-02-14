#ifndef RC_UNREAL_FARRAYPROPERTY_HPP
#define RC_UNREAL_FARRAYPROPERTY_HPP

#include <Unreal/FProperty.hpp>
#include <Unreal/StaticOffsetFinder.hpp>

namespace RC::Unreal
{
    class RC_UE_API FArrayProperty : public FProperty
    {
        DECLARE_FIELD_CLASS(FArrayProperty);
        DECLARE_VIRTUAL_TYPE(FArrayProperty);

    public:
        auto GetInner() -> FProperty*;
    };
}


#endif //RC_UNREAL_FARRAYPROPERTY_HPP
