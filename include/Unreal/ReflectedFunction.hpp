#ifndef RC_UNREAL_REFLECTEDFUNCTION_HPP
#define RC_UNREAL_REFLECTEDFUNCTION_HPP

#include <File/Macros.hpp>

#include <Unreal/Common.hpp>
#include <Unreal/UObject.hpp>

namespace RC::Unreal
{
    class RC_UE_API ReflectedFunctionBase
    {
    protected:
        class UFunction* m_function{nullptr};
        const File::CharType* m_function_name{};

    public:
        ReflectedFunctionBase(const File::CharType* function_name) : m_function(nullptr), m_function_name(function_name) {}

    public:
        auto get_name() const -> const File::CharType* { return m_function_name; }
        auto assign_function(UFunction* function) -> void { m_function = function; }
        auto is_valid() -> bool;
    };

    template<typename Owner>
    class RC_UE_API StaticReflectedFunctionBase : public ReflectedFunctionBase
    {
    public:
        template<typename ParamsType>
        auto operator()(ParamsType& params)
        {
            Owner::self->process_event(this->m_function, &params);
        }
    };

    class RC_UE_API InstancedReflectedFunction: public ReflectedFunctionBase
    {
    public:
        template<typename ParamsType>
        auto operator()(UObject* instance, ParamsType& params)
        {
            instance->process_event(this->m_function, &params);
        }
    };
}

#endif //RC_UNREAL_REFLECTEDFUNCTION_HPP
