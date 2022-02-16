#ifndef RC_UNREAL_DERIVED425_HPP
#define RC_UNREAL_DERIVED425_HPP

#include <Unreal/VersionedContainer/Base.hpp>

namespace RC::Unreal
{
    class Derived425 : public Default
    {
    public:
        auto Field_get_type_fname(FField* p_this) -> FName override;
        auto Field_get_ffield_owner(FField* p_this) -> void* override;
        auto FFieldClass_get_fname(FFieldClass* p_this) -> FName override;
        auto find_property_vc(void* uobject, const wchar_t* property_string, WithSideEffects with_side_effects, ExcludeSelf = ExcludeSelf::Yes) -> PropertyDataVC override;
        auto read_property_vc(void* uobject, const wchar_t* property_string, CustomProperty* custom_property = nullptr) -> PropertyDataVC override;
    };
}


#endif //RC_UNREAL_DERIVED425_HPP
