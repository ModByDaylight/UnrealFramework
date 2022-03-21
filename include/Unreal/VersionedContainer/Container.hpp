#ifndef RC_UNREAL_CONTAINER_HPP
#define RC_UNREAL_CONTAINER_HPP

#include <memory>

#include <Unreal/Common.hpp>
#include <Unreal/VersionedContainer/Base.hpp>
#include <Unreal/VersionedContainer/UObjectBaseVC.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtualBaseVC.hpp>

namespace RC::Unreal
{
    class Container
    {
    public:
        inline static std::unique_ptr<Base> UnrealVC;
        inline static std::unique_ptr<UObjectBaseVC> UnrealObjectVC;
        inline static std::unique_ptr<UnrealVirtualBaseVC> UnrealVirtualVC;

    public:
        auto static SetDerivedBaseObjects() -> void;
    };
}


#endif //RC_UNREAL_CONTAINER_HPP
