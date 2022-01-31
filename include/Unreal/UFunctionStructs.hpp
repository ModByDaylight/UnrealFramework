#ifndef RC_UNREAL_UFUNCTIONSTRUCTS_HPP
#define RC_UNREAL_UFUNCTIONSTRUCTS_HPP

#include <functional>
#include <Unreal/Common.hpp>
#include <Unreal/FFrame.hpp>
#include <map>

namespace RC::Unreal
{
    class UObject;

    using UnrealScriptFunction = void(*)(UObject* context, FFrame& the_stack, void* result_decl);

    class RC_UE_API UnrealScriptFunctionCallableContext
    {
    public:
        UObject* context;
        FFrame& the_stack;
        void* result_decl;

        UnrealScriptFunctionCallableContext(UObject* context, FFrame& the_stack, void* result_decl);

        template<typename ReturnType>
        auto set_return_value(ReturnType new_value) -> void
        {
            *static_cast<ReturnType*>(result_decl) = new_value;
        }

        template<typename ParamStruct>
        auto get_params() -> ParamStruct&
        {
            return *static_cast<ParamStruct*>(static_cast<void*>(the_stack.Locals));
        }
    };
    using UnrealScriptFunctionCallable = std::function<void(UnrealScriptFunctionCallableContext& context)>;
    using CallbackId = int32_t;

    class RC_UE_API UnrealScriptFunctionData
    {
    private:
        UnrealScriptFunction original_func;
        CallbackId hook_index_counter;
        std::map<int32_t, UnrealScriptFunctionCallable> pre_callbacks;
        std::map<int32_t, UnrealScriptFunctionCallable> post_callbacks;
    public:
        UnrealScriptFunctionData(UnrealScriptFunction original_func_ptr);
        inline auto get_original_func_ptr() const -> UnrealScriptFunction { return original_func; }

        CallbackId add_pre_callback(const UnrealScriptFunctionCallable& callable);
        CallbackId add_post_callback(const UnrealScriptFunctionCallable& callable);
        bool remove_callback(CallbackId callback_id);

        void remove_all_callbacks();

        void fire_pre_callbacks(UnrealScriptFunctionCallableContext& context) const;
        void fire_post_callbacks(UnrealScriptFunctionCallableContext& context) const;
    };

    namespace Internal {
        using HookedUFunctionMap = std::unordered_map<const UFunction*, UnrealScriptFunctionData>;
        auto RC_UE_API get_hooked_functions_map() -> HookedUFunctionMap&;

        auto unreal_script_function_hook(UObject *context, FFrame &the_stack, void *result_decl) -> void;
    }
}

#endif //RC_UNREAL_UFUNCTIONSTRUCTS_HPP
