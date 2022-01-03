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

    public:
        template<EngineVersion UnrealVersion>
        auto static set_to_derived() -> void
        {
            if constexpr (UnrealVersion == EngineVersion::UE4_12)
            {
                m_unreal_vc_base = std::make_unique<Derived412>();
                m_unreal_object_base = std::make_unique<UObject412>();
            }
            else if constexpr (UnrealVersion == EngineVersion::UE4_15)
            {
                m_unreal_vc_base = std::make_unique<Derived415>();
                m_unreal_object_base = std::make_unique<UObject415>();
            }
            else if constexpr (UnrealVersion == EngineVersion::UE4_22)
            {
                m_unreal_vc_base = std::make_unique<Derived422>();
                m_unreal_object_base = std::make_unique<UObject422>();
            }
            else if constexpr (UnrealVersion == EngineVersion::UE4_25)
            {
                m_unreal_vc_base = std::make_unique<Derived425>();
                m_unreal_object_base = std::make_unique<UObject425>();
            }
            else
            {
                static_assert(false, "Engine version not supported");
            }
        }
    };
}


#endif //RC_UNREAL_CONTAINER_HPP
