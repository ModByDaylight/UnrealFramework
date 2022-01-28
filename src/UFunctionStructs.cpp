#include <Unreal/UFunctionStructs.hpp>
#include <Unreal/UFunction.hpp>
#include "DynamicOutput/Output.hpp"
#include "Helpers/Format.hpp"
#include "Unreal/Hooks.hpp"

namespace RC::Unreal
{
    UnrealScriptFunctionData::UnrealScriptFunctionData(UnrealScriptFunction original_func_ptr) {
        this->original_func = original_func_ptr;
        this->hook_index_counter = 1;
    }

    CallbackId UnrealScriptFunctionData::add_pre_callback(const UnrealScriptFunctionCallable& callable)
    {
        CallbackId new_callback_id = this->hook_index_counter++;
        this->pre_callbacks.insert({new_callback_id, callable});
        return new_callback_id;
    }

    CallbackId UnrealScriptFunctionData::add_post_callback(const UnrealScriptFunctionCallable& callable)
    {
        CallbackId new_callback_id = this->hook_index_counter++;
        this->post_callbacks.insert({new_callback_id, callable});
        return new_callback_id;
    }

    bool UnrealScriptFunctionData::remove_callback(CallbackId callback_id)
    {
        return pre_callbacks.erase(callback_id) ||
            post_callbacks.erase(callback_id);
    }

    void UnrealScriptFunctionData::remove_all_callbacks()
    {
        pre_callbacks.clear();
        post_callbacks.clear();
    }

    void UnrealScriptFunctionData::fire_pre_callbacks(UnrealScriptFunctionCallableContext& context) const
    {
        for (const auto& pair : pre_callbacks)
        {
            pair.second(context);
        }
    }

    void UnrealScriptFunctionData::fire_post_callbacks(UnrealScriptFunctionCallableContext& context) const
    {
        for (const auto& pair : post_callbacks)
        {
            pair.second(context);
        }
    }

    UnrealScriptFunctionCallableContext::UnrealScriptFunctionCallableContext(UObject* context, FFrame& the_stack, void* result_decl) {
        this->context = context;
        this->the_stack = the_stack;
        this->result_decl = result_decl;
    }

    static Internal::HookedUFunctionMap g_hooked_script_functions{};

    auto Internal::get_hooked_functions_map() -> HookedUFunctionMap&
    {
        return g_hooked_script_functions;
    }

    auto Internal::unreal_script_function_hook(UObject* context, FFrame& the_stack, void* result_decl) -> void
    {
        try
        {
            HookedUFunctionMap& function_map = get_hooked_functions_map();
            const auto iterator = function_map.find(the_stack.CurrentNativeFunction);

            if (iterator == function_map.end())
            {
                throw std::runtime_error("Failed to find the function map entry for hooked function");
            }

            const UnrealScriptFunctionCallableContext context(context, the_stack, result_decl);

            iterator->second.fire_pre_callbacks(context);
            iterator->second.get_original_func_ptr()(context, the_stack, result_decl);
            iterator->second.fire_post_callbacks(context);
        }
        catch (std::exception& e)
        {
            Output::send(STR("{}\n"), RC::fmt(L"Error executing hooked function %s: %S", the_stack.CurrentNativeFunction->get_path_name().c_str(), e.what()));
        }
    }
}
