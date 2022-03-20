struct VTableOffsets
{
    static uint32_t CanBeUsedOnAnyThread;
    static uint32_t FOutputDevice_Destructor;
    static uint32_t Serialize;
    static uint32_t Dump;
    static uint32_t Flush;
    static uint32_t TearDown;
    static uint32_t __vecDelDtor;
    static uint32_t IsMemoryOnly;
    static uint32_t CanBeUsedOnMultipleThreads;
};
