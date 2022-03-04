#include <Unreal/VersionedContainer/Container.hpp>
#include <Unreal/UnrealVersion.hpp>
#include <Unreal/VersionedContainer/Derived412.hpp>
#include <Unreal/VersionedContainer/Derived413.hpp>
#include <Unreal/VersionedContainer/Derived415.hpp>
#include <Unreal/VersionedContainer/Derived422.hpp>
#include <Unreal/VersionedContainer/Derived425.hpp>
#include <Unreal/VersionedContainer/UObject412.hpp>
#include <Unreal/VersionedContainer/UObject415.hpp>
#include <Unreal/VersionedContainer/UObject422.hpp>
#include <Unreal/VersionedContainer/UObject425.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual412.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual413.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual414.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual415.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual416.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual417.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual418.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual419.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual420.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual421.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual422.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual423.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual424.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual425.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual426.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual427.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtual500.hpp>

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
            // FUObjectItem changed in this version
            m_unreal_vc_base = std::make_unique<Derived413>();
            m_unreal_object_base = std::make_unique<UObject415>();
            m_unreal_virtual_base = std::make_unique<UnrealVirtual413>();
        }
        else if (Version::is_equal(4, 14))
        {
            m_unreal_vc_base = std::make_unique<Derived413>();
            m_unreal_object_base = std::make_unique<UObject415>();
            m_unreal_virtual_base = std::make_unique<UnrealVirtual414>();
        }
        else if (Version::is_equal(4, 15))
        {
            m_unreal_vc_base = std::make_unique<Derived413>();
            m_unreal_object_base = std::make_unique<UObject412>();
            m_unreal_virtual_base = std::make_unique<UnrealVirtual415>();
        }
        else if (Version::is_equal(4, 16))
        {
            m_unreal_vc_base = std::make_unique<Derived413>();
            m_unreal_object_base = std::make_unique<UObject412>();
            m_unreal_virtual_base = std::make_unique<UnrealVirtual416>();
        }
        else if (Version::is_equal(4, 17))
        {
            m_unreal_vc_base = std::make_unique<Derived413>();
            m_unreal_object_base = std::make_unique<UObject412>();
            m_unreal_virtual_base = std::make_unique<UnrealVirtual417>();
        }
        else if (Version::is_equal(4, 18))
        {
            // Using 4.12 since it seems to have the correct GUObjectArray definition
            m_unreal_vc_base = std::make_unique<Derived413>();
            m_unreal_object_base = std::make_unique<UObject412>();
            m_unreal_virtual_base = std::make_unique<UnrealVirtual418>();
        }
        else if (Version::is_equal(4, 19))
        {
            m_unreal_vc_base = std::make_unique<Derived413>();
            m_unreal_object_base = std::make_unique<UObject422>();
            m_unreal_virtual_base = std::make_unique<UnrealVirtual419>();
        }
        else if (Version::is_equal(4, 20))
        {
            // FFixedUObjectArray was replaced with FChunkedFixedUObjectArray
            m_unreal_vc_base = std::make_unique<Derived420>();
            m_unreal_object_base = std::make_unique<UObject422>();
            m_unreal_virtual_base = std::make_unique<UnrealVirtual420>();
        }
        else if (Version::is_equal(4, 21))
        {
            m_unreal_vc_base = std::make_unique<Derived420>();
            m_unreal_object_base = std::make_unique<UObject422>();
            m_unreal_virtual_base = std::make_unique<UnrealVirtual421>();
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
            m_unreal_virtual_base = std::make_unique<UnrealVirtual423>();
        }
        else if (Version::is_equal(4, 24))
        {
            m_unreal_vc_base = std::make_unique<Derived422>();
            m_unreal_object_base = std::make_unique<UObject422>();
            m_unreal_virtual_base = std::make_unique<UnrealVirtual424>();
        }
        else if (Version::is_equal(4, 25))
        {
            m_unreal_vc_base = std::make_unique<Derived425>();
            m_unreal_object_base = std::make_unique<UObject425>();
            m_unreal_virtual_base = std::make_unique<UnrealVirtual425>();
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
            m_unreal_virtual_base = std::make_unique<UnrealVirtual427>();
        }
        else if (Version::is_equal(4, 5))
        {
            m_unreal_vc_base = std::make_unique<Derived425>();
            m_unreal_object_base = std::make_unique<UObject425>();
            m_unreal_virtual_base = std::make_unique<UnrealVirtual500>();
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