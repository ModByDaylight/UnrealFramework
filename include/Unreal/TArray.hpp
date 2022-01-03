#ifndef RC_UNREAL_TARRAY_HPP
#define RC_UNREAL_TARRAY_HPP

#include <functional>
#include <stdexcept>
#include <type_traits>

#include <Constructs/Loop.hpp>
#include <Helpers/Format.hpp>

#include <Unreal/Common.hpp>
#include <Unreal/VersionedContainer/Base.hpp>
#include <Unreal/Property/XArrayProperty.hpp>
#include <Unreal/Property/XStructProperty.hpp>

namespace RC::Unreal
{
    class XStructData;

    template<typename ArrayInnerType>
    constexpr bool ArraySizeIsImplicit = !(std::is_same_v<ArrayInnerType, struct FAssetData>);

    auto RC_API get_struct_data_at(void* data_param, size_t index) -> XStructData*;

    template<typename ArrayInnerType>
    class TArray
    {
    public:
        constexpr static inline InternalType internal_type = InternalType::Array;

    protected:
        ArrayInnerType* data{};
        int32_t array_num{};
        int32_t array_max{};

    public:
        TArray(ArrayInnerType* data_ptr, int32_t current_size, int32_t capacity) : data(data_ptr), array_num(current_size),
                                                                                   array_max(capacity) {}

    public:
        auto static read_data(PropertyDataVC data) -> TArray<ArrayInnerType>
        {
            UntypedTArray untyped_array = XArrayProperty::read_data(data);

            if constexpr (std::is_same_v<ArrayInnerType, XStructData>)
            {
                // Doing some cheating here, we're storing an XStructData object here instead of an actual data ptr
                // The data ptr can be found inside the XStructData object

                XProperty* inner = XArrayProperty::get_inner(data);
                if (!inner)
                {
                    throw std::runtime_error{"[TArray::read_data] Inner is nullptr"};
                }

                return TArray<ArrayInnerType>(
                        XStructProperty::construct(data, inner, untyped_array.elements_ptr),
                        untyped_array.num_elements,
                        untyped_array.max_elements
                );
            }
            else
            {
                return TArray<ArrayInnerType>(
                        static_cast<ArrayInnerType*>(untyped_array.elements_ptr),
                        untyped_array.num_elements,
                        untyped_array.max_elements
                );
            }
        }

        auto get_data_ptr() const -> ArrayInnerType*
        {
            return data;
        }

        auto set_data_ptr(ArrayInnerType* new_data_ptr) -> void
        {
            data = new_data_ptr;
        }

        auto get_array_num() const -> int32_t
        {
            return array_num;
        }

        auto set_array_num(int32_t new_array_num) -> void
        {
            array_num = new_array_num;
        }

        auto get_array_max() const -> int32_t
        {
            return array_max;
        }

        auto set_array_max(int32_t new_array_max) -> void
        {
            array_max = new_array_max;
        }

        auto copy_fast(const TArray<ArrayInnerType>& other) -> void
        {
            data = other.data;
            array_num = other.array_num;
            array_max = other.array_max;
        }

        auto at(size_t index) -> ArrayInnerType*
        {
            // Index is zero-based and the stored max is one-based
            if (get_array_num() == 0 || index > get_array_num() - 1)
            {
                throw std::runtime_error{fmt("[TArrayImpl::at] out of range (num elements: %d | requested elem: %d)", (get_array_num() - 1 < 0 ? 0 : get_array_num() - 1), index)};
            }

            // Need to know the type here so that we can calculate the location of each array element
            // TODO: Come up with a better solution. TArray shouldn't need to know about StructProperty or XStructData.
            // I've hardcoded the StructProperty implementation for now but at least it's constexpr.
            if constexpr (std::is_same_v<ArrayInnerType, class XStructData>)
            {
                return get_struct_data_at(get_data_ptr(), index);
            }
            else
            {
                if constexpr (ArraySizeIsImplicit<ArrayInnerType>)
                {
                    // This is for trivial types
                    // The data ptr for trivial types is directly mapped to the game memory
                    // The type is also exactly ArrayInnerType so it's safe to use operator[]
                    return &data[index];
                }
                else
                {
                    // This is for non-trivial types
                    return reinterpret_cast<ArrayInnerType*>(reinterpret_cast<uintptr_t>(data) + (index * ArrayInnerType::static_class()->get_size()));
                }
            }
        }

        auto operator[](size_t index) -> ArrayInnerType*
        {
            return static_cast<ArrayInnerType*>(at(index));
        }

    private:
        template<typename Callable>
        auto for_each_internal(Callable callable) -> void
        {
            for (size_t i = 0; i < get_array_num(); ++i)
            {
                if (callable(at(i), i) == LoopAction::Break) { break; }
            }
        }

    public:
        using TArrayForEachCallableAllParamsImpl = const std::function<LoopAction(ArrayInnerType*, size_t)>&;
        auto for_each(TArrayForEachCallableAllParamsImpl callable) -> void
        {
            for_each_internal([&](auto elem, auto index) {
                return callable(elem, index);
            });
        }

        using TArrayForEachCallableParamElemOnlyImpl = const std::function<LoopAction(ArrayInnerType*)>&;
        auto for_each(TArrayForEachCallableParamElemOnlyImpl callable) -> void
        {
            for_each_internal([&](auto elem, [[maybe_unused]]auto index) {
                return callable(elem);
            });
        }
    };
}


#endif //RC_UNREAL_TARRAY_HPP
