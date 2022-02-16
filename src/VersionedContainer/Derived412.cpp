#include <Unreal/VersionedContainer/Derived412.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/UStruct.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/FField.hpp>
#include <Unreal/FProperty.hpp>
#include <Unreal/CustomType.hpp>

namespace RC::Unreal
{
    auto Derived412::Field_get_type_fname(const FField* p_this) -> FName
    {
        // Since this code is only executed in <4.25, before FField existed,
        // this reinterpret_cast should not be undefined behavior
        UClass* uclass = reinterpret_cast<const UField*>(p_this)->get_uclass();
        return uclass->get_fname();
    }

    auto Derived412::Field_get_ffield_owner([[maybe_unused]]const FField* p_this) -> void*
    {
        // FField does not exist before 4.25 so therefor FField::Owner also does not exist
        // It's up to the caller to make sure the returned value isn't nullptr
        return nullptr;
    }

    auto Derived412::FFieldClass_get_fname(const FFieldClass* p_this) -> FName
    {
        return Helper::Casting::offset_deref<FName>(p_this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UObject_NamePrivate));
    }

    auto Derived412::find_property_vc(void* uobject, const wchar_t* property_string, Base::WithSideEffects with_side_effects, ExcludeSelf exclude_self) -> PropertyDataVC
    {
        Unreal::UObject* uobject_typed = static_cast<Unreal::UObject*>(uobject);
        UStruct* uobject_uclass = exclude_self == ExcludeSelf::Yes ? uobject_typed->get_uclass() : static_cast<UStruct*>(uobject);
        if (!uobject_uclass) { throw std::runtime_error{"[UObjectImpl::read_property] UObject was nullptr"}; }

        FProperty* property{};

        FName property_name(property_string);

        uobject_uclass->for_each_child_in_chain<FProperty*>([&](auto* child) {
            if (child->GetFName() == property_name)
            {
                property = child;
                return LoopAction::Break;
            }
            else
            {
                return LoopAction::Continue;
            }
        });

        if (with_side_effects == WithSideEffects::Yes)
        {
            m_current_property = property;
        }

        return {.property_ptr = property, .uobject_ptr = uobject};
    }

    auto Derived412::read_property_vc(void* uobject, const wchar_t* property_string, CustomProperty* custom_property) -> PropertyDataVC
    {
        FProperty* property{};
        int32_t offset_internal;

        bool is_custom{false};
        if (custom_property)
        {
            is_custom = true;
            offset_internal = custom_property->get_offset_for_internal();
            property = custom_property;
        }
        else
        {
            if (!m_current_property)
            {
                find_property_vc(uobject, property_string, WithSideEffects::Yes);
            }

            property = m_current_property;

            // Return nullptr on failure (the failure should be dealt with silently by the caller)
            // Maybe make another variant that guarantees success (or throws exception)
            if (!property) { return {}; }

            offset_internal = property->get_offset_for_internal();
        }

        void* data = static_cast<void*>(static_cast<char*>(uobject) + offset_internal);

        m_current_property = nullptr;

        return {.property_ptr = property, .uobject_ptr = uobject, .data_ptr = data, .is_custom = is_custom};
    }
}
