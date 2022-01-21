#ifndef RC_UNREAL_UFUNCTION_HPP
#define RC_UNREAL_UFUNCTION_HPP

#include <Unreal/UStruct.hpp>
#include <Unreal/UFunctionStructs.hpp>
#include <Unreal/Hooks.hpp>

namespace RC::Unreal
{
    struct FFrame;

    class RC_UE_API UFunction : public UStruct
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
            if (!m_static_obj) { throw std::runtime_error{"[UFunction::get_static_obj_ptr] m_static_obj is nullptr"}; }

            return m_static_obj;
        }

    public:
        auto get_function_flags() -> EFunctionFlags;
        auto get_num_parms() -> uint8_t;
        auto get_return_value_offset() -> uint16_t;
        auto get_func_ptr() -> void*;
        auto set_func_ptr(UnrealScriptFunction) -> void;
        auto get_return_property() -> XProperty*;

        template<typename Callback, typename ReturnValueSetterCallback, typename CustomDataType = void>
        auto register_hook(const Callback& callback, const ReturnValueSetterCallback& return_value_setter_callback, CustomDataType* custom_data = nullptr) -> void
        {
            void* untyped_custom_data = static_cast<void*>(custom_data);

            auto add_callbacks = [&](UnrealScriptFunctionData& hook_data){
                hook_data.pre_callbacks.emplace_back(UnrealScriptFunctionPreCallback{
                        .callable = callback,
                        .custom_data = untyped_custom_data,
                });
                hook_data.post_callbacks.emplace_back(UnrealScriptFunctionPostCallback{
                        .callable = return_value_setter_callback,
                        .custom_data = untyped_custom_data
                });
            };

            HookedUFunctionMap& hooked_functions = get_hooked_functions_map();
            if (hooked_functions.contains(this))
            {
                // If the UFunction has already been hooked then all we need to do is add the callback to the vector of callbacks
                auto& hook_data = hooked_functions.at(this);

                add_callbacks(hook_data);
            }
            else
            {
                // If the UFunction hasn't been hooked yet, then we add the callback to the vector of callbacks and then hook the UFunction
                auto& hook_data = hooked_functions.emplace(this, UnrealScriptFunctionData{
                        .original_func = reinterpret_cast<UnrealScriptFunction>(get_func_ptr()),
                }).first->second;

                add_callbacks(hook_data);

                // The UFunction has to be hooked _after_ data has been inserted into the hooked functions map
                set_func_ptr(&unreal_script_function_hook);
            }
        }

        auto unhook_all() -> void;

    public:
        static auto to_string(void* p_this, std::wstring& out_line) -> void
        {
            UObject::trivial_dump_to_string(p_this, out_line, L":");
        }
    };
}


#endif //RC_UNREAL_UFUNCTION_HPP
