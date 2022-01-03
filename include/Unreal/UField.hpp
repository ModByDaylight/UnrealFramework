#ifndef RC_UNREAL_UFIELD_HPP
#define RC_UNREAL_UFIELD_HPP

#include <Unreal/UObject.hpp>

namespace RC::Unreal
{
    class RC_API UField : public UObject
    {
    public:
        // For non-properties, i.e: functions
        template<typename ChildType = FField*>
        auto get_next_child() -> ChildType
        {
            return Helper::Casting::offset_deref<ChildType>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UField_Next));
        }

        // In <4.25: Returns properties and functions
        // In 4.25+: Returns functions
        template<typename ChildType = FField*>
        auto get_children() -> ChildType
        {
            return Helper::Casting::offset_deref<ChildType>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::XField_Children));
        }

        // In <4.25: Identical to 'get_children'
        // In 4.25+: Returns properties
        template<typename ChildType = FField*>
        auto get_childproperties() -> ChildType
        {
            return Helper::Casting::offset_deref<ChildType>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::XField_ChildProperties));
        }

        template<typename TypeParam1>
        using ForEachChildCallable = std::function<LoopAction(TypeParam1)>;

        // Iterates through properties only
        auto for_each_property(std::function<LoopAction(XProperty*)> callable) -> LoopAction;

        // Iterates through functions only
        auto for_each_function(ForEachChildCallable<UFunction*> callable) -> LoopAction;
    };
}

#endif //RC_UNREAL_UFIELD_HPP
