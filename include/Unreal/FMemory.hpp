#ifndef RC_UNREAL_FMEMORY_HPP
#define RC_UNREAL_FMEMORY_HPP

#include <Function/Function.hpp>

#include <Unreal/Common.hpp>

/*
 * How GMalloc gets created:
 * 1. UnrealMemory.cpp -> FMemory_GCreateMalloc_ThreadUnsafe()
 * 2.     It sets GMalloc to:
 * 3. WindowsPlatformMemory.cpp -> FWindowsPlatformMemory::BaseAllocator()
 * 4. It selects Binned2
 * 5. It uses 'new' to create an instance of 'FMallocBinned2' and returns it
 */

namespace RC::Unreal
{
    class FMalloc
    {
    private:
        void** vtable;

    public:
        static Function<void*(FMalloc* self, size_t count, uint32_t alignment)> malloc_internal;
        static Function<void(FMalloc* self, void* original)> free_internal;

    public:
        auto get_vtable_entry(size_t index) -> void*;
        auto malloc(size_t count, uint32_t alignment = 0) -> void*;
        auto free(void* original) -> void;
    };

    extern FMalloc* gmalloc;

    class RC_UE_API FMemory
    {
    public:
        static Function<void(void*)> free;
    };
}

#endif //RC_UNREAL_FMEMORY_HPP
