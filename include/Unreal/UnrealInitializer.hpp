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
}

namespace RC::Unreal::UnrealInitializer
{
    struct RC_UE_API ScanOverrides
    {
        std::function<void(std::vector<SignatureContainer>&)> guobjectarray;
        std::function<void(std::vector<SignatureContainer>&)> static_find_object;
        std::function<void(std::vector<SignatureContainer>&)> version_finder;
        std::function<void(std::vector<SignatureContainer>&)> fname_to_string;
        std::function<void(std::vector<SignatureContainer>&)> fname_constructor;
        std::function<void(std::vector<SignatureContainer>&)> fmemory_free;
        std::function<void(std::vector<SignatureContainer>&)> process_event;
        std::function<void(std::vector<SignatureContainer>&)> static_construct_object;
    };

    // Struct to be passed to the 'initialize' function
    // Should always have valid default values
    struct RC_UE_API Config
    {
    public:
        HANDLE process_handle;
        HMODULE module_handle;

        uint32_t num_scan_threads{8};
        uint32_t multithreading_module_size_threshold{0x1000000};

        // Functions to be called if you want to override one or more scans
        ScanOverrides scan_overrides{};

        std::filesystem::path self_file{};
        std::filesystem::path game_exe{};

        // Path to cache files, used to bypass aob scanning when the module hasn't changed from the last time it was scanned
        std::filesystem::path cache_path{};

        bool invalidate_cache_if_self_changed{true};

        int64_t num_scan_attempts_normal{20};
        int64_t num_scan_attempts_modular{2000};
    };

    struct RC_UE_API CacheInfo
    {
        File::Handle game_exe_file;
        bool should_use_cache;
        bool should_serialize_cache;
    };

    struct RC_UE_API StaticStorage
    {
        static inline bool is_initialized{false};
    };

    auto RC_UE_API setup_unreal_modules(const Config&) -> void;
    auto RC_UE_API verify_module_cache(const Config&) -> CacheInfo;
    auto RC_UE_API create_cache(CacheInfo&) -> void;
    auto RC_UE_API load_cache(CacheInfo&) -> void;
    auto RC_UE_API initialize_versioned_container() -> void;
    auto RC_UE_API initialize(const Config&) -> void;
}


#endif //RC_UNREAL_UNREAL_INITIALIZER_HPP
