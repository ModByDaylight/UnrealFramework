#include <Unreal/VirtualFunctionHelper.hpp>

auto get_shared_objects() -> std::unordered_map<std::string, void*>&
{
    static std::unordered_map<std::string, void*> g_shared_objects;
    return g_shared_objects;
}

void* SharedObjectManager::get_shared_object_internal(const char* id)
{
    const auto iterator = get_shared_objects().find(std::string(id));
    return iterator != get_shared_objects().end() ? iterator->second : nullptr;
}

void SharedObjectManager::set_shared_object_internal(const char* id, void* new_value)
{
    get_shared_objects().insert({std::string(id), new_value});
}
