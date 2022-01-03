#ifndef RC_UNREAL_UFUNCTIONSTRUCTS_HPP
#define RC_UNREAL_UFUNCTIONSTRUCTS_HPP

#include <functional>

#include <Unreal/Common.hpp>
#include <Unreal/FFrame.hpp>

namespace RC::Unreal
{
    class UObject;

    using UnrealScriptFunction = void(*)(UObject* context, FFrame& the_stack, void* result_decl);

    struct RC_API UnrealScriptFunctionCallableContextParam
    {
        UObject* context;
        FFrame& the_stack;
        void* result_decl;

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
    using UnrealScriptFunctionCallable = std::function<void(UnrealScriptFunctionCallableContextParam context, void* custom_data)>;

    struct RC_API UnrealScriptFunctionPreCallback
    {
        UnrealScriptFunctionCallable callable;

        // The hooked-function must cast before using the 'custom_data' pointer
        // This pointer should be passed to the hooked function as the last parameter
        void* custom_data{};
    };

    struct RC_API UnrealScriptFunctionPostCallback
    {
        UnrealScriptFunctionCallable callable;

        // The hooked-function must cast before using the 'custom_data' pointer
        // This pointer should be passed to the hooked function as the last parameter
        void* custom_data{};
    };

    struct RC_API UnrealScriptFunctionData
    {
        UnrealScriptFunction original_func;
        std::vector<UnrealScriptFunctionPreCallback> pre_callbacks{};
        std::vector<UnrealScriptFunctionPostCallback> post_callbacks{};
    };
}

#endif //RC_UNREAL_UFUNCTIONSTRUCTS_HPP
