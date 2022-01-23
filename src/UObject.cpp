#include <Unreal/TypeChecker.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/VersionedContainer/Container.hpp>
#include <Unreal/UPackage.hpp>

namespace RC::Unreal
{
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UObject);

    using MemberOffsets = ::RC::Unreal::StaticOffsetFinder::MemberOffsets;

    auto UObject::get_uclass() const -> UClass*
    {
        return Helper::Casting::offset_deref<UClass*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UObject_ClassPrivate));
    }

    auto UObject::get_outer() -> UObject*
    {
        return Helper::Casting::offset_deref<UObject*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UObject_OuterPrivate));
    }

    auto UObject::get_fname() -> FName
    {
        return Helper::Casting::offset_deref<FName>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::UObject_NamePrivate));
    }

    auto UObject::get_flags() -> EObjectFlags
    {
        return Container::m_unreal_object_base->UObject_get_flags(this);
    }

    auto UObject::set_flags_to(EObjectFlags new_flag) -> void
    {
        Container::m_unreal_object_base->UObject_set_flags(this, new_flag);
    }

    auto UObject::get_internal_index() -> uint32_t
    {
        return Container::m_unreal_object_base->UObject_get_internal_index(this);
    }

    auto UObject::get_object_item() -> FUObjectItem*
    {
        return static_cast<FUObjectItem*>(Container::m_unreal_vc_base->UObjectArray_index_to_object(get_internal_index()));
    }

    auto UObject::process_event(UFunction* function, void* params) -> void
    {
        process_event_internal(this, function, params);
    }

    auto UObject::get_outermost() -> UObject* {
        UObject* current_object = this;
        while (current_object->get_outer())
        {
            current_object = current_object->get_outer();
        }
        return current_object;
    }

    auto UObject::is_a(UClass* uclass) -> bool {
        return get_uclass()->is_child_of(uclass);
    }

    auto UObject::get_path_name(UObject *stop_outer) -> std::wstring
    {
        std::wstring result_name;
        get_path_name(stop_outer, result_name);
        return result_name;
    }

    auto UObject::get_path_name(UObject* stop_outer, std::wstring& result_string) -> void
    {
        if(this != stop_outer && this != nullptr)
        {
            UObject* outer = get_outer();

            if (outer && outer != stop_outer)
            {
                outer->get_path_name(stop_outer, result_string);

                // SUBOBJECT_DELIMITER_CHAR is used to indicate that this object's outer is not a UPackage
                if (outer->get_uclass() != UPackage::static_class()
                    && outer->get_outer()->get_uclass() == UPackage::static_class())
                {
                    result_string.append(STR(":"));
                }
                else
                {
                    result_string.append(STR("."));
                }
            }
            result_string.append(get_name());
        }
        else
        {
            result_string.append(STR("None"));
        }
    }

    auto UObject::get_full_name(UObject* stop_outer) -> std::wstring
    {
        if (this == nullptr)
        {
            return STR("None");
        }
        else
        {
            std::wstring out_name;
            out_name.append(get_uclass()->get_name() + STR(" "));
            get_path_name(stop_outer, out_name);
            return out_name;
        }
    }
}