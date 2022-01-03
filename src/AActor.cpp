#include <Unreal/AActor.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UObjectArray.hpp>
#include <Unreal/UFunction.hpp>

namespace RC::Unreal
{
    auto AActor::get_level() -> UObject*
    {
        UObject* outer = get_outer();
        if (!outer) { return nullptr; }

        if (!has_any_flag(Unreal::RF_ClassDefaultObject) && !outer->has_any_flag(Unreal::RF_BeginDestroyed) && !outer->get_object_item()->is_unreachable())
        {
            while (outer)
            {
                // TODO: Shouldn't need to specify the 'UObject::' here
                // XField needs to be split to UField and FField and then this problem should go away
                if (outer->get_uclass()->get_fname() == TypeChecker::get_level_name())
                {
                    return outer;
                }

                outer = outer->get_outer();
            }
        }

        return nullptr;
    }

    auto AActor::get_world() -> UObject*
    {
        UObject* outer = get_outer();
        if (!outer) { return nullptr; }

        if (!has_any_flag(Unreal::RF_ClassDefaultObject) && !outer->has_any_flag(Unreal::RF_BeginDestroyed) && !outer->get_object_item()->is_unreachable())
        {
            while (outer)
            {
                // TODO: Shouldn't need to specify the 'UObject::' here
                // XField needs to be split to UField and FField and then this problem should go away
                if (outer->get_uclass()->get_fname() == TypeChecker::get_world_name())
                {
                    return outer;
                }

                outer = outer->get_outer();
            }
        }

        return nullptr;
    }

    auto AActor::K2_TeleportTo(FVector DestLocation, FRotator DestRotation) -> bool
    {
        UFunction* function = UObjectGlobals::static_find_object<UFunction*>(nullptr, nullptr, L"/Script/Engine.Actor:K2_TeleportTo");

        struct K2_TeleportTo_Params
        {
            FVector DestLocation;
            FRotator DestRotation;
            bool ReturnValue;
        };
        K2_TeleportTo_Params params{
            .DestLocation = DestLocation,
            .DestRotation = DestRotation,
        };

        process_event(function, &params);
        return params.ReturnValue;
    }

    auto AActor::K2_GetActorLocation() -> FVector
    {
        UFunction* function = UObjectGlobals::static_find_object<UFunction*>(nullptr, nullptr, L"/Script/Engine.Actor:K2_GetActorLocation");

        struct K2_GetActorLocation_Params
        {
            FVector ReturnValue;
        };
        K2_GetActorLocation_Params params{};

        process_event(function, &params);
        return params.ReturnValue;
    }

    auto AActor::GetActorForwardVector() -> FVector
    {
        UFunction* function = UObjectGlobals::static_find_object<UFunction*>(nullptr, nullptr, L"/Script/Engine.Actor:GetActorForwardVector");

        struct GetActorForwardVector_Params
        {
            FVector ReturnValue;
        };
        GetActorForwardVector_Params params{};

        process_event(function, &params);
        return params.ReturnValue;
    }
}
