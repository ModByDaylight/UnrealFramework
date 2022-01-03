#include <Unreal/UClass.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/XProperty.hpp>

namespace RC::Unreal
{
    auto UScriptStruct::has_any_struct_flags(EStructFlags flags_to_check)
    {
        return (get_struct_flags() & flags_to_check) != 0;
    }

    auto UScriptStruct::has_all_struct_flags(EStructFlags flags_to_check)
    {
        return (get_struct_flags() & flags_to_check) == flags_to_check;
    }

    auto UScriptStruct::get_struct_flags() -> EStructFlags
    {
        return Helper::Casting::offset_deref<EStructFlags>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UScriptStruct_StructFlags));
    }

    auto UScriptStruct::find_property(FName property_name, Base::WithSideEffects with_side_effects, ExcludeSelf exclude_self) -> XProperty*
    {
        PropertyDataVC data = Container::m_unreal_vc_base->find_property_vc(this, property_name.to_string().c_str(), with_side_effects, exclude_self);
        return static_cast<XProperty*>(data.property_ptr);
    }

    auto UScriptStruct::to_string_complex(void* p_this, std::wstring& out_line, TypeChecker::ObjectToStringComplexDeclCallable callable) -> void
    {
        UScriptStruct* script_struct = static_cast<UScriptStruct*>(p_this);

        script_struct->for_each_property([&](XProperty* prop) {
            callable(prop);
            return LoopAction::Continue;
        });
    }
}