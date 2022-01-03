#ifndef RC_UNREAL_UENUM_HPP
#define RC_UNREAL_UENUM_HPP

#include <format>

#include <Unreal/Common.hpp>
#include <Unreal/FField.hpp>
#include <Unreal/TypeChecker.hpp>
#include <Unreal/UField.hpp>
#include <Unreal/TArray.hpp>
#include <Unreal/TPair.hpp>

namespace RC::Unreal
{
    using EnumNamesPair = TPair<FName, int64_t>;
    using EnumNamesArray = TArray<EnumNamesPair>;

    class RC_API UEnum : public UField
    {
    private:
        static inline class UClass* m_static_obj{};

    public:
        // DO NOT USE... MEANT FOR INTERNAL USE ONLY
        auto static set_static_obj_ptr(class UClass* ptr)
        {
            m_static_obj = ptr;
        }

        auto static static_class() -> class UClass*
        {
            if (!m_static_obj) { throw std::runtime_error{"[UEnum::get_static_obj_ptr] m_static_obj is nullptr"}; }

            return m_static_obj;
        }

    public:
        enum class ECppForm
        {
            Regular,
            Namespaced,
            EnumClass
        };

        // EEnumFlags, up-to-date with UE 5.0 EA
        enum class EEnumFlags
        {
            None,
            Flags = 0x00000001,
            NewerVersionExists = 0x00000002,
        };

    public:
        auto get_enum_names() -> EnumNamesArray
        {
            return Helper::Casting::offset_deref<EnumNamesArray>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UEnum_Names));
        }

        auto get_cpp_form() -> ECppForm
        {
            // The offset of 'Names' + the size of 'Names' = offset of 'CppForm'
            return Helper::Casting::offset_deref<ECppForm>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UEnum_Names) + sizeof(EnumNamesArray));
        }

        auto get_enum_flags() -> EEnumFlags
        {
            if (Version::is_below(4, 26))
            {
                // EEnumFlags was added in 4.26
                // Let's assume the 'None' flag is valid for whatever code calls this function in <4.26
                return EEnumFlags::None;
            }
            else
            {
                // The offset of 'Names' + the size of 'Names' + the size of 'CppForm' = offset of 'EnumFlags'
                return Helper::Casting::offset_deref<EEnumFlags>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UEnum_Names) + sizeof(EnumNamesArray) + sizeof(ECppForm));
            }
        }

    public:
        auto static to_string(void* p_this, std::wstring& out_line) -> void
        {
            trivial_dump_to_string(p_this, out_line);

            auto* typed_this = static_cast<UEnum*>(p_this);

            auto enum_names = typed_this->get_enum_names();
            enum_names.for_each([&](EnumNamesPair* elem) {
                out_line.append(std::format(L"\n[{:016X}] {} [n: {:X}] [v: {}]", reinterpret_cast<uintptr_t>(elem), elem->key.to_string(), elem->key.get_comparison_index(), static_cast<uint8_t>(elem->value)));
                return LoopAction::Continue;
            });
        }
    };

    class RC_API UUserDefinedEnum : public UEnum
    {
    private:
        static inline class UClass* m_static_obj{};

    public:
        // DO NOT USE... MEANT FOR INTERNAL USE ONLY
        auto static set_static_obj_ptr(class UClass* ptr)
        {
            m_static_obj = ptr;
        }

        auto static static_class() -> class UClass*
        {
            if (!m_static_obj) { throw std::runtime_error{"[UUserDefinedEnum::get_static_obj_ptr] m_static_obj is nullptr"}; }

            return m_static_obj;
        }
    };

    template<typename InnerType>
    class TUEnum : public UEnum
    {
    public:
        auto static read_data(const PropertyDataVC& data) -> InnerType*
        {
            return static_cast<InnerType*>(data.data_ptr);
        }
    };
}


#endif //RC_UNREAL_UENUM_HPP
