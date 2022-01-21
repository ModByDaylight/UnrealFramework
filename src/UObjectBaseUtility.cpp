#include <Unreal/UObjectBaseUtility.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/FString.hpp>
#include <Unreal/TypeChecker.hpp>
#include <Timer/ScopedTimer.hpp>

namespace RC::Unreal
{
    auto UObjectBaseUtility::get_path_name_recursive(const UObject* object, const void* stop_outer, std::wstring& out_path) -> void
    {
        if (object == stop_outer || !object)
        {
            out_path = L"None";
        }
        else
        {
            UObject* outer = object->get_outer();
            if (outer && outer != stop_outer)
            {
                get_path_name_recursive(outer, stop_outer, out_path);

                if (outer->get_uclass() &&
                    outer->get_uclass()->get_fname() != TypeChecker::m_core_package_name &&
                    outer->get_outer() &&
                    outer->get_outer()->get_uclass()->get_fname() == TypeChecker::m_core_package_name)
                {
                    out_path.append(L":");
                }
                else
                {
                    out_path.append(L".");
                }
            }
            out_path.append(object->get_name());
        }
    }

    auto UObjectBaseUtility::get_path_name(const UObject* object, const void* stop_outer, std::wstring& out_path) -> void
    {
        std::wstring path;
        path.reserve(200);

        get_path_name_recursive(object, stop_outer, path);
        out_path.append(path);
    }

    auto UObjectBaseUtility::get_full_name(const UObject* object, std::wstring& out_name, const void* stop_outer) -> void
    {
        if (!object)
        {
            out_name = L"None";
        }
        else
        {
            out_name.append(object->get_uclass()->get_name() + L' ');
            get_path_name(object, stop_outer, out_name);
        }
    }

    auto UObjectBaseUtility::get_full_name(const void* stop_outer) -> std::wstring
    {
        std::wstring name;
        name.reserve(800);

        get_full_name(static_cast<const UObject*>(this), name, stop_outer);
        return name;
    }

    auto UObjectBaseUtility::get_full_name(std::wstring& out_name, const void* stop_outer) -> void
    {
        get_full_name(static_cast<const UObject*>(this), out_name, stop_outer);
    }
}