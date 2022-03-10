#ifndef RC_UNREAL_TMAP_HPP
#define RC_UNREAL_TMAP_HPP

#include <Constructs/Loop.hpp>

#include <Unreal/Common.hpp>
#include <Unreal/TypeChecker.hpp>
#include <Unreal/VersionedContainer/Base.hpp>
#include <Unreal/Property/FMapProperty.hpp>

namespace RC::Unreal
{
    template<typename MapInnerKeyType, typename MapInnerValueType>
    class TMapElement
    {
    public:
        MapInnerKeyType key;
        MapInnerValueType value;
        int64_t padding; // Figure out what this is later
    };

    template<typename MapInnerKeyType, typename MapInnerValueType>
    class TMap
    {
    public:
        constexpr static inline InternalType internal_type = InternalType::Map;
        using MapElement = TMapElement<MapInnerKeyType, MapInnerValueType>;

    protected:
        MapElement* elements{};
        int32_t map_num{};
        int32_t map_max{};

    public:
        TMap(MapElement* elements_ptr, int32_t current_size, int32_t capacity) : elements(elements_ptr), map_num(current_size), map_max(capacity) {}

    protected:
        template<typename Callable>
        auto for_each(Callable callable) -> void
        {
            if (!elements) { return; }

            for (int32_t i = 0; i < get_map_num(); ++i)
            {
                if (callable(&elements[i]) == LoopAction::Break)
                {
                    break;
                }
            }
        }

    public:
        auto get_map_num() -> int32_t
        {
            return map_num;
        }

        auto get_map_max() -> int32_t
        {
            return map_max;
        }

        auto get_elements_ptr() -> MapElement*
        {
            return elements;
        }

        auto at(MapInnerKeyType key) -> MapInnerValueType*
        {
            // Map is empty, figure out what to do here
            if (get_map_num() == 0)
            {

            }

            MapInnerValueType* found_value{};

            for_each([&](MapElement* element) {
                if (element->key == key)
                {
                    found_value = &element->value;
                    return LoopAction::Break;
                }
                else
                {
                    return LoopAction::Continue;
                }
            });

            return found_value;
        }

        auto operator[](MapInnerKeyType key) -> MapInnerValueType*
        {
            return static_cast<MapInnerValueType*>(at(key));
        }
    };
}


#endif //RC_UNREAL_TMAP_HPP
