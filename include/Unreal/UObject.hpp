#ifndef RC_UNREAL_UOBJECT_HPP
#define RC_UNREAL_UOBJECT_HPP

#include <array>
#include <stdexcept>

#include <Helpers/Casting.hpp>
#include <Helpers/Format.hpp>

#include <Unreal/Common.hpp>
#include <Unreal/VersionedContainer/Container.hpp>
#include <Unreal/StaticOffsetFinder.hpp>
#include <Unreal/UObjectBaseUtility.hpp>
#include <Unreal/NameTypes.hpp>
#include <Unreal/UnrealFlags.hpp>
#include <Unreal/PropertyReaders.hpp>
#include <Unreal/TypeChecker.hpp>

namespace RC::Unreal
{
    class UClass;
    class XStructData;
    class UEnum;
    template<typename InnerType>
    class TUEnum;

    // Temporary location for the FVector & FRotator structs
    struct RC_API FVector
    {
        float X;
        float Y;
        float Z;
    };

    struct RC_API FRotator
    {
        float Pitch;
        float Yaw;
        float Roll;
    };

    // TODO: Replace this enum with actual types
    // These types are used when speed is important
    // To remain sane I will only add core types such as U/FProperty, IntProperty, ObjectProperty, etc
    enum class EUObjectType
    {
        Default,
        TestOne,
        TestTwo,
        TestThree,
    };

    class RC_API UObjectType
    {
    private:
        class UObject* m_object{};

    public:
        explicit UObjectType(class UObject* object) : m_object(object) {};

        [[nodiscard]] auto as_string() -> std::wstring;
        [[nodiscard]] auto as_enum() -> EUObjectType;
        [[nodiscard]] auto as_fname() -> FName;
    };

    class RC_API UObject : public UObjectBaseUtility
    {
    public:
        using MemberOffsets = ::RC::Unreal::StaticOffsetFinder::MemberOffsets;
        using ProcessEventSignature = void(*)(UObject* p_this, class UFunction*, void* parms);
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
            if (!m_static_obj) { throw std::runtime_error{"[UObject::get_static_obj_ptr] m_static_obj is nullptr"}; }

            return m_static_obj;
        }

    public:
        auto get_internal_index() -> uint32_t
        {
            // TODO: Automate this offset
            return Helper::Casting::offset_deref<uint32_t>(this, 0xC);
        }

        [[nodiscard]] auto get_uclass() const -> UClass*;

        auto get_outer() -> UObject*;
        auto get_outer() const -> UObject*;
        auto get_fname() -> FName;
        auto get_fname() const -> FName;
        auto get_type() -> UObjectType;
        auto get_name() -> std::wstring;
        auto get_name() const -> std::wstring;
        auto is_a(UClass* uclass) -> bool;

        auto get_flags() const -> int32_t;
        auto set_flags(const EObjectFlags new_flag) -> void;
        auto set_flags(const std::array<EObjectFlags, EObjectFlags_Max>& new_flags) -> void;
        auto has_any_flag(const EObjectFlags flags_to_check) -> bool;
        auto has_any_flag(const std::array<EObjectFlags, EObjectFlags_Max>& flags_to_check) -> bool;
        auto has_all_flags(const EObjectFlags flags_to_check) -> bool;
        auto has_all_flags(const std::array<EObjectFlags, EObjectFlags_Max>& flags_to_check) -> bool;

        auto get_object_item() -> struct FUObjectItem*;

        auto is_function() -> bool;
        auto is_derived_from_struct() -> bool;
        auto is_script_struct() -> bool;
        auto is_actor() -> bool;
        auto is_widget() -> bool;
        auto is_class() -> bool;
        auto is_any_class() -> bool;
        auto is_enum() -> bool;
        auto get_name_prefix() -> File::StringType;

        template<UObjectGlobals::UObjectDerivative UObjectDerivedType>
        auto is_a() -> bool
        {
            return is_a(UObjectDerivedType::static_class());
        }

        auto get_outermost() -> UObject*
        {
            UObject* current_object = this;
            while (current_object->get_outer())
            {
                current_object = current_object->get_outer();
            }
            return current_object;
        }

        template<typename Callable>
        auto static for_each_uobject(Callable callable) -> void
        {
            Container::m_unreal_vc_base->UObjectArray_for_each_uobject(callable);
        }

        auto static is_real_object(const void* p_this) -> bool
        {
            bool object_was_found{};

            for_each_uobject([&](void* obj, [[maybe_unused]]int32_t chunk_index, [[maybe_unused]]int32_t obj_index) {
                if (p_this == obj)
                {
                    object_was_found = true;
                    return LoopAction::Break;
                }

                return LoopAction::Continue;
            });

            return object_was_found;
        }

        auto get_property_data_ptr(const wchar_t* property_string) -> void*
        {
            PropertyDataVC data = Container::m_unreal_vc_base->read_property_vc(this, property_string);
            return data.data_ptr;
        }

        /**
         * Attempts to find a property
         * Can have the side effect of setting up 'get_property()'
         * This is useful if you want the XProperty* but also want to rely on the system to get the value
         * @param property_name
         * @param with_side_effects Whether or not you want side effects (No by default & is recommended)
         * @return Pointer to property or nullptr
         */
        auto find_property(FName, Base::WithSideEffects = Base::WithSideEffects::No, ExcludeSelf = ExcludeSelf::Yes) -> XProperty*;

        auto get_property_internal(const wchar_t* property_string, class CustomProperty* custom_property = nullptr) -> PropertyDataVC;

        template<typename PropertyType>
        auto get_property(const wchar_t* property_string, class CustomProperty* custom_property = nullptr) -> auto
        {
            // TODO: Get XStructData working properly
            // For now you'll have to use 'XStructProperty' instead of 'XStructData' when calling 'get_property'
            // The problem is that 'XStructData' is incomplete in UObject.hpp and it cannot be included due to circ-inclusion

            if constexpr (std::is_same_v<PropertyType, class XStructProperty>)
            {
                return PropertyType::read_data(get_property_internal(property_string, custom_property));
            }
            else if constexpr (std::is_enum_v<PropertyType>)
            {
                return TUEnum<PropertyType>::read_data(get_property_internal(property_string, custom_property));
            }
            else if constexpr (PropertyType::internal_type == InternalType::Map ||
                               PropertyType::internal_type == InternalType::Array)
            {
                return PropertyType::read_data(get_property_internal(property_string, custom_property));
            }
            else
            {
                static_assert(false, "never hit");
            }
        }

        // Specializations for different property types
        // Used to translate a standard type to XProperty so that the correct "read_data" function can be called
        // This enables this syntax: get_property<int32_t>(L"SomeIntVar")
        // You still have to use the old syntax for StructProperty: get_property<XStructProperty>(L"SomeStructVar")
        REGISTER_ALL_PROPERTY_READERS(get_property, REGISTER_UOBJECT_PROPERTY_READER)

        // To be used for any non-trivial properties that don't have their own special setter (for example operator[])
        // For trivial properties simply deref and set normally
        template<typename PropertyType>
        auto set_property_value(const wchar_t* property_string, typename PropertyType::InnerType new_data) -> void
        {
            if (PropertyType::is_trivial)
            {
                throw std::runtime_error{
                        "[UObjectImpl::write_property_data_impl] Tried to use 'write_property_data_impl' for a trivial property."};
            }

            PropertyDataVC data = Container::m_unreal_vc_base->read_property_vc(this, property_string);

            if (!data.data_ptr) { throw std::runtime_error{"[UObjectImpl::write_property_data_impl] Data pointer is nullptr"}; }

            PropertyType::write_data(data, new_data);
        }

        template<typename PropertyType>
        auto get_property_object(const wchar_t* property_string) -> void*
        {
            return Container::m_unreal_vc_base->read_property_vc(this, property_string).property_ptr;
        }

        static inline Function<void(UObject* p_this, class UFunction*, void* parms)> process_event_internal;
        auto process_event(class UFunction*, void* parms) -> void;

        static auto trivial_dump_to_string(void* p_this, std::wstring& out_line, const wchar_t* post_delimiter = L".") -> void;
        static auto to_string(void* p_this, std::wstring& out_line) -> void;
    };

    template<UObjectGlobals::UObjectDerivative CastResultType>
    auto cast_uobject(UObject* object) -> CastResultType*
    {
        return object != nullptr && object->is_a<CastResultType>() ? static_cast<CastResultType*>(object) : nullptr;
    }
}


#endif //RC_UNREAL_UOBJECT_HPP
