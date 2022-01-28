#ifndef RC_UNREAL_USCRIPTSTRUCT_HPP
#define RC_UNREAL_USCRIPTSTRUCT_HPP

#include <Helpers/Casting.hpp>

#include <Unreal/UStruct.hpp>
#include <Unreal/StaticOffsetFinder.hpp>
#include <Unreal/TypeChecker.hpp>

namespace RC::Unreal
{
    class RC_UE_API UScriptStruct : public UStruct
    {
        DECLARE_EXTERNAL_OBJECT_CLASS(UScriptStruct, CoreUObject);
    public:
        /**
         * Returns the struct acting as a parent to this struct
         * Can totally return nullptr since most structs do not have a parent struct
         */
        auto get_super_script_struct() -> UScriptStruct*;

        /**
         * Returns the flags set on the struct
         * Some of the returned flags are determined automatically in runtime, others are saved
         */
        auto get_struct_flags() -> EStructFlags;

        /**
         * Checks if the struct has any of the provided flags set
         * Returns true if at least one of the provided struct flags is set
         */
        inline auto has_any_struct_flags(EStructFlags flags_to_check)
        {
            return (get_struct_flags() & flags_to_check) != 0;
        }

        /**
         * Checks if the struct has all of the provided flags set
         * Returns true only if all of the provided struct flags are set
         */
        inline auto has_all_struct_flags(EStructFlags flags_to_check)
        {
            return (get_struct_flags() & flags_to_check) == flags_to_check;
        }
    };
}

#endif //RC_UNREAL_USCRIPTSTRUCT_HPP
