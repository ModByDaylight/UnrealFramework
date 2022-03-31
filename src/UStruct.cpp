#include <Unreal/UStruct.hpp>
#include <Unreal/FField.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UFunction.hpp>
#include <Unreal/FProperty.hpp>
#include <Helpers/Casting.hpp>

namespace RC::Unreal
{
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UStruct);

    using MemberOffsets = ::RC::Unreal::StaticOffsetFinder::MemberOffsets;

    auto UStruct::GetChildren() -> UField*
    {
        return Helper::Casting::offset_deref<UField*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::XField_Children));
    }

    auto UStruct::GetChildProperties() -> FField*
    {
        if (Version::IsBelow(4, 25))
        {
            throw std::runtime_error("UStruct::ChildProperties is not available before UE 4.25");
        }
        return Helper::Casting::offset_deref<FField*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::XField_ChildProperties));
    }

    auto UStruct::GetSuperStruct() -> UStruct*
    {
        return Helper::Casting::offset_deref<UStruct*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UStruct_SuperStruct));
    }

    auto UStruct::GetSuperStruct() const -> const UStruct*
    {
        return Helper::Casting::offset_deref<UStruct*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UStruct_SuperStruct));
    }

    auto UStruct::GetPropertiesSize() -> int32_t
    {
        return Helper::Casting::offset_deref<int32_t>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UStruct_PropertiesSize));
    }

    auto UStruct::GetMinAlignment() -> int32_t
    {
        return Helper::Casting::offset_deref<int32_t>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UStruct_MinAlignment));
    }

    auto UStruct::IsChildOf(UStruct* Struct) const -> bool
    {
        auto* CurrentStruct = this;
        do {
            if (CurrentStruct == Struct)
            {
                return true;
            }
            CurrentStruct = CurrentStruct->GetSuperStruct();
        }
        while (CurrentStruct);
        return false;
    }

    auto UStruct::ForEachFunction(const std::function<LoopAction(UFunction*)>& Callable) -> void
    {
        UField* CurrentField = GetChildren();

        while (CurrentField != nullptr) {
            //Only trigger the callable on UFunction objects
            if (UFunction* Function = Cast<UFunction>(CurrentField)) {
                LoopAction LoopAction = Callable(Function);

                if (LoopAction == LoopAction::Break) {
                    break;
                }
            }
            CurrentField = CurrentField->GetNextField();
        }
    }

    auto UStruct::ForEachProperty(const std::function<LoopAction(FProperty* Property)>& Callable) -> void
    {
        if (Version::IsBelow(4, 25))
        {
            UField* CurrentField = GetChildren();

            while (CurrentField != nullptr) {
                FField* CurrentFieldAsFField = CurrentField->AsFFieldUnsafe();

                //Only trigger the callable on UProperty objects
                if (FProperty* Property = CastField<FProperty>(CurrentFieldAsFField)) {
                    LoopAction LoopAction = Callable(Property);

                    if (LoopAction == LoopAction::Break) {
                        break;
                    }
                }
                CurrentField = CurrentField->GetNextField();
            }
        }
        else
        {
            FField* CurrentField = GetChildProperties();

            while (CurrentField != nullptr) {
                //Only trigger the callable on FProperty objects
                if (FProperty* Property = CastField<FProperty>(CurrentField)) {
                    LoopAction LoopAction = Callable(Property);

                    if (LoopAction == LoopAction::Break) {
                        break;
                    }
                }
                CurrentField = CurrentField->GetNextFFieldUnsafe();
            }
        }
    }

    auto UStruct::ForEachSuperStruct(const ForEachSuperStructCallable& Callable) -> void
    {
        UStruct* SuperStruct = GetSuperStruct();
        LoopAction LoopAction{};

        while (SuperStruct)
        {
            LoopAction = Callable(SuperStruct);
            if (LoopAction == LoopAction::Break) { break; }

            SuperStruct = SuperStruct->GetSuperStruct();
        }
    }

    auto UStruct::ForEachPropertyInChain(const ForEachPropertyInChainCallable& Callable) -> void
    {
        UStruct* Class = this;

        while (Class)
        {
            bool bShouldOuterLoopBreak{};

            Class->ForEachProperty([&](FProperty* child) {
                if (Callable(child) == LoopAction::Break)
                {
                    bShouldOuterLoopBreak = true;
                    return LoopAction::Break;
                }
                else
                {
                    return LoopAction::Continue;
                }
            });

            if (bShouldOuterLoopBreak) { break; }

            Class->ForEachSuperStruct([&](UStruct* SuperStruct) {
                if (!SuperStruct) { return LoopAction::Continue; }

                SuperStruct->ForEachProperty([&](FProperty* Property) {
                    auto loop_action = Callable(Property);
                    if (loop_action == LoopAction::Break)
                    {
                        bShouldOuterLoopBreak = true;
                        return LoopAction::Break;
                    }
                    else
                    {
                        return LoopAction::Continue;
                    }
                });

                if (bShouldOuterLoopBreak) { return LoopAction::Break; }

                return LoopAction::Continue;
            });

            if (bShouldOuterLoopBreak) { break; }

            UStruct* NextClass = Class->GetClass();
            if (NextClass != Class)
            {
                Class = NextClass;
            }
            else
            {
                break;
            }
        }
    }

    FProperty* UStruct::FindProperty(FName PropertyName)
    {
        FProperty* PropertyFound = nullptr;
        ForEachPropertyInChain([&](FProperty* Property) {
            if (Property->GetFName() == PropertyName)
            {
                PropertyFound = Property;
                return LoopAction::Break;
            }
            else
            {
                return LoopAction::Continue;
            }
        });

        return PropertyFound;
    }

    bool UStruct::HasChildren()
    {
        if (Version::IsBelow(4, 25))
        {
            return GetChildren();
        }
        else
        {
            return GetChildProperties() || GetChildren();
        }
    }

    FProperty* UStruct::GetFirstProperty()
    {
        if (Version::IsBelow(4, 25))
        {
            // In <4.25, this is safe if a UField is a property, which 'CastField' checks
            // In <4.25, all properties are of type UField
            auto* Child = Helper::Casting::ptr_cast_deref<FField*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::XField_Children));
            return CastField<FProperty>(Child);
        }
        else
        {
            return CastField<FProperty>(GetChildProperties());
        }
    }
}