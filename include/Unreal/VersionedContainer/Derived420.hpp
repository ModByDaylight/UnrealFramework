#ifndef RC_UNREAL_DERIVED420_HPP
#define RC_UNREAL_DERIVED420_HPP

#include <Unreal/VersionedContainer/Base.hpp>

namespace RC::Unreal
{
    class Derived420 : public Default
    {
    public:
        auto Field_get_type_fname(FField* p_this) -> FName override;
        auto Field_get_ffield_owner([[maybe_unused]]FField* p_this) -> void* override;
        auto FFieldClass_get_fname(FFieldClass* p_this) -> FName override;
    };
}


#endif //RC_UNREAL_DERIVED420_HPP
