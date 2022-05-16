#include <UnrealVFS/FileManager.hpp>
#include <stdexcept>

NS_RC_UE_START

static IFileManager* ActiveFileManager{nullptr};

void IFileManager::SetFileManager(IFileManager* UnderlyingManager) {
    if (ActiveFileManager != nullptr && ActiveFileManager != UnderlyingManager) {
        throw std::runtime_error{"IFileManager has already been initialized"};
    }
    ActiveFileManager = UnderlyingManager;
}

IFileManager& IFileManager::Get() {
    if (ActiveFileManager == nullptr) {
        throw std::runtime_error{"IFileManager has been accessed before initialization"};
    }
    return *ActiveFileManager;
}

NS_RC_UE_END