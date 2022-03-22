#ifndef RC_UNREAL_UNREAL_INITIALIZER_HPP
#define RC_UNREAL_UNREAL_INITIALIZER_HPP

#include <cstdint>
#include <functional>
#include <filesystem>

#include <File/File.hpp>

#include <Unreal/Common.hpp>
#include <Unreal/UObjectGlobals.hpp>

namespace RC
{
    class SignatureContainer;

    namespace Unreal::Signatures
    {
        struct ScanResult;
    }
}

namespace RC::Unreal::UnrealInitializer
{
    struct RC_UE_API ScanOverrides
    {
        std::function<void(std::vector<SignatureContainer>&, Signatures::ScanResult&)> guobjectarray;
        std::function<void(std::vector<SignatureContainer>&, Signatures::ScanResult&)> static_find_object;
        std::function<void(std::vector<SignatureContainer>&, Signatures::ScanResult&)> version_finder;
        std::function<void(std::vector<SignatureContainer>&, Signatures::ScanResult&)> fname_to_string;
        std::function<void(std::vector<SignatureContainer>&, Signatures::ScanResult&)> fname_constructor;
        std::function<void(std::vector<SignatureContainer>&, Signatures::ScanResult&)> fmemory_free;
        std::function<void(std::vector<SignatureContainer>&, Signatures::ScanResult&)> process_event;
        std::function<void(std::vector<SignatureContainer>&, Signatures::ScanResult&)> static_construct_object;
    };

    // Struct to be passed to the 'initialize' function
    // Should always have valid default values
    struct RC_UE_API Config
    {
    public:
        uint32_t NumScanThreads{8};
        uint32_t MultithreadingModuleSizeThreshold{0x1000000};

        // Functions to be called if you want to override one or more scans
        ScanOverrides ScanOverrides{};

        std::filesystem::path SelfFile{};
        std::filesystem::path GameExe{};

        // Path to cache files, used to bypass aob scanning when the module hasn't changed from the last time it was scanned
        std::filesystem::path CachePath{};

        bool bInvalidateCacheIfSelfChanged{true};

        int64_t NumScanAttemptsNormal{60};
        int64_t NumScanAttemptsModular{2500};
    };

    struct RC_UE_API CacheInfo
    {
        File::Handle GameExeFile;
        bool ShouldUseCache;
        bool ShouldSerializeCache;
    };

    struct RC_UE_API StaticStorage
    {
        static inline bool bIsInitialized{false};
    };

    auto RC_UE_API SetupUnrealModules() -> void;
    auto RC_UE_API VerifyModuleCache(const Config&) -> CacheInfo;
    auto RC_UE_API CreateCache(CacheInfo&) -> void;
    auto RC_UE_API LoadCache(CacheInfo&) -> void;
    auto RC_UE_API InitializeVersionedContainer() -> void;
    auto RC_UE_API Initialize(const Config&) -> void;
}


#endif //RC_UNREAL_UNREAL_INITIALIZER_HPP
