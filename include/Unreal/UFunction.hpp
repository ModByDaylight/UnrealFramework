#ifndef RC_UNREAL_UFUNCTION_HPP
#define RC_UNREAL_UFUNCTION_HPP

#include <Unreal/UStruct.hpp>
#include <Unreal/UFunctionStructs.hpp>
#include <Unreal/Hooks.hpp>

namespace RC::Unreal
{
    struct FFrame;

    class RC_UE_API  UFunction : public UStruct
    {
        DECLARE_EXTERNAL_OBJECT_CLASS(UFunction, CoreUObject)
    public:
        /**
         * Retrieves the flag set on this function object
         * Function calls define their type, metadata, access and additional attributes
         */
        auto get_function_flags() -> EFunctionFlags;

        /**
         * Checks if function has any of the flags specified
         * Returns true if either of the provided flags are set on the function
         */
        inline auto has_any_function_flags(EFunctionFlags function_flags) -> bool
        {
            return (get_function_flags() & function_flags) != 0;
        }

        /**
         * Checks if function has all of the specified flags
         * Returns true only if all of the provided flags are set on the function
         */
        inline auto has_all_function_flags(EFunctionFlags function_flags) -> bool
        {
            return (get_function_flags() & function_flags) == function_flags;
        }

        /**
         * Returns amount of parameters of the function, including
         * output parameters and the return value if it is defined for the function
         */
        auto get_num_parms() -> uint8_t;

        /**
         * Returns the total size of the function params used by the function
         * Keep in mind that this value is different from properties size, as it includes
         * only function parameters and does not include other local variables
         */
        auto get_parms_size() -> int32_t;

        /**
         * Retrieves the return property of the function, or returns nullptr
         * if the function does not have a return value
         */
        auto get_return_property() -> FProperty*;

        /**
         * Retrieves the pointer to the underlying native function
         * For script functions that would point to UObject::ProcessInternal, which handles
         * execution of the Blueprint VM bytecode, otherwise it would point to the actual native function
         */
        auto get_func_ptr() -> UnrealScriptFunction;

        /**
         * Updates function pointer to the new value
         * The provided function will be executed when this UFunction object is called
         */
        auto set_func_ptr(UnrealScriptFunction new_func_ptr) -> void;

        auto register_pre_hook(const UnrealScriptFunctionCallable& pre_callback) -> CallbackId;
        auto register_post_hook(const UnrealScriptFunctionCallable& post_callback) -> CallbackId;
        auto unregister_hook(CallbackId callback_id) -> bool;
        auto unregister_all_hooks() -> void;
    protected:
        auto get_function_hook_data() -> UnrealScriptFunctionData&;
    };
}


#endif //RC_UNREAL_UFUNCTION_HPP
