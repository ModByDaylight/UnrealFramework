#ifndef RC_UNREAL_FMEMORY_HPP
#define RC_UNREAL_FMEMORY_HPP

#include <Unreal/Common.hpp>
#include <Unreal/PrimitiveTypes.hpp>
#include <unordered_map>

#define IMPLEMENT_UNREAL_TYPE(TypeName) \
    FORCEINLINE void* operator new(size_t Size) { return FMemory::Malloc(Size); } \
    FORCEINLINE void operator delete(void* Ptr) { if (Ptr) FMemory::Free(Ptr); }  \
    FORCEINLINE void* operator new[](size_t Size) { return FMemory::Malloc(Size); } \
    FORCEINLINE void operator delete[](void* Ptr) { if (Ptr) FMemory::Free(Ptr); } \
    FORCEINLINE void* operator new(size_t Size, void* Ptr) { return Ptr; }

NS_RC_UE_START

enum {
    // Default allocator alignment. If the default is specified, the allocator applies to engine rules.
    // Blocks >= 16 bytes will be 16-byte-aligned, Blocks < 16 will be 8-byte aligned. If the allocator does
    // not support allocation alignment, the alignment will be ignored.
    DEFAULT_ALIGNMENT = 0,

    // Minimum allocator alignment
    MIN_ALIGNMENT = 8,
};

struct FGenericMemoryStats;

class RC_UE_API FMalloc {
public:
    static std::unordered_map<std::wstring, uint32_t> VTableLayoutMap;
public:
    void* Malloc(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
    void* TryMalloc(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
    void* Realloc(void* Original, SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
    void* TryRealloc(void* Original, SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
    void Free(void* Original);
    SIZE_T QuantizeSize(SIZE_T Count, uint32 Alignment);
    bool GetAllocationSize(void* Original, SIZE_T& SizeOut);
    void Trim(bool bTrimThreadCaches);
    void SetupTLSCachesOnCurrentThread();
    void ClearAndDisableTLSCachesOnCurrentThread();
    void InitializeStatsMetadata();
    void UpdateStats();
    void GetAllocatorStats(FGenericMemoryStats& out_Stats);
    void DumpAllocatorStats(class FOutputDevice& Ar);
    bool IsInternallyThreadSafe() const;
    bool ValidateHeap();
    const TCHAR* GetDescriptiveName();
};

RC_UE_API extern FMalloc** GMallocStorage;
RC_UE_API extern FMalloc* GMalloc;

class RC_UE_API FMemory {
public:
    static void* Memzero(void* Dest, SIZE_T Count);

    static void* Malloc(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);

    static void* Realloc(void* Original, SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);

    static void Free(void* Original);

    static void* Memcpy(void* Dest, const void* Src, SIZE_T Count);

    static void Memswap(void* Ptr1, void* Ptr2, SIZE_T Size);

    static SIZE_T QuantizeSize(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
};

NS_RC_UE_END

#endif //RC_UNREAL_FMEMORY_HPP
