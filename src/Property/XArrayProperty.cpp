#include <Unreal/Property/XArrayProperty.hpp>

namespace RC::Unreal
{
    auto XArrayProperty::get_inner() -> FProperty*
    {
        return Helper::Casting::offset_deref<FProperty*>(this, StaticOffsetFinder::retrieve_static_offset(StaticOffsetFinder::MemberOffsets::ArrayProperty_Inner));
    }

    auto XArrayProperty::get_inner(PropertyDataVC data) -> FProperty*
    {
        return Helper::Casting::offset_deref<FProperty*>(data.property_ptr, StaticOffsetFinder::retrieve_static_offset(StaticOffsetFinder::MemberOffsets::ArrayProperty_Inner));
    }

    auto XArrayProperty::get_inner(XArrayProperty* p_this) -> FProperty*
    {
        return Helper::Casting::offset_deref<FProperty*>(p_this, StaticOffsetFinder::retrieve_static_offset(StaticOffsetFinder::MemberOffsets::ArrayProperty_Inner));
    }

    auto XArrayProperty::read_data(PropertyDataVC data) -> InnerType
    {
        UntypedTArray untyped_array = Helper::Casting::offset_deref<UntypedTArray>(data.data_ptr, 0x0);

        if (!untyped_array.elements_ptr)
        {
            // If the data ptr is nullptr that means the array is empty and capacity is also zero
            return InnerType{nullptr, 0, 0};
        }
        else
        {
            printf_s("elements_ptr: %p\n", untyped_array.elements_ptr);
            return InnerType{untyped_array.elements_ptr, untyped_array.num_elements, untyped_array.max_elements};
        }
    }

    auto XArrayProperty::to_string(void* p_this, std::wstring& out_line) -> void
    {
        trivial_dump_to_string(p_this, out_line);

        XArrayProperty* p_typed_this = static_cast<XArrayProperty*>(p_this);
        out_line.append(std::format(L" [ai: {:016X}]", reinterpret_cast<uintptr_t>(get_inner(p_typed_this))));
    }

    auto XArrayProperty::to_string_complex(void* p_this, std::wstring& out_line, const std::function<void(void*)>& callable) -> void
    {
        FProperty* array_inner = get_inner(static_cast<XArrayProperty*>(p_this));
        auto* array_inner_class = array_inner->get_ffieldclass();

        if (Unreal::TypeChecker::to_string_exists(array_inner_class))
        {
            Unreal::TypeChecker::get_to_string(array_inner_class)(array_inner, out_line);

            if (Unreal::TypeChecker::to_string_complex_exists(array_inner_class))
            {
                // If this code is executed then we'll be having another line before we return to the dumper, so we need to explicitly add a new line
                // If this code is not executed then we'll not be having another line and the dumper will add the new line
                out_line.append(L"\n");

                Unreal::TypeChecker::get_to_string_complex(array_inner_class)(array_inner, out_line, [&]([[maybe_unused]]void* prop) {
                    // It's possible that a new line is supposed to be appended here
                });
            }

            callable(array_inner);
        }
        else
        {
            array_inner->get_full_name(out_line);
            callable(array_inner);
        }
    }
}
