#ifndef RC_UNREAL_CONTAINER_HPP
#define RC_UNREAL_CONTAINER_HPP

#include <memory>

#include <Unreal/Common.hpp>
#include <Unreal/VersionedContainer/Base.hpp>
#include <Unreal/VersionedContainer/Derived412.hpp>
#include <Unreal/VersionedContainer/Derived415.hpp>
#include <Unreal/VersionedContainer/Derived422.hpp>
#include <Unreal/VersionedContainer/Derived425.hpp>
#include <Unreal/VersionedContainer/UObjectBaseVC.hpp>
#include <Unreal/VersionedContainer/UObject412.hpp>
#include <Unreal/VersionedContainer/UObject415.hpp>
#include <Unreal/VersionedContainer/UObject422.hpp>
#include <Unreal/VersionedContainer/UObject425.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtualBaseVC.hpp>

namespace RC::Unreal
{
    enum class EngineVersion
    {
        UE4_12,
        UE4_13,
        UE4_14,
        UE4_15,
        UE4_16,
        UE4_17,
        UE4_18,
        UE4_19,
        UE4_20,
        UE4_21,
        UE4_22,
        UE4_23,
        UE4_24,
        UE4_25,
        UE4_26,
        UE4_27,
        UE5_00,
    };

    class Container
    {
    public:
        inline static std::unique_ptr<Base> m_unreal_vc_base;
        inline static std::unique_ptr<UObjectBaseVC> m_unreal_object_base;
        inline static std::unique_ptr<UnrealVirtualBaseVC> m_unreal_virtual_base;

    public:
        auto static set_derived_base_objects() -> void;
    };
}


#endif //RC_UNREAL_CONTAINER_HPP
