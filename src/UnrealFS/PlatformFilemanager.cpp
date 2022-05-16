#include <UnrealVFS/PlatformFilemanager.hpp>
#include <UnrealVFS/GenericPlatformFile.h>

NS_RC_UE_START

static FPlatformFileManager* ActiveFileManager{nullptr};

void FPlatformFileManager::SetPlatformFileManager(FPlatformFileManager* UnderlyingManager) {
    if (ActiveFileManager != nullptr && ActiveFileManager != UnderlyingManager) {
        throw std::runtime_error{"Active FPlatformFileManager has already been set"};
    }
    ActiveFileManager = UnderlyingManager;
}

IPlatformFile& FPlatformFileManager::GetPlatformFile() {
    return *TopmostPlatformFile;
}

void FPlatformFileManager::SetPlatformFile(IPlatformFile& NewTopmostPlatformFile) {
    TopmostPlatformFile = &NewTopmostPlatformFile;
    NewTopmostPlatformFile.InitializeAfterSetActive();
}

IPlatformFile* FPlatformFileManager::FindPlatformFile(const TCHAR* Name) {
    for (IPlatformFile* ChainElement = TopmostPlatformFile; ChainElement; ChainElement = ChainElement->GetLowerLevel()) {
        if (_wcsicmp(ChainElement->GetName(), Name) == 0) {
            return ChainElement;
        }
    }
    return nullptr;
}

FPlatformFileManager& FPlatformFileManager::Get() {
    if (ActiveFileManager == nullptr) {
        throw std::runtime_error{"Attempt to retrieve FPlatformFileManager while one has not been initialized yet"};
    }
    return *ActiveFileManager;
}

NS_RC_UE_END