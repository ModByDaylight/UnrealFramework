#ifndef RC_UNREAL_USCRIPTSTRUCT_HPP
#define RC_UNREAL_USCRIPTSTRUCT_HPP

#include <Helpers/Casting.hpp>

#include <Unreal/UStruct.hpp>
#include <Unreal/StaticOffsetFinder.hpp>
#include <Unreal/TypeChecker.hpp>

namespace RC::Unreal
{
    class RC_API UScriptStruct : public UStruct
    {
    private:
        static inline class UClass* m_static_obj{};

    public:
        // DO NOT USE... MEANT FOR INTERNAL USE ONLY
        auto static set_static_obj_ptr(class UClass* ptr)
        {
            m_static_obj = ptr;
        }

        auto static static_class() -> class UClass*
        {
            if (!m_static_obj) { throw std::runtime_error{"[UScriptStruct::get_static_obj_ptr] m_static_obj is nullptr"}; }

            return m_static_obj;
        }

    public:
        auto get_size() -> int32_t
        {
            return Helper::Casting::offset_deref<int32_t>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UStruct_PropertiesSize));
        }

        auto get_super_script_struct() -> UScriptStruct* {
            return static_cast<UScriptStruct*>(get_super_struct());
        }

        auto has_any_struct_flags(EStructFlags flags_to_check);
        auto has_all_struct_flags(EStructFlags flags_to_check);
        auto get_struct_flags() -> EStructFlags;

        // Intentionally hides a non-virtual function
        // For this variant, there could be properties that belong to this object
        // As opposed to UObject that cannot have any properties itself
        auto find_property(FName property_name, Base::WithSideEffects with_side_effects = Base::WithSideEffects::No, ExcludeSelf = ExcludeSelf::No) -> XProperty*;

    public:
        static auto to_string_complex(void* p_this, std::wstring& out_line, TypeChecker::ObjectToStringComplexDeclCallable callable) -> void;
    };
}

#endif //RC_UNREAL_USCRIPTSTRUCT_HPP
