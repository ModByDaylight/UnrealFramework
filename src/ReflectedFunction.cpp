#include <Unreal/ReflectedFunction.hpp>
#include <Unreal/UFunction.hpp>
#include <DynamicOutput/DynamicOutput.hpp>

namespace RC::Unreal
{
    auto ReflectedFunctionBase::is_valid() -> bool
    {
        if (!m_function)
        {
            assign_function(UObjectGlobals::static_find_object<UFunction*>(nullptr, nullptr, get_name()));
            if (!m_function)
            {
                Output::send(STR("Was unable to locate '{}'\n"), File::StringViewType{get_name()});
                return false;
            }
        }

        return m_function;
    }
}
