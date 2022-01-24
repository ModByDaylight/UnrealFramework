#include <filesystem>

#include <Unreal/Signatures.hpp>
#include <Unreal/UnrealVersion.hpp>
#include <Unreal/UObjectArray.hpp>
#include <Unreal/FString.hpp>
#include <Unreal/FMemory.hpp>
#include <SigScanner/SinglePassSigScanner.hpp>
#include <File/File.hpp>
#include <Helpers/ASM.hpp>

namespace RC::Unreal::Signatures
{
    auto scan_for_game_functions_and_data(const UnrealInitializer::Config& config) -> ScanResult
    {
        // TODO: Add some code here to deal with nothing being found due to code being injected too early
        return scan_for_game_functions_and_data_impl(config);
    }
    auto scan_for_guobjectarray(const UnrealInitializer::Config& config) -> ScanResult
    {
        // TODO: Add some code here to deal with nothing being found due to code being injected too early
        return scan_for_guobjectarray_impl(config);
    }

    auto scan_for_game_functions_and_data_impl(const UnrealInitializer::Config& config) -> ScanResult
    {
        ScanResult scan_result;

        SinglePassScanner::SignatureContainerMap signature_container_map;
        std::vector<SignatureContainer> signature_containers_coreuobject;
        std::vector<SignatureContainer> signature_containers_core;
        std::vector<SignatureContainer> signature_containers_umg;
        std::vector<SignatureContainer> signature_containers_mainexe;

        /*
        if (config.scan_overrides.static_find_object)
        {
            config.scan_overrides.static_find_object(signature_containers_coreuobject);
        }
        else
        {
            SignatureContainer staticfindobject{
                    {
                            {
                                    // 5.0 Early Access
                                    "4 8/8 9/5 C/2 4/0 8/4 8/8 9/7 4/2 4/1 8/5 5/5 7/4 1/5 4/4 1/5 6/4 1/5 7/4 8/8 B/E C/4 8/8 3/E C/6 0/4 8/8 3/F A/F F/4 8/8 B",
                            },
                            {
                                    // 4.12 to 4.26
                                    "4 8/8 9/5 C/2 4/0 8/4 8/8 9/7 4/2 4/1 8/5 5/5 7/4 1/5 4/4 1/5 6/4 1/5 7/4 8/? ?/? ?/4 8/8 3/E C/? ?/8 0/3 D/? ?/? ?/? ?/? ?/0 0",
                            },
                            {
                                    "4 8/8 9/5 C/2 4/0 8/4 8/8 9/7 4/2 4/1 8/4 8/8 9/7 C/2 4/2 0/5 5/4 1/5 4/4 1/5 5/4 1/5 6/4 1/5 7/4 8/8 B/E C/4 8/8 3/E C/6 0/8 0/3 D/4 B/F 8",
                            },
                            {
                                    "4 8/8 9/5 C/2 4/0 8/4 8/8 9/6 C/2 4/1 8/4 8/8 9/7 4/2 4/2 0/5 7/4 1/5 6/4 1/5 7/4 8/8 3/E C/6 0/8 0/3 D/? ?/? ?/? ?/? ?/? ?/4 1/0 F/B 6/E 9",
                            },
                            {
                                    "4 8/8 9/5 C/2 4/0 8/4 8/8 9/7 4/2 4/1 8/4 8/8 9/7 C/2 4/2 0/5 5/4 1/5 4/4 1/5 5/4 1/5 6/4 1/5 7/4 8/8 B/E C/4 8/8 3/E C/6 0/8 0/3 D/? ?/? ?/? ?/? ?/? ?/4 5/0 F/B 6",
                            },
                            {
                                    "4 8/8 B/C 4/5 7/4 8/8 3/E C/7 0/8 0/3 D/? ?/? ?/? ?/? ?/? ?/4 8/8 9",
                            },
                            {
                                    // 4.25 - compiled from source, WITH_CASE_PRESERVING_NAME = 1
                                    // Needs to be replaced because it interferes with the '4.12 to 4.26' aob in some games (Ghostrunner)
                                    // Maybe this could be an indirect scan ?
                                    "4 8/8 9/5 C/2 4/0 8/4 8/8 9/7 4/2 4/1 8/5 5/5 7/4 1/5 4/4 1/5 6/4 1/5 7/4 8/8 B/E C/4 8/8 1/E C/8 0/0 0/0 0/0 0"
                            },
                            {
                                    // 4.12, compiled from source
                                    "4 8/8 B/C 4/5 7/4 8/8 1/E C/9 0/0 0/0 0/0 0/8 0/3 D/? ?/? ?/? ?/? ?/? ?/4 8/8 9/6 8/F 0",
                            },
                            {
                                    // 4.25 Modularized
                                    "4 8/8 9/5 C/2 4/0 8/4 8/8 9/7 4/2 4/1 8/4 8/8 9/7 C/2 4/2 0/5 5/4 1/5 4/4 1/5 5/4 1/5 6/4 1/5 7/4 8/8 B/E C/4 8/8 3/E C/7 0/8 0/3 D/? ?/? ?/? ?/? ?/? ?/4 5/0 F/B 6/F 9",
                            },
                    },
                    // On Match Found
                    [&](SignatureContainer& self) {
                        scan_result.success_messages.emplace_back(std::format(STR("StaticFindObject address: {} <- Built-in\n"), static_cast<void*>(self.get_match_address())));
                        UObjectGlobals::setup_static_find_object_address(self.get_match_address());
                        self.get_did_succeed() = true;
                        return true;
                    },
                    // On Scan Completed
                    [&](const SignatureContainer& self) {
                        if (!self.get_did_succeed())
                        {
                            scan_result.errors.emplace_back("Was unable to find AOB for 'StaticFindObject'\nYou can supply your own in 'UE4SS_Signatures/StaticFindObject.lua'");
                        }
                    }
            };
            signature_containers_coreuobject.emplace_back(staticfindobject);
        }
        //*/

        if (config.scan_overrides.version_finder)
        {
            config.scan_overrides.version_finder(signature_containers_core);
        }
        else
        {
            enum UnrealVersionFinderSignatureType
            {
                Newest,
                Compat,
                Oldest,
                Dbd,
            };
            VersionStatus version_status{};
            SignatureContainer unreal_version_finder{
                    {
                            {
                                    // Newer version (added in 4.24 or 4.25)
                                    "E 8/? ?/? ?/? ?/? ?/E 8/? ?/? ?/? ?/? ?/4 ?/8 ?/C 0/4 8/8 D/? ?/? ?/? ?/? ?/? ?/4 8/8 D/4 D/? ?/E 8/? ?/? ?/? ?/? ?/8 3",
                                    UnrealVersionFinderSignatureType::Newest,
                            },
                            {
                                    // 4.24/4.25+ compat
                                    "E 8/? ?/? ?/? ?/? ?/E 8/? ?/? ?/? ?/? ?/4 C/8 B/C 0/4 8/8 D/? ?/? ?/? ?/? ?/? ?/4 8/8 D/4 C/2 4/5 0/E 8",
                                    UnrealVersionFinderSignatureType::Compat,
                            },
                            {
                                    // 4.25 w/ 4.26 features - Dead By Daylight, modded client, WITH_CASE_PRESERVING_NAME = 1
                                    "4 1/C 7/? ?/? ?/? ?/? ?/? ?/? ?/B 9/0 1/0 0/0 0/0 0",
                                    UnrealVersionFinderSignatureType::Dbd,
                            },
                            {
                                    // Older version
                                    "C 7/0 5/? ?/? ?/? ?/? ?/0 4/0 0/? ?/0 0/6 6",
                                    UnrealVersionFinderSignatureType::Oldest,
                            },
                    },
                    // On Match Found
                    [&](SignatureContainer& self) {
                        if (uint8_t* match_address = self.get_match_address(); match_address)
                        {
                            const int32_t custom_data = self.get_signatures()[self.get_index_into_signatures()].custom_data;
                            const UnrealVersionFinderSignatureType signature_identifier = static_cast<UnrealVersionFinderSignatureType>(custom_data);

                            switch (signature_identifier)
                            {
                                case Newest:
                                case Compat:
                                    version_status = Unreal::Version::setup(match_address, Unreal::Version::VersionType::NEW);
                                    break;
                                case Oldest:
                                    version_status = Unreal::Version::setup(match_address, Unreal::Version::VersionType::OLD);
                                    break;
                                case Dbd:
                                    version_status = Unreal::Version::setup(match_address - 2, Unreal::Version::VersionType::OLD);
                                    break;
                            }

                            if (version_status.status_code != VersionStatus::SUCCESS) { return false; }

                            if (!Version::is_atleast(4, 12))
                            {
                                // Minimum supported version is 4.12 so we'll just go onto the next aob match if less than that was detected
                                return false;
                            }
                            else if (Version::is_above(5, 40))
                            {
                                // Arbitrary version UE 5.40 is the maximum... increase this if Unreal Engine ever gets close to this version
                                // If this or a greater number is found then it's assumed to be invalid do we go onto the next aob match
                                return false;
                            }
                            else
                            {
                                scan_result.success_messages.emplace_back(std::format(STR("Detected Unreal Engine version: {}.{} (Method: {})\n"),
                                             Version::major, Version::minor,
                                             (self.get_index_into_signatures() < self.get_signatures().size() ? STR("NEW") : STR("OLD"))
                                ));
                                self.get_did_succeed() = true;
                                return true;
                            }
                        }

                        return false;
                    },
                    // On Scan Completed
                    [&]([[maybe_unused]]SignatureContainer& self) {
                        if (!self.get_did_succeed())
                        {
                            Version::major = 4;
                            Version::minor = 25;
                            auto& error = scan_result.errors.emplace_back(fmt("%S\nWas unable to find AOB for 'Unreal Engine Version', assuming 4.25.\nIf you run into problems, you can override the engine version in 'UE4SS-settings.ini, this bypasses the need for an AOB.", version_status.error_message.c_str()));
                            error.is_fatal = false;
                        }
                    }
            };
            signature_containers_core.emplace_back(unreal_version_finder);
        }

        if (config.scan_overrides.fname_to_string)
        {
            config.scan_overrides.fname_to_string(signature_containers_core);
        }
        else
        {
            enum FNameToStringSignatureType
            {
                Default,
                GoForwardsFollowCall,
            };
            SignatureContainer fname_to_string{
                    {
                            {
                                    "4 8/8 9/5 C/2 4/0 8/5 5/5 6/5 7/4 8/8 3/E C/4 0/4 8/8 B/D A/4 8/8 B/E 9/E 8/? ?/? ?/? ?/? ?/4 8/8 B/F 0/4 8/8 5/C 0/7 5/6 1/4 8/8 D",
                            },
                            {
                                    "4 8/8 9/5 C/2 4/0 8/4 8/8 9/7 4/2 4/1 0/4 8/8 9/7 C/2 4/1 8/4 1/5 6/4 8/8 3/E C/2 0/4 8/8 9/D 3/4 9/8 9/C E/E 8/? ?/? ?/? ?/? ?/4 9/8 9",
                            },
                            {
                                    "4 8/8 9/5 C/2 4/1 0/4 8/8 9/6 C/2 4/1 8/4 8/8 9/7 4/2 4/2 0/5 7/4 8/8 3/E C/2 0/8 B/0 1/4 8/8 B/D A/8 B/F 8/4 4/0 F/B 7/C 0/C 1/E F/1 0/4 8/8 B/F 1/8 0/3 D/? ?/? ?/? ?/? ?/? ?/8 9/7 C/2 4/3 0",
                            },
                            {
                                    // 4.22 - compiled from editor
                                    "C 3/3 3/C 0/4 8/8 D/5 4/2 4/2 0/4 8/8 B/C F/4 8/8 9/4 4/2 4/2 0/4 8/8 9/4 4/2 4/2 8/E 8",
                                    FNameToStringSignatureType::GoForwardsFollowCall,
                            },
                            {
                                    "4 8/8 9/5 C/2 4/0 8/4 8/8 9/7 4/2 4/1 0/4 8/8 9/7 C/2 4/1 8/4 1/5 6/4 8/8 3/E C/2 0/4 8/8 B/D A/4 C/8 B/F 1/E 8/? ?/? ?/? ?/? ?/4 C/8 B/C 8/4 1",
                            },
                            {
                                    "4 8/8 9/5 C/2 4/1 0/4 8/8 9/6 C/2 4/1 8/4 8/8 9/7 4/2 4/2 0/5 7/4 8/8 3/E C/2 0/8 B/0 1/4 8/8 B/D A/8 B/F 8/4 4/0 F/B 7/C 0/C 1/E F/1 0/4 8/8 B/F 1/8 0/3 D/? ?/? ?/? ?/? ?/? ?/8 9/7 C",
                            },
                            {
                                    "4 8/8 9/5 C/2 4/0 8/5 7/4 8/8 3/E C/2 0/4 8/8 B/D A/4 8/8 B/F 9/E 8/? ?/? ?/? ?/? ?/4 C/8 B/C 8/8 B/0 7",
                            },
                            {
                                    // 4.25 - compiled from source, WITH_CASE_PRESERVING_NAME = 1
                                    "4 8/8 9/5 C/2 4/1 0/4 8/8 9/6 C/2 4/1 8/4 8/8 9/7 4/2 4/2 0/5 7/4 8/8 3/E C/2 0/8 B/4 1/0 4/4 8/8 B/D A/8 B/F 8"
                            },
                            {
                                    // 4.25 Modularized
                                    "4 8/8 9/5 C/2 4/1 0/5 5/5 6/5 7/4 8/8 3/E C/4 0/8 B/0 1/4 8/8 B/D A/8 B/F 8/4 4/0 F/B 7/C 0/C 1/E F/1 0/4 8/8 B/F 1",
                            },
                    },
                    // On Match Found
                    [&](SignatureContainer& self) {
                        const FNameToStringSignatureType signature_identifier = static_cast<FNameToStringSignatureType>(self.get_signatures()[self.get_index_into_signatures()].custom_data);
                        if (signature_identifier == FNameToStringSignatureType::GoForwardsFollowCall)
                        {
                            uint8_t* a = Helper::ASM::follow_call(self.get_match_address() + self.get_match_signature_size() - 1);
                            FName::to_string_internal.assign_address(a);
                        }
                        else
                        {

                            FName::to_string_internal.assign_address(self.get_match_address());
                        }

                        scan_result.success_messages.emplace_back(std::format(STR("FName::ToString address: {} <- Built-in\n"), static_cast<void*>(self.get_match_address())));
                        self.get_did_succeed() = true;
                        return true;
                    },
                    // On Scan Completed
                    [&](const SignatureContainer& self) {
                        if (!self.get_did_succeed())
                        {
                            scan_result.errors.emplace_back("Was unable to find AOB for 'FName::ToString'\nYou can supply your own in 'UE4SS_Signatures/FName_ToString.lua'");
                        }
                    }
            };
            signature_containers_core.emplace_back(fname_to_string);
        }

        if (config.scan_overrides.fname_constructor)
        {
            config.scan_overrides.fname_constructor(signature_containers_core);
        }
        else
        {
            auto resolve_fname_jmp = [&](uint8_t* ptr, int32_t offset, ScanResult& scan_result, bool call_instead_of_jmp = false) -> bool {
                uint8_t* instr = ptr + offset;

                uint8_t* destination;
                if (call_instead_of_jmp)
                {
                    destination = Helper::ASM::follow_call(instr);
                }
                else
                {
                    destination = Helper::ASM::follow_jmp(instr);
                }

                if (!destination) { return false; }

                FName name = FName(L"bCanBeDamaged", destination);

                if (name == L"bCanBeDamaged")
                {
                    scan_result.success_messages.emplace_back(std::format(STR("FName::FName address: {} <- Built-in\n"), static_cast<void*>(destination)));
                    FName::constructor_internal.assign_address(destination);
                    return true;
                }
                else
                {
                    return false;
                }
            };

            enum FNameConstructorSignatureType
            {
                Default,
                Modularized_425,
            };

            SignatureContainer fname_constructor{
                    {
                            {
                                    "4 1/B 8/0 1/0 0/0 0/0 0/4 8/8 D/1 5/? ?/? ?/? ?/? ?/4 8/8 D/0 D/? ?/? ?/? ?/? ?/E 9",
                            },
                            {
                                    // 4.25 Modularized
                                    "E 8/? ?/? ?/? ?/? ?/4 8/8 B/4 4/2 4/3 0/4 8/8 9/? ?/? ?/? ?/? ?/? ?/C 7/0 5",
                                    FNameConstructorSignatureType::Modularized_425,
                            },
                    },
                    // On Match Found
                    []([[maybe_unused]]const SignatureContainer& self) {
                        return false;
                    },
                    // On Scan Completed
                    [&](SignatureContainer& self) {
                        bool success{};

                        for (const auto& result_container : self.get_result_store())
                        {
                            const int32_t custom_data = self.get_signatures()[result_container.index_into_signatures].custom_data;
                            const FNameConstructorSignatureType signature_identifier = static_cast<const FNameConstructorSignatureType>(custom_data);

                            if (signature_identifier == FNameConstructorSignatureType::Default)
                            {
                                success = resolve_fname_jmp(result_container.match_address, 0x14, scan_result);
                            }
                            else if (signature_identifier == FNameConstructorSignatureType::Modularized_425)
                            {
                                success = resolve_fname_jmp(result_container.match_address, 0x0, scan_result, true);
                            }

                            if (success) { break; }
                        }

                        if (!success)
                        {
                            scan_result.errors.emplace_back("Was unable to find AOB for 'FName::FName'\nYou can supply your own in 'UE4SS_Signatures/FName_Constructor.lua'");
                        }
                        else
                        {
                            self.get_did_succeed() = true;
                        }
                    }, true // true = store results
            };
            signature_containers_core.emplace_back(fname_constructor);
        }

        if (config.scan_overrides.process_event)
        {
            config.scan_overrides.process_event(signature_containers_coreuobject);
        }
        else
        {
            SignatureContainer process_event{
                    {
                            {
                                    "4 0/5 5/5 6/5 7/4 1/5 4/4 1/5 5/4 1/5 6/4 1/5 7/4 8/8 1/E C/F 0/0 0/0 0/0 0/4 8/8 D/6 C/2 4/3 0/4 8/8 9/9 D/1 8/0 1/0 0/0 0/4 8/8 B/? ?/? ?/? ?/? ?/? ?/4 8/3 3/C 5/4 8/8 9/8 5",
                            },
                            {
                                    "4 0/5 5/5 6/5 7/4 1/5 4/4 1/5 5/4 1/5 6/4 1/5 7/4 8/8 1/E C/A 0/0 1/0 0/0 0/4 8/8 D/6 C/2 4/3 0/4 8/8 9/9 D/C 8/0 1/0 0/0 0/4 8/8 B/? ?/? ?/? ?/? ?/? ?/4 8/3 3/C 5/4 8/8 9/8 5",
                            },
                    },
                    // On Match Found
                    [&](SignatureContainer& self) {
                        UObject::process_event_internal.assign_address(self.get_match_address());

                        scan_result.success_messages.emplace_back(std::format(STR("UObject::ProcessEvent: {} <- Built-in\n"), static_cast<void*>(self.get_match_address())));
                        self.get_did_succeed() = true;
                        return true;
                    },
                    // On Scan Completed
                    [&](const SignatureContainer& self) {
                        if (!self.get_did_succeed())
                        {
                            scan_result.errors.emplace_back("Was unable to find AOB for 'UObject::ProcessEvent'\nYou can supply your own in 'UE4SS_Signatures/ProcessEvent.lua'");
                        }
                    }
            };
            signature_containers_coreuobject.emplace_back(process_event);
        }

        if (config.scan_overrides.static_construct_object)
        {
            config.scan_overrides.static_construct_object(signature_containers_umg);
        }
        else
        {
            SignatureContainer static_construct_object{
                    {
                            {
                                    "C 0/E 9/0 2/3 2/8 8/? ?/? ?/? ?/? ?/8 0/E 1/0 1/3 0/8 8/? ?/? ?/? ?/? ?/4 8",
                            },
                            {
                                    // 4.12 (FirstPerson C++)
                                    "8 9/8 E/C 8/0 3/0 0/0 0/3 B/8 E/C C/0 3/0 0/0 0/7 E/0 F/4 1/8 B/D 6/4 8/8 D/8 E/C 0/0 3/0 0/0 0",
                            },
                    },
                    // On Match Found
                    [&](SignatureContainer& self) {
                        uint8_t* call_instr = static_cast<uint8_t*>(self.get_match_address()) - 0x13;
                        uint8_t* callee = Helper::ASM::follow_call(call_instr);

                        // If no callee, try go back one more byte, the call instruction might be prefixed with 0xFF
                        if (!callee)
                        {
                            --call_instr;
                            callee = Helper::ASM::follow_call(call_instr);
                        }

                        if (!callee) { return false; }

                        scan_result.success_messages.emplace_back(std::format(STR("StaticConstructObject_Internal address: {} <- Built-in\n"), static_cast<void*>(callee)));
                        UObjectGlobals::setup_static_construct_object_internal_address(callee);
                        self.get_did_succeed() = true;
                        return true;

                    },
                    // On Scan Completed
                    [&](const SignatureContainer& self) {
                        if (!self.get_did_succeed())
                        {
                            scan_result.errors.emplace_back("Was unable to find address for 'StaticConstructObject_Internal'\nYou can supply your own in 'UE4SS_Signatures/StaticConstructObject'");
                        }
                    }
            };
            signature_containers_umg.emplace_back(static_construct_object);
        }

        signature_container_map.emplace(ScanTarget::CoreUObject, signature_containers_coreuobject);
        signature_container_map.emplace(ScanTarget::Core, signature_containers_core);
        signature_container_map.emplace(ScanTarget::UMG, signature_containers_umg);
        signature_container_map.emplace(ScanTarget::MainExe, signature_containers_mainexe);

        SinglePassScanner::start_scan(signature_container_map);

        if (scan_result.errors.empty())
        {
            scan_result.scan_status = ScanStatus::Succeeded;
        }
        return scan_result;
    }
    auto scan_for_guobjectarray_impl(const UnrealInitializer::Config& config) -> ScanResult
    {
        ScanResult scan_result;

        SinglePassScanner::SignatureContainerMap signature_container_map;
        std::vector<SignatureContainer> signature_containers_coreuobject;
        std::vector<SignatureContainer> signature_containers_core;

        // FMemory stuff needs to be scanned in the second pass
        // This is because we need access to the engine version which we don't until after the first pass
        if (config.scan_overrides.fmemory_free)
        {
            config.scan_overrides.fmemory_free(signature_containers_core);
        }
        else
        {
            SignatureContainer fmemory_free{
                    {
                            {
                                    // 4.12, 4.22, 4.25, 4.26
                                    "4 8/8 5/C 9/7 4/2 E/5 3/4 8/8 3/E C/2 0/4 8/8 B/D 9/4 8/8 B/? ?/? ?/? ?/? ?/? ?/4 8/8 5/C 9",
                            },
                    },
                    // On Match Found
                    [&](SignatureContainer& self) {
                        scan_result.success_messages.emplace_back(std::format(STR("FMemory::Free address: {} <- Built-in\n"), static_cast<void*>(self.get_match_address())));
                        FMemory::free.assign_address(self.get_match_address());
                        self.get_did_succeed() = true;

                        // Find the second MOV instruction and resolve it
                        // Ideally a disassembler is used to guarantee that we are at the second MOV instruction
                        // But seeing as FMemory::Free has never really changed, we can take a shortcut and just count the bytes to the second MOV instruction
                        // 13 bytes (0xD)
                        // This MOV instruction behaves like a LEA instruction
                        uint8_t* mov_instruction = self.get_match_address() + 0xD;
                        // Instruction size, including REX and ModR
                        constexpr uint8_t instr_size = 0x7;
                        uint8_t* next_instruction = mov_instruction + instr_size;
                        uint32_t* offset = std::bit_cast<uint32_t*>(mov_instruction + 0x3);
                        gmalloc = *std::bit_cast<FMalloc**>(next_instruction + *offset);
                        FMalloc::malloc_internal.assign_address(gmalloc->get_vtable_entry(2));

                        if (Version::is_below(4, 25))
                        {
                            FMalloc::free_internal.assign_address(gmalloc->get_vtable_entry(4));
                        }
                        else
                        {
                            FMalloc::free_internal.assign_address(gmalloc->get_vtable_entry(6));
                        }

                        return true;
                    },
                    // On Scan Completed
                    [&](const SignatureContainer& self) {
                        if (!self.get_did_succeed())
                        {
                            auto& error = scan_result.errors.emplace_back("Was unable to find AOB for 'FMemory::Free'\nYou can supply your own in 'UE4SS_Signatures/FMemory_Free.lua");
                            error.is_fatal = false;
                        }
                    }
            };
            signature_containers_core.emplace_back(fmemory_free);
        }

        if (config.scan_overrides.guobjectarray)
        {
            config.scan_overrides.guobjectarray(signature_containers_coreuobject);
        }
        else
        {
            SignatureContainer guobjectarray = [&]() -> SignatureContainer {
                if (Version::is_atmost(4, 12))
                {
                    return {
                            {
                                    {
                                            "8 B/5 1/0 4/8 5/D 2/7 4/5 A/4 8/6 3/0 1/8 5/C 0/7 8/5 3/4 4/8 B/? ?/? ?/? ?/? ?/? ?/4 1/3 B/C 0/7 D/4 7/4 C/8 B",
                                    },
                            },
                            [&](SignatureContainer& self) {
                                uint8_t* mov_from = Helper::ASM::resolve_mov<uint8_t*>(static_cast<uint8_t*>(self.get_match_address()) + 0x1A);
                                if (!mov_from)
                                {
                                    scan_result.errors.emplace_back("Was unable to find GUObjectArray: mov source is nullptr\nYou can supply your own in 'UE4SS_Signatures/GUObjectArray.lua'");
                                    return true;
                                }

                                scan_result.success_messages.emplace_back(std::format(STR("GUObjectArray address: {} [sig#: {}] <- Built-in\n"), (void*)(mov_from - 0x10), self.get_index_into_signatures() + 1));
                                UObjectArray::setup_guobjectarray_address(mov_from - 0x10);

                                self.get_did_succeed() = true;
                                return true;
                            },
                            [&](const SignatureContainer& self) {
                                if (!self.get_did_succeed())
                                {
                                    scan_result.errors.emplace_back("Was unable to find AOB for 'GUObjectArray'\nYou can supply your own in 'UE4SS_Signatures/GUObjectArray.lua'");
                                }
                            }
                    };
                }
                else if (Version::is_atmost(4, 18))
                {
                    return {
                            {
                                    {
                                            "4 8/8 9/5 C/2 4/0 8/4 8/8 9/7 4/2 4/1 0/5 7/4 8/8 3/E C/2 0/3 3/F 6/C 7/4 1/0 4/F F/F F/F F/F F/8 9",
                                    },
                            },
                            [&](SignatureContainer& self) {
                                // Note that the 0x8C offset is only accurate in some games
                                // I've found that in certain games (for example using engine 4.22) the offset may be 0x94
                                // This particular aob may not be great to use
                                uint8_t* lea_from = Helper::ASM::resolve_lea<uint8_t*>(static_cast<uint8_t*>(self.get_match_address()) + 0x8C);
                                if (!lea_from)
                                {
                                    scan_result.errors.emplace_back("Was unable to find GUObjectArray: lea source is nullptr\nYou can supply your own in 'UE4SS_Signatures/GUObjectArray.lua'");
                                    return true;
                                }

                                scan_result.success_messages.emplace_back(std::format(STR("GUObjectArray address: {} [sig#: {}] <- Built-in\n"), (void*)(lea_from - 0x10), self.get_index_into_signatures() + 1));
                                UObjectArray::setup_guobjectarray_address(lea_from - 0x10);

                                self.get_did_succeed() = true;
                                return true;
                            },
                            [&](const SignatureContainer& self) {
                                if (!self.get_did_succeed())
                                {
                                    scan_result.errors.emplace_back("Was unable to find AOB for 'GUObjectArray'\nYou can supply your own in 'UE4SS_Signatures/GUObjectArray.lua'");
                                }
                            }
                    };
                }
                else
                {
                    return {
                            {
                                    {
                                            "? ?/? ?/0 ?/0 0/? ?/? ?/? ?/? ?/? ?/? ?/0 ?/0 0/0 0/0 0/0 0/0 0/? ?/? ?/? ?/? ?/? ?/? ?/0 0/0 0/0 0/0 0/0 0/0 0/0 0/0 0/0 0/0 0/0 0/? 0/2 ?/0 0/? ?/? ?/? ?/0 0/2 ?/0 0/0 0/0 0/0 ?/0 0/0 0/0 0/F F/F F/F F/F F/F F/F F/F F/F F/F F",
                                    },
                                    {
                                            "5 9/D C/0 0/0 0/5 8/D C/0 0/0 0/5 9/D C/0 0/0 0/0 0/0 0/0 0/0 0/A 0/1 2/? ?/? ?/? ?/? ?/0 0/0 0/0 0/0 0/0 0/0 0/0 0/0 0/0 0/0 0/0 0/0 0/2 1/0 0/? ?/? ?/? ?/0 0/2 1/0 0/0 0/0 0/? ?/0 0/0 0/0 0",
                                    },
                            },
                            [&](SignatureContainer& self) {
                                scan_result.success_messages.emplace_back(std::format(STR("GUObjectArray address: {} [sig#: {}] <- Built-in\n"), (void*)self.get_match_address(), self.get_index_into_signatures() + 1));
                                UObjectArray::setup_guobjectarray_address(self.get_match_address());

                                self.get_did_succeed() = true;
                                return true;
                            },
                            [&](const SignatureContainer& self) {
                                if (!self.get_did_succeed())
                                {
                                    scan_result.errors.emplace_back("Was unable to find AOB for 'GUObjectArray'\nYou can supply your own in 'UE4SS_Signatures/GUObjectArray.lua'");
                                }
                            }
                    };
                }
            }();

            signature_containers_coreuobject.emplace_back(guobjectarray);
        }

        signature_container_map.emplace(ScanTarget::Core, signature_containers_core);
        signature_container_map.emplace(ScanTarget::CoreUObject, signature_containers_coreuobject);
        SinglePassScanner::start_scan(signature_container_map);

        if (scan_result.errors.empty())
        {
            scan_result.scan_status = Signatures::ScanStatus::Succeeded;
        }
        return scan_result;
    }
}