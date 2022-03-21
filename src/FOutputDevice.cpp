#include <Unreal/FOutputDevice.hpp>
#include <Function/Function.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtualBaseVC.hpp>
#include <Unreal/UnrealVersion.hpp>

namespace RC::Unreal
{
    void FOutputDevice::Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const FName& Category, const double Time)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FOutputDevice,
                                         Serialize,
                                         void,
                                         PARAMS(const TCHAR*, ELogVerbosity::Type, const FName&, const double),
                                         ARGS(V, Verbosity, Category, Time))
    }

    void FOutputDevice::Flush()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FOutputDevice, Flush, void)
    }

    void FOutputDevice::TearDown()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FOutputDevice, TearDown, void)
    }

    void FOutputDevice::Dump(class FArchive& Ar)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FOutputDevice, Dump, void, PARAMS(class FArchive&), ARGS(Ar))
    }

    bool FOutputDevice::IsMemoryOnly() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FOutputDevice, IsMemoryOnly, bool)
    }

    bool FOutputDevice::CanBeUsedOnAnyThread() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FOutputDevice, CanBeUsedOnAnyThread, bool)
    }

    bool FOutputDevice::CanBeUsedOnMultipleThreads() const
    {
        if (Version::is_below(4, 21))
        {
            throw std::runtime_error{"FOutputDevice::CanBeUsedOnMultipleThreads is only available in 4.21+"};
        }

        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FOutputDevice, CanBeUsedOnMultipleThreads, bool)
    }

    void FOutputDevice::Log(const TCHAR* Str)
    {
        Serialize(Str, ELogVerbosity::Verbose, FName(), 0);
    }
}
