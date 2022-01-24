#include <Unreal/FMemory.hpp>

namespace RC::Unreal
{
    Function<void*(size_t count, uint32_t alignment)> FMalloc::malloc{};
    Function<void*(void* original)> FMalloc::free{};

    // If the FMalloc stuff works, then maybe we should remove FMemory::free.
    Function<void(void*)> FMemory::free{};

    auto FMalloc::get_vtable_entry(size_t index) -> void*
    {
        return vtable[index];
    }
}
