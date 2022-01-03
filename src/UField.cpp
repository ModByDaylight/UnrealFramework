#include <Unreal/UField.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UFunction.hpp>
#include <Unreal/XProperty.hpp>

namespace RC::Unreal
{
    auto UField::for_each_function(ForEachChildCallable<UFunction*> callable) -> LoopAction
    {
        UFunction* function = get_children<UFunction*>();
        if (!function) { return LoopAction::Continue; }

        bool is_below_425 = Version::is_below(4, 25);
        LoopAction loop_action{};

        do
        {
            if (loop_action == LoopAction::Break) { return LoopAction::Break; }

            if (is_below_425)
            {
                // In <4.25, the linked list contains both properties and functions
                // We must filter out all non-functions
                if (!function->get_uclass()->is_child_of<UFunction>())
                {
                    function = function->get_next_child<UFunction*>();
                    continue;
                }
            }

            loop_action = callable(function);

            function = function->get_next_child<UFunction*>();
        } while (function);

        return loop_action;
    }

    auto UField::for_each_property(std::function<LoopAction(XProperty*)> callable) -> LoopAction
    {
        XProperty* child = get_childproperties<XProperty*>();
        if (!child) { return LoopAction::Continue; }

        bool is_below_425 = Version::is_below(4, 25);
        LoopAction loop_action{};

        do
        {
            if (loop_action == LoopAction::Break) { return LoopAction::Break; }

            if (is_below_425)
            {
                // In <4.25, the linked list contains both properties and functions
                // We must filter out all functions
                if (child->as_object()->get_uclass()->is_child_of<UFunction>())
                {
                    child = child->get_next<XProperty*>();
                    continue;
                }
            }

            loop_action = callable(child);

            child = child->get_next<XProperty*>();
        } while (child);

        return loop_action;
    }
}
