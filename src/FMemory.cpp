#include <Unreal/FMemory.hpp>

namespace RC::Unreal
{
    FMalloc* gmalloc{};

    Function<void*(FMalloc* self, size_t count, uint32_t alignment)> FMalloc::malloc_internal{};
    Function<void(FMalloc* self, void* original)> FMalloc::free_internal{};

    // If the FMalloc stuff works, then maybe we should remove FMemory::free.
    Function<void(void*)> FMemory::free{};

    auto FMalloc::get_vtable_entry(size_t index) -> void*
    {
        return vtable[index];
    }

    auto FMalloc::malloc(size_t count, uint32_t alignment) -> void*
    {
        return malloc_internal(this, count, alignment);
    }

    auto FMalloc::free(void* original) -> void
    {
        free_internal(this, original);
    }
}
