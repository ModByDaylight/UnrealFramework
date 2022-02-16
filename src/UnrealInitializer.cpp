#include <stdexcept>
#include <Helpers/Casting.hpp>
#include <SigScanner/SinglePassSigScanner.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include <Unreal/UnrealInitializer.hpp>
#include <Unreal/VersionedContainer/Container.hpp>
#include <Unreal/UnrealVersion.hpp>
#include <Unreal/Signatures.hpp>
#include <Unreal/Hooks.hpp>
#include <Unreal/FString.hpp>
#include "Unreal/FMemory.hpp"


namespace RC::Unreal::UnrealInitializer
{
    auto setup_unreal_modules(const Config& config) -> void
    {
        // Setup all modules for the aob scanner
        MODULEINFO module_info;
        K32GetModuleInformation(config.process_handle, config.module_handle, &module_info, sizeof(MODULEINFO));
        SigScannerStaticData::m_modules_info[ScanTarget::MainExe] = module_info;

        HMODULE modules[1024];
        DWORD bytes_required;

        if (K32EnumProcessModules(config.process_handle, modules, sizeof(modules), &bytes_required) == 0)
        {
            throw std::runtime_error{std::format("Was unable to enumerate game modules. Error Code: {}", GetLastError())};
        }

        // Default all modules to be the same as MainExe
        // This is because most UE4 games only have the MainExe module
        for (size_t i = 0; i < static_cast<size_t>(ScanTarget::Max); ++i)
        {
            SigScannerStaticData::m_modules_info.array[i] = module_info;
        }

        // Check for modules and save the module info if they exist
        for (auto i = 0; i < bytes_required / sizeof(HMODULE); ++i)
        {
            char module_raw_name[MAX_PATH];
            // TODO: Fix an occasional error: "Call to K32GetModuleBaseNameA failed. Error Code: 6 (ERROR_INVALID_HANDLE)"
            if (K32GetModuleBaseNameA(config.process_handle, modules[i], module_raw_name, sizeof(module_raw_name) / sizeof(char)) == 0)
            {
                throw std::runtime_error{std::format("Call to K32GetModuleBaseNameA failed. Error Code: {}", GetLastError())};
            }

            std::string module_name{module_raw_name};

            for (size_t i2 = 0; i2 < static_cast<size_t>(ScanTarget::Max); ++i2)
            {
                std::string module_to_find{"-"};
                module_to_find.append(ScanTargetToString(i2));
                module_to_find.append("-Win64-Shipping.dll");

                size_t occurrence = module_name.find(module_to_find);
                if (occurrence != module_name.npos)
                {
                    if (!SigScannerStaticData::m_is_modular) { SigScannerStaticData::m_is_modular = true; }

                    K32GetModuleInformation(config.process_handle, modules[i], &SigScannerStaticData::m_modules_info.array[i2], sizeof(MODULEINFO));
                }
            }
        }
    }

    auto verify_module_cache(const Config& config) -> CacheInfo
    {
        bool use_cache{};
        bool should_serialize_cache{true};
        bool should_serialize_cache_override{};
        File::Handle game_exe_file;
        File::Handle self_file;

        // Check if the self file (the injected dll that this code is contained within) has changed
        // and whether that means that we should invalidate the cache
        try
        {
            self_file = File::open(config.self_file);

            std::filesystem::path self_cache_path_and_file = config.cache_path;
            self_cache_path_and_file /= config.self_file.filename();
            self_cache_path_and_file.replace_extension(".cache");

            self_file.set_serialization_output_file(self_cache_path_and_file);

            if (!self_file.is_deserialized_and_live_equal())
            {
                // The self file has changed, invalidate the cache
                // This could also mean that the cache file didn't exist

                bool should_invalidate = !std::filesystem::exists(self_cache_path_and_file) || config.invalidate_cache_if_self_changed;

                if (should_invalidate)
                {
                    // Invalidate all caches
                    for (const auto& item : std::filesystem::directory_iterator(config.cache_path))
                    {
                        std::error_code ec;
                        if (item.is_directory()) { continue; }
                        if (ec.value() != 0) { throw std::runtime_error{""}; }

                        if (item.path().extension() == ".cache")
                        {
                            File::Handle game_cache_file = File::open(item.path());
                            game_cache_file.delete_file();
                        }
                    }

                    // If the self cache file didn't exist, then we must create it
                    // If it does exist then this means that the self file has changed
                    // We invalidate depending on a setting in the config
                    self_file.serialize_identifying_properties();
                    should_serialize_cache_override = true;
                }
            }

            self_file.close();
        }
        catch (std::runtime_error&)
        {
            // Make sure we don't load the cache if there was an error while checking the self file
            should_serialize_cache_override = true;
        }

        // Check whether the game has a usable cache or whether we need to create one
        if (!config.cache_path.empty() && !config.game_exe.empty())
        {
            std::filesystem::path cache_path_and_file = config.cache_path;
            cache_path_and_file /= config.game_exe.filename();
            cache_path_and_file.replace_extension(".cache");

            game_exe_file = File::open(config.game_exe);
            game_exe_file.set_serialization_output_file(cache_path_and_file);

            if (game_exe_file.is_deserialized_and_live_equal() && !should_serialize_cache_override)
            {
                use_cache = true;
                should_serialize_cache = false;
            }
            else
            {
                // Invalidate the serialization file if it already exists
                // The deserialization not matching may be due to the serialization file not existing
                game_exe_file.invalidate_serialization();

                use_cache = false;
                should_serialize_cache = true;
            }
        }

        return {game_exe_file, use_cache, should_serialize_cache};
    }

    auto create_cache(CacheInfo& cache_info) -> void
    {
        auto get_module_offset = [&](ScanTarget scan_target, void* address) -> unsigned long {
            MODULEINFO module = SigScannerStaticData::m_modules_info[scan_target];
            return reinterpret_cast<uintptr_t>(address) - reinterpret_cast<uintptr_t>(module.lpBaseOfDll);
        };

        auto serialize = [&](ScanTarget scan_target, void* address) -> void {
            // Serialize user data
            // This automatically creates the serialization file if it doesn't yet exist
            // This will also automatically serialize identifying properties if they aren't already serialized
            cache_info.game_exe_file.serialize_item(static_cast<unsigned long>(scan_target));
            cache_info.game_exe_file.serialize_item(get_module_offset(scan_target, address));
        };

        // IMPORTANT: The ScanTarget does NOT have to be the same as they are in 'Signatures.cpp'
        // This is because, in 'Signatures.cpp', the ScanTarget is for the module to scan in
        // But when serializing, we're serializing the module offset into the module that contains the actual final data
        // So for example, ~FString(), exists the 'Core' module but the scanner scans in the MainExe module

        serialize(ScanTarget::CoreUObject, Container::m_unreal_vc_base->UObjectArray_get_internal_storage_ptr());

#if 0
        // This is test code that shouldn't be compiled for release
        serialize(ScanTarget::PakFile, PakFileTesting::Storage::is_non_pak_filename_allowed_address);
        serialize(ScanTarget::PakFile, PakFileTesting::Storage::find_file_in_pak_files_address);
        serialize(ScanTarget::PakFile, PakFileTesting::Storage::open_read_address);
        serialize(ScanTarget::CoreUObject, PakFileTesting::Storage::static_load_object_address);
#endif

        serialize(ScanTarget::CoreUObject, UObjectGlobals::GlobalState::static_find_object_internal.get_function_address());

        // Version is special, it's major/minor instead of ScanTarget/ModuleOffset
        cache_info.game_exe_file.serialize_item(Version::major);
        cache_info.game_exe_file.serialize_item(Version::minor);

        serialize(ScanTarget::Core, FName::to_string_internal.get_function_address());
        serialize(ScanTarget::Core, FName::constructor_internal.get_function_address());
        serialize(ScanTarget::Core, FMemory::free.get_function_address());
        serialize(ScanTarget::CoreUObject, UObject::process_event_internal.get_function_address());
        serialize(ScanTarget::CoreUObject, UObjectGlobals::GlobalState::static_construct_object_internal.get_function_address());
    }

    auto load_cache(CacheInfo& cache_info) -> void
    {
        auto module_offset_to_address = [](ScanTarget scan_target, unsigned long module_offset) -> void* {
            MODULEINFO module = SigScannerStaticData::m_modules_info[scan_target];
            return static_cast<void*>(static_cast<unsigned char*>(module.lpBaseOfDll) + module_offset);
        };

        auto deserialize = [&]() -> void* {
            ScanTarget scan_target = static_cast<ScanTarget>(cache_info.game_exe_file.get_serialized_item<unsigned long>());
            auto module_offset = cache_info.game_exe_file.get_serialized_item<unsigned long>();

            return module_offset_to_address(scan_target, module_offset);
        };


        void* guobjectarray = deserialize();

#if 0
        // This is test code that shouldn't be compiled for release
        PakFileTesting::Storage::is_non_pak_filename_allowed_func = std::bit_cast<PakFileTesting::IsNonPakFilenameAllowedFunc>(PakFileTesting::Storage::is_non_pak_filename_allowed_address);
        PakFileTesting::Storage::is_non_pak_filename_allowed_address = deserialize();
        PakFileTesting::Storage::find_file_in_pak_files_address = deserialize();
        PakFileTesting::Storage::find_file_in_pak_files_func = std::bit_cast<PakFileTesting::FindFileInPakFilesFunc>(PakFileTesting::Storage::find_file_in_pak_files_address);
        PakFileTesting::Storage::open_read_address = deserialize();
        PakFileTesting::Storage::open_read_func = std::bit_cast<PakFileTesting::OpenReadFunc>(PakFileTesting::Storage::open_read_address);
        PakFileTesting::Storage::static_load_object_address = deserialize();
        PakFileTesting::Storage::static_load_object_func = std::bit_cast<PakFileTesting::StaticLoadObjectFunc>(PakFileTesting::Storage::static_load_object_address);
#endif

        UObjectGlobals::setup_static_find_object_address(deserialize());

        // Version is special, it's major/minor instead of ScanTarget/ModuleOffset
       auto major = cache_info.game_exe_file.get_serialized_item<unsigned int>();
       auto minor = cache_info.game_exe_file.get_serialized_item<unsigned int>();
        if (Version::major == -1)
        {
            // Only set the version if it's not already been set elsewhere
            Version::major = major;
            Version::minor = minor;
        }

        initialize_versioned_container();
        Container::m_unreal_vc_base->UObjectArray_set_internal_storage_ptr(guobjectarray);

        FName::to_string_internal.assign_address(deserialize());
        FName::constructor_internal.assign_address(deserialize());
        FMemory::free.assign_address(deserialize());
        UObject::process_event_internal.assign_address(deserialize());
        void* static_construct_object_address = deserialize();
        UObjectGlobals::GlobalState::static_construct_object_internal.assign_address(static_construct_object_address);
        UObjectGlobals::GlobalState::static_construct_object_internal_deprecated.assign_address(static_construct_object_address);
    }

    auto initialize_versioned_container() -> void
    {
        Container::set_derived_base_objects();
    }

    auto initialize(const Config& config) -> void
    {
        // Setup scanner
        SinglePassScanner::m_num_threads = config.num_scan_threads;
        SinglePassScanner::m_multithreading_module_size_threshold = config.multithreading_module_size_threshold;

        // Setup all modules for the aob scanner
        // This is currently done outside the Unreal API in order to over come a problem
        // TODO: Put an option here to check if the modules are initialized and initialize if they aren't
        //setup_unreal_modules(config);

        // Check if we have a valid cache or if a cache should be created after scanning
        CacheInfo cache_info = verify_module_cache(config);

        if (cache_info.should_use_cache)
        {
            load_cache(cache_info);
        }
        else
        {
            enum class SuppressScanAttemptMessage { Yes, No };
            enum class OutputErrorsByThrowing { Yes, No };
            enum class ErrorsOnly { Yes, No };

            auto output_result = [](Signatures::ScanResult& scan_result, OutputErrorsByThrowing output_errors_by_throwing = OutputErrorsByThrowing::No, ErrorsOnly errors_only = ErrorsOnly::No) {
                if (scan_result.scan_status == Signatures::ScanStatus::Failed)
                {
                    std::string all_errors{"AOB scans could not be completed because of the following reasons:\n"};
                    std::string fatal_errors{};
                    std::string non_fatal_errors{};
                    for (const auto& error : scan_result.errors)
                    {
                        if (error.is_fatal)
                        {
                            fatal_errors.append(error.message + "\n\n");
                        }
                        else
                        {
                            non_fatal_errors.append(error.message + "\n\n");
                        }
                    }

                    all_errors.append(fatal_errors);
                    all_errors.append(non_fatal_errors);

                    if (!fatal_errors.empty() && output_errors_by_throwing == OutputErrorsByThrowing::Yes)
                    {
                        throw std::runtime_error{all_errors};
                    }
                    else
                    {
                        Output::send(to_wstring(all_errors));
                    }
                }

                if (errors_only == ErrorsOnly::No)
                {
                    for (const auto& success_message : scan_result.success_messages)
                    {
                        Output::send(success_message);
                    }
                }
            };

            auto do_scan = [&](auto scanner_function, SuppressScanAttemptMessage suppress_scan_attempt_message = SuppressScanAttemptMessage::No) {
                // Modular games have much smaller binaries, therefore many scans can be completed very quickly
                static const int num_scans_before_fatal_failure = SigScannerStaticData::m_is_modular ? config.num_scan_attempts_modular : config.num_scan_attempts_normal;

                Signatures::ScanResult scan_result{};
                for (int i = 0; scan_result.scan_status == Signatures::ScanStatus::Failed && i < num_scans_before_fatal_failure; ++i)
                {
                    if (suppress_scan_attempt_message == SuppressScanAttemptMessage::No)
                    {
                        Output::send<LogLevel::Verbose>(STR("Scan attempt {}/{}\n"), i + 1, num_scans_before_fatal_failure);
                    }

                    scan_result = scanner_function(config);
                    output_result(scan_result);

                    bool has_fatal_error{};
                    for (const auto& error : scan_result.errors)
                    {
                        if (error.is_fatal)
                        {
                            has_fatal_error = true;
                            break;
                        }
                    }
                    if (!has_fatal_error) { break; }
                }

                output_result(scan_result, OutputErrorsByThrowing::Yes, ErrorsOnly::Yes);
            };

            do_scan(&Signatures::scan_for_game_functions_and_data);
            initialize_versioned_container();
            do_scan(&Signatures::scan_for_guobjectarray, SuppressScanAttemptMessage::Yes);
        }

        if (cache_info.should_serialize_cache)
        {
            create_cache(cache_info);
        }

        cache_info.game_exe_file.close();

        // Find offsets that are required for the StaticOffsetInternal implementation
        // These do not require that any objects in GUObjectArray to be initialized
        StaticOffsetFinder::find_independent_offsets(config.process_handle);

        // Objects that are required to exist before we can continue
        // Add objects to here before you use them in StaticOffsetFinder
        Hook::add_required_object(STR("/Script/CoreUObject.Class"));
        Hook::add_required_object(STR("/Script/CoreUObject"));
        Hook::add_required_object(STR("/Script/CoreUObject.Struct"));
        Hook::add_required_object(STR("/Script/Engine.Pawn"));
        Hook::add_required_object(STR("/Script/Engine.Character"));
        Hook::add_required_object(STR("/Script/Engine.Actor"));
        Hook::add_required_object(STR("/Script/CoreUObject.Vector"));
        Hook::add_required_object(STR("/Script/Engine.Default__DefaultPawn"));
        Hook::add_required_object(STR("/Script/Engine.HitResult"));
        Hook::add_required_object(STR("/Script/Engine.Default__MaterialExpression"));
        Hook::add_required_object(STR("/Script/Engine.ActorComponent"));
        Hook::add_required_object(STR("/Script/CoreUObject.OrientedBox"));
        Hook::add_required_object(STR("/Script/Engine.MovementComponent"));
        Hook::add_required_object(STR("/Script/Engine.HUD"));
        Hook::add_required_object(STR("/Script/Engine.PlayerController"));
        Hook::add_required_object(STR("/Script/Engine.PlayerCameraManager"));
        Hook::add_required_object(STR("/Script/CoreUObject.EInterpCurveMode"));
        Hook::add_required_object(STR("/Script/Engine.ENetRole"));
        Hook::add_required_object(STR("/Script/MovieScene.MovieSceneEditorData"));
        Hook::add_required_object(STR("/Script/UMG.Widget"));
        Hook::add_required_object(STR("/Script/UMG.ComboBoxString"));
        Hook::add_required_object(STR("/Script/CoreUObject.Interface"));
        Hook::add_required_object(STR("/Script/CoreUObject.DynamicClass"));

        hook_static_construct_object();
        hook_process_event();

        for (int32_t i = 0; i < 2000 && !Hook::StaticStorage::all_required_objects_constructed; ++i)
        {
            // The control variable for this loop is controlled from the game thread in a
            // hook created in the function call right above this loop

            // Sleeping here will prevent this loop from getting optimized away
            // It will also prevent unnecessarily high CPU usage
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        TypeChecker::store_all_object_names();

        Output::send(STR("Constructed {} of {} objects\n"), Hook::StaticStorage::num_required_objects_constructed, Hook::StaticStorage::required_objects_for_init.size());
        if (!Hook::StaticStorage::all_required_objects_constructed)
        {
            Output::send(STR("Fatal error! The following objects were never constructed:\n"));
            for (const auto& required_object : Hook::StaticStorage::required_objects_for_init)
            {
                if (required_object.object_constructed) { continue; }
                Output::send(STR("{}\n"), required_object.object_name);
            }
            throw std::runtime_error{""};
        }

        StaticOffsetFinder::find_offsets(config.process_handle);
        StaticOffsetFinder::output_all_member_offsets();
        Container::m_unreal_virtual_base->set_virtual_offsets();

        FMalloc::IsInitialized = true;
        StaticStorage::is_initialized = true;

    }
}
