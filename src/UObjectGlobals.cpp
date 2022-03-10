#include <Unreal/UObjectGlobals.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UnrealVersion.hpp>
#include <Unreal/VersionedContainer/Container.hpp>

namespace RC::Unreal::UObjectGlobals
{
    auto setup_static_construct_object_internal_address(void* function_address) -> void
    {
        GlobalState::static_construct_object_internal.assign_address(function_address);
        GlobalState::static_construct_object_internal_deprecated.assign_address(function_address);
    }

    auto version_is_atmost(uint32_t major_p, uint32_t minor_p) -> bool
    {
        return Unreal::Version::is_atmost(major_p, minor_p);
    }

    auto static_find_object_impl([[maybe_unused]]UClass* object_class, [[maybe_unused]]UObject* in_object_package, const wchar_t* orig_in_name, [[maybe_unused]]bool exact_class) -> UObject*
    {
        UObject* found_object{};

        UObjectGlobals::ForEachUObject([&](void* obj, [[maybe_unused]]int32_t chunk_index, [[maybe_unused]]int32_t obj_index) {
            // This call to 'get_full_name' is a problem because it relies on offsets already being found
            // This function is called before offsets have been found as a way to check if required objects have been initialized
            auto obj_name = static_cast<UObject*>(obj)->get_full_name();
            auto obj_name_no_type = obj_name.substr(obj_name.find(STR(" ")) + 1);

            if (obj_name_no_type == orig_in_name)
            {
                found_object = static_cast<UObject*>(obj);
                return LoopAction::Break;
            }
            else
            {
                return LoopAction::Continue;
            }
        });

        return found_object;
    }

    auto static is_valid_object_for_find_x_of(UObject* object) -> bool
    {
        return !object->has_any_flag(static_cast<EObjectFlags>(RF_ClassDefaultObject | RF_ArchetypeObject)) && !object->is_a<UClass>();
    }

    auto find_first_of(FName class_name) -> UObject*
    {
        UObject* object_found{nullptr};

        UObjectGlobals::ForEachUObject([&](void* object, [[maybe_unused]]int32_t chunk_index, [[maybe_unused]]int32_t object_index) {
            UObject* typed_object = static_cast<UObject*>(object);
            UClass* typed_class = static_cast<UObject*>(object)->get_uclass();

            if (typed_class->get_fname().equals(class_name) && is_valid_object_for_find_x_of(typed_object))
            {
                object_found = typed_object;
                return LoopAction::Break;

            }

            if (!is_valid_object_for_find_x_of(typed_object)) { return LoopAction::Continue; }

            typed_class->for_each_super_struct([&](UStruct* super_struct) {
                if (super_struct->get_fname().equals(class_name))
                {
                    object_found = typed_object;
                    return LoopAction::Break;
                }

                return LoopAction::Continue;
            });

            return LoopAction::Continue;
        });

        return object_found;
    }

    auto find_first_of(const wchar_t* class_name) -> UObject*
    {
        return find_first_of(FName(class_name));
    }

    auto find_first_of(std::wstring_view class_name) -> UObject*
    {
        return find_first_of(FName(class_name));
    }

    auto find_first_of(const std::wstring& class_name) -> UObject*
    {
        return find_first_of(FName(class_name));
    }

    auto find_first_of(std::string_view class_name) -> UObject*
    {
        return find_first_of(FName(to_wstring(class_name)));
    }

    auto find_first_of(const std::string& class_name) -> UObject*
    {
        return find_first_of(FName(to_wstring(class_name)));
    }

    auto find_all_of(FName class_name, std::vector<UObject*>& out_storage) -> void
    {
        UObjectGlobals::ForEachUObject([&](void* object, [[maybe_unused]]int32_t chunk_index, [[maybe_unused]]int32_t object_index) {
            if (!object) { return LoopAction::Continue; }

            UObject* typed_object = static_cast<UObject*>(object);
            UClass* typed_class = static_cast<UObject*>(object)->get_uclass();

            if (typed_class->get_fname().equals(class_name) && is_valid_object_for_find_x_of(typed_object))
            {
                out_storage.emplace_back(typed_object);
                return LoopAction::Continue;
            }

            if (!is_valid_object_for_find_x_of(typed_object)) { return LoopAction::Continue; }

            typed_class->for_each_super_struct([&](UStruct* super_struct) {
                if (super_struct->get_fname().equals(class_name))
                {
                    out_storage.emplace_back(typed_object);
                    return LoopAction::Break;
                }

                return LoopAction::Continue;
            });

            return LoopAction::Continue;
        });
    }

    auto find_all_of(const wchar_t* class_name, std::vector<UObject*>& out_storage) -> void
    {
        find_all_of(FName(class_name), out_storage);
    }

    auto find_all_of(std::wstring_view class_name, std::vector<UObject*>& out_storage) -> void
    {
        find_all_of(FName(class_name), out_storage);
    }

    auto find_all_of(const std::wstring& class_name, std::vector<UObject*>& out_storage) -> void
    {
        find_all_of(FName(class_name), out_storage);
    }

    auto find_all_of(std::string_view class_name, std::vector<UObject*>& out_storage) -> void
    {
        find_all_of(FName(to_wstring(class_name)), out_storage);
    }

    auto find_all_of(const std::string& class_name, std::vector<UObject*>& out_storage) -> void
    {
        find_all_of(FName(to_wstring(class_name)), out_storage);
    }

    auto find_objects(size_t num_objects_to_find, const FName class_name, const FName object_short_name, std::vector<UObject*>& out_storage, int32 required_flags, int32 banned_flags) -> void
    {
        bool care_about_class = class_name != FName(0u, 0u);
        bool care_about_name = object_short_name != FName(0u, 0u);

        if (!care_about_class && !care_about_name)
        {
            throw std::runtime_error{"[UObjectGlobals::find_objects] Must supply class_name, object_short_name, or both"};
        }

        size_t num_objects_found{};

        ForEachUObject([&](UObject* object, int32, int32) {
            bool name_matches{};
            if (care_about_name && object->get_fname().equals(object_short_name))
            {
                name_matches = true;
            }

            bool class_matches{};
            if (care_about_class && object->get_uclass()->get_fname().equals(class_name))
            {
                class_matches = true;
            }

            if ((care_about_class && class_matches && care_about_name && name_matches) ||
                (!care_about_name && care_about_class && class_matches) ||
                (!care_about_class && care_about_name && name_matches))
            {
                bool required_flags_passes = object->has_all_flags(static_cast<EObjectFlags>(required_flags));
                bool banned_flags_passes = !object->has_any_flag(static_cast<EObjectFlags>(banned_flags));

                if (required_flags_passes && banned_flags_passes)
                {
                    out_storage.emplace_back(object);
                    ++num_objects_found;
                }
            }

            if (num_objects_to_find != 0 && num_objects_found >= num_objects_to_find)
            {
                return LoopAction::Break;
            }
            else
            {
                return LoopAction::Continue;
            }
        });
    }

    auto find_objects(size_t num_objects_to_find, const wchar_t* class_name, const wchar_t* object_short_name, std::vector<UObject*>& out_storage, int32 required_flags, int32 banned_flags) -> void
    {
        find_objects(num_objects_to_find, FName(class_name), FName(object_short_name), out_storage, required_flags, banned_flags);
    }

    auto find_objects(size_t num_objects_to_find, const wchar_t* class_name, std::vector<UObject*>& out_storage) -> void
    {
        find_objects(num_objects_to_find, FName(class_name), FName(0u, 0u), out_storage);
    }

    auto find_object(const FName class_name, const FName object_short_name, int32 required_flags, int32 banned_flags) -> UObject*
    {
        std::vector<UObject*> found_object{};
        find_objects(1, class_name, object_short_name, found_object, required_flags, banned_flags);

        if (found_object.empty())
        {
            return nullptr;
        }
        else
        {
            return found_object[0];
        }
    };

    auto find_objects(const FName class_name, const FName object_short_name, std::vector<UObject*>& out_storage, int32 required_flags, int32 banned_flags) -> void
    {
        find_objects(0, class_name, object_short_name, out_storage, required_flags, banned_flags);
    }

    auto find_objects(const wchar_t* class_name, const wchar_t* object_short_name, std::vector<UObject*>& out_storage, int32 required_flags, int32 banned_flags) -> void
    {
        find_objects(0, class_name, object_short_name, out_storage, required_flags, banned_flags);
    }

    auto find_object(const wchar_t* class_name, const wchar_t* object_short_name, int32 required_flags, int32 banned_flags) -> UObject*
    {
        return find_object(FName(class_name), FName(object_short_name), required_flags, banned_flags);
    }

    auto ForEachUObject(const std::function<LoopAction(UObject*, int32, int32)>& callable) -> void
    {
        Container::m_unreal_vc_base->UObjectArray_for_each_uobject([&](void* raw_object, int32 object_index, int32 chunk_index)
        {
            return callable(static_cast<UObject*>(raw_object), object_index, chunk_index);
        });
    };
}

