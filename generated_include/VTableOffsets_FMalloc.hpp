struct VTableOffsets
{
    static uint32_t DumpAllocatorStats;
    static uint32_t Malloc;
    static uint32_t FMalloc_Destructor;
    static uint32_t Trim;
    static uint32_t ValidateHeap;
    static uint32_t Realloc;
    static uint32_t ClearAndDisableTLSCachesOnCurrentThread;
    static uint32_t QuantizeSize;
    static uint32_t Free;
    static uint32_t GetAllocationSize;
    static uint32_t SetupTLSCachesOnCurrentThread;
    static uint32_t InitializeStatsMetadata;
    static uint32_t Exec;
    static uint32_t UpdateStats;
    static uint32_t GetAllocatorStats;
    static uint32_t IsInternallyThreadSafe;
    static uint32_t GetDescriptiveName;
    static uint32_t __vecDelDtor;
    static uint32_t TryMalloc;
    static uint32_t TryRealloc;
};
