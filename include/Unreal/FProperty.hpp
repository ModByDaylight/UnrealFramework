#ifndef RC_UNREAL_XPROPERTY_HPP
#define RC_UNREAL_XPROPERTY_HPP

#include <Unreal/FField.hpp>

namespace RC::Unreal
{
    class RC_UE_API FProperty : public FField
    {
    private:
        static FFieldClassVariant m_static_obj;
    public:
        auto static static_class() -> FFieldClassVariant;

        auto get_array_dim() -> int32_t;
        auto get_element_size() -> int32_t;
        auto get_property_flags() -> EPropertyFlags;

        auto get_offset_for_internal() -> int32_t;
        auto get_rep_notify_func() -> FName;

        inline auto has_any_property_flags(uint64_t flags_to_check) -> bool
        {
            return (get_property_flags() & flags_to_check) != 0 || flags_to_check == CPF_AllFlags;
        }

        inline auto has_all_property_flags(uint64_t flags_to_check) -> bool
        {
            return ((get_property_flags() & flags_to_check) == flags_to_check);
        }

        template<typename T>
        auto container_ptr_to_value_ptr(void* container, int32_t array_index = 0) -> T*
        {
            return reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(container) + get_offset_for_internal() + get_element_size() * array_index);
        }
    };
}


#endif //RC_UNREAL_XPROPERTY_HPP
