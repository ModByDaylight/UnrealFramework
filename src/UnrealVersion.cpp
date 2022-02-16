#include <Unreal/UnrealVersion.hpp>
#include <Unreal/UClass.hpp>

namespace RC::Unreal
{
    auto Version::setup_fallback() -> void
    {
        // TODO: Fix this up incase it doesn't work in future versions of Unreal Engine
        //UObject* prop = UObjectGlobals::static_find_object(nullptr, nullptr, L"/Script/CoreUObject.Vector:X");
        UObject* prop{};
        FName vector_name = FName(STR("Vector"));
        FName x_name = FName(STR("X"));
        UObjectGlobals::ForEachUObject([&](void* obj, [[maybe_unused]]int32_t chunk_index, [[maybe_unused]]int32_t obj_index) {
            UObject* typed_obj = static_cast<UObject*>(obj);
            // I would prefer to use a helper function here but get_fname() and get_uclass() are the only API functions available at this point
            FName obj_name = typed_obj->get_fname();
            FName obj_class_name = typed_obj->get_uclass()->get_fname();

            if (obj_name.equals(vector_name) && obj_class_name.equals(x_name))
            {
                prop = typed_obj;
                return LoopAction::Break;
            }
            else
            {
                return LoopAction::Continue;
            }
        });

        if (prop)
        {
            // Assume it's <4.25 if property exists in GUObjectArray
            major = 4;
            minor = 24;
        }
        else
        {
            // Assume it's >=4.25 if the property is not in GUObjectArray
            major = 4;
            minor = 25;
        }
    }
}
