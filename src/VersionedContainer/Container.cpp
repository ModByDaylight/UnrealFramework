#include <Unreal/VersionedContainer/Container.hpp>
#include <Unreal/UnrealVersion.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual412.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual422.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual426.hpp>

namespace RC::Unreal
{
    auto Container::set_derived_base_objects() -> void
    {
        if (Version::is_equal(4, 12))
        {
            m_unreal_vc_base = std::make_unique<Derived412>();
            m_unreal_object_base = std::make_unique<UObject412>();
            m_unreal_virtual_base = std::make_unique<UnrealVirtual412>();
        }
        else if (Version::is_equal(4, 13))
        {
            m_unreal_vc_base = std::make_unique<Derived415>();
            m_unreal_object_base = std::make_unique<UObject415>();
        }
        else if (Version::is_equal(4, 14))
        {
            m_unreal_vc_base = std::make_unique<Derived415>();
            m_unreal_object_base = std::make_unique<UObject415>();
        }
        else if (Version::is_equal(4, 15))
        {
            m_unreal_vc_base = std::make_unique<Derived412>();
            m_unreal_object_base = std::make_unique<UObject412>();
        }
        else if (Version::is_equal(4, 16))
        {
            m_unreal_vc_base = std::make_unique<Derived412>();
            m_unreal_object_base = std::make_unique<UObject412>();
        }
        else if (Version::is_equal(4, 17))
        {
            m_unreal_vc_base = std::make_unique<Derived412>();
            m_unreal_object_base = std::make_unique<UObject412>();
        }
        else if (Version::is_equal(4, 18))
        {
            // Using 4.12 since it seems to have the correct GUObjectArray definition
            m_unreal_vc_base = std::make_unique<Derived412>();
            m_unreal_object_base = std::make_unique<UObject412>();
            m_unreal_virtual_base = std::make_unique<UnrealVirtual412>();
        }
        else if (Version::is_equal(4, 19))
        {
            m_unreal_vc_base = std::make_unique<Derived422>();
            m_unreal_object_base = std::make_unique<UObject422>();
        }
        else if (Version::is_equal(4, 20))
        {
            m_unreal_vc_base = std::make_unique<Derived422>();
            m_unreal_object_base = std::make_unique<UObject422>();
        }
        else if (Version::is_equal(4, 21))
        {
            m_unreal_vc_base = std::make_unique<Derived422>();
            m_unreal_object_base = std::make_unique<UObject422>();
        }
        else if (Version::is_equal(4, 22))
        {
            m_unreal_vc_base = std::make_unique<Derived422>();
            m_unreal_object_base = std::make_unique<UObject422>();
            m_unreal_virtual_base = std::make_unique<UnrealVirtual422>();
        }
        else if (Version::is_equal(4, 23))
        {
            m_unreal_vc_base = std::make_unique<Derived422>();
            m_unreal_object_base = std::make_unique<UObject422>();
        }
        else if (Version::is_equal(4, 24))
        {
            m_unreal_vc_base = std::make_unique<Derived422>();
            m_unreal_object_base = std::make_unique<UObject422>();
        }
        else if (Version::is_equal(4, 25))
        {
            m_unreal_vc_base = std::make_unique<Derived425>();
            m_unreal_object_base = std::make_unique<UObject425>();
            m_unreal_virtual_base = std::make_unique<UnrealVirtual426>();
        }
        else if (Version::is_equal(4, 26))
        {
            m_unreal_vc_base = std::make_unique<Derived425>();
            m_unreal_object_base = std::make_unique<UObject425>();
            m_unreal_virtual_base = std::make_unique<UnrealVirtual426>();
        }
        else if (Version::is_equal(4, 27))
        {
            m_unreal_vc_base = std::make_unique<Derived425>();
            m_unreal_object_base = std::make_unique<UObject425>();
            m_unreal_virtual_base = std::make_unique<UnrealVirtual426>();
        }
        else if (Version::is_equal(4, 5))
        {
            m_unreal_vc_base = std::make_unique<Derived425>();
            m_unreal_object_base = std::make_unique<UObject425>();
            m_unreal_virtual_base = std::make_unique<UnrealVirtual426>();
        }
        else
        {
            // If this code is ever reached then the version isn't explicitly supported
            // Using 4.25 as a long-shot, sometimes this will work but in the future it's likely that this will break
            m_unreal_vc_base = std::make_unique<Derived425>();
            m_unreal_object_base = std::make_unique<UObject425>();
            m_unreal_virtual_base = std::make_unique<UnrealVirtual426>();
        }
    }
}