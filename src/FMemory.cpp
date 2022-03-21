#include <Unreal/FMemory.hpp>
#include <Unreal/VersionedContainer/UnrealVirtualImpl/UnrealVirtualBaseVC.hpp>

namespace RC::Unreal
{
    FMalloc* GMalloc{};
    FMalloc** FMalloc::UnrealStaticGMalloc{};
    bool FMalloc::bIsInitialized = false;

    void* FMalloc::Malloc(SIZE_T Count, uint32 Alignment)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FMalloc, Malloc, void*, PARAMS(SIZE_T, uint32), ARGS(Count, Alignment))
    }

    void* FMalloc::TryMalloc(SIZE_T Count, uint32 Alignment)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FMalloc, TryMalloc, void*, PARAMS(SIZE_T, uint32), ARGS(Count, Alignment))
    }

    void* FMalloc::Realloc(void* Original, SIZE_T Count, uint32 Alignment)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FMalloc, Realloc, void*, PARAMS(void*, SIZE_T, uint32), ARGS(Original, Count, Alignment))
    }

    void* FMalloc::TryRealloc(void* Original, SIZE_T Count, uint32 Alignment)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FMalloc, TryRealloc, void*, PARAMS(void*, SIZE_T, uint32), ARGS(Original, Count, Alignment))
    }

    void FMalloc::Free(void* Original)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FMalloc, Free, void, PARAMS(void*), ARGS(Original))
    }

    SIZE_T FMalloc::QuantizeSize(SIZE_T Count, uint32 Alignment)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FMalloc, QuantizeSize, SIZE_T, PARAMS(SIZE_T, uint32), ARGS(Count, Alignment))
    }

    bool FMalloc::GetAllocationSize(void* Original, SIZE_T& SizeOut)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FMalloc, GetAllocationSize, bool, PARAMS(void*, SIZE_T&), ARGS(Original, SizeOut))
    }

    void FMalloc::Trim(bool bTrimThreadCaches)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FMalloc, Trim, void, PARAMS(bool), ARGS(bTrimThreadCaches))
    }

    void FMalloc::SetupTLSCachesOnCurrentThread()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FMalloc, SetupTLSCachesOnCurrentThread, void)
    }

    void FMalloc::ClearAndDisableTLSCachesOnCurrentThread()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FMalloc, ClearAndDisableTLSCachesOnCurrentThread, void)
    }

    void FMalloc::InitializeStatsMetadata()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FMalloc, InitializeStatsMetadata, void)
    }

    void FMalloc::UpdateStats()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FMalloc, UpdateStats, void)
    }

    void FMalloc::GetAllocatorStats(FGenericMemoryStats& out_Stats)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FMalloc, GetAllocatorStats, void, PARAMS(FGenericMemoryStats&), ARGS(out_Stats))
    }

    void FMalloc::DumpAllocatorStats(class FOutputDevice& Ar)
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(FMalloc, DumpAllocatorStats, void, PARAMS(FOutputDevice&), ARGS(Ar))
    }

    bool FMalloc::IsInternallyThreadSafe() const
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FMalloc, IsInternallyThreadSafe, bool)
    }

    bool FMalloc::ValidateHeap()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FMalloc, ValidateHeap, bool)
    }

    const TCHAR* FMalloc::GetDescriptiveName()
    {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER_NO_PARAMS(FMalloc, GetDescriptiveName, const TCHAR*)
    }

    void* FMemory::Memzero(void* Dest, SIZE_T Count)
    {
        return std::memset(Dest, 0, Count);
    }

    void* FMemory::Malloc(SIZE_T Count, uint32 Alignment)
    {
        if (!GMalloc || !FMalloc::bIsInitialized)
        {
            throw std::runtime_error{"Tried to call 'FMemory::Malloc' before the FMalloc instance was found"};
        }

        return GMalloc->Malloc(Count, Alignment);
    }

    void FMemory::Free(void* Original)
    {
        if (!GMalloc || !FMalloc::bIsInitialized) { return; }

        GMalloc->Free(Original);
    }

    void* FMemory::Realloc(void* Original, SIZE_T Count, uint32 Alignment)
    {
        if (!GMalloc || !FMalloc::bIsInitialized)
        {
            throw std::runtime_error{"Tried to call 'FMemory::Realloc' before the FMalloc instance was found"};
        }

        return GMalloc->Realloc(Original, Count, Alignment);
    }
}
