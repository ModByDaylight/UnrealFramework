#include <filesystem>

#include <Unreal/Signatures.hpp>
#include <Unreal/UnrealVersion.hpp>
#include <Unreal/UObjectArray.hpp>
#include <Unreal/UObject.hpp>
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
        std::vector<SignatureContainer> signature_containers_engine;
        std::vector<SignatureContainer> signature_containers_umg;
        std::vector<SignatureContainer> signature_containers_mainexe;

        uint8_t* FNameToStringAddress{};
        uint8_t FNameToStringNumMatches{};

        if (config.scan_overrides.version_finder)
        {
            config.scan_overrides.version_finder(signature_containers_core, scan_result);
        }
        else
        {
            VersionStatus version_status{};
            SignatureContainer unreal_version_finder{
                    {
                            {
                                    // 4.x.x
                                    "0 4/0 0/? ?/0 0/? ?/0 0/0 0/0 0/? ?/? ?/? ?/? ?/0 0/0 0/0 0/0 0/? ?/? ?/? ?/? ?/? ?/? ?/0 0/0 0/? ?/0 0/0 0/0 0/? ?/0 0/0 0/0 0/0 4/0 0/? ?/0 0/? ?/0 0/0 0/0 0",
                            },
                            {
                                    // 5.x.x
                                    "0 5/0 0/? ?/0 0/? ?/0 0/0 0/0 0/? ?/? ?/? ?/? ?/0 0/0 0/0 0/0 0/? ?/? ?/? ?/? ?/? ?/? ?/0 0/0 0/? ?/0 0/0 0/0 0/? ?/0 0/0 0/0 0/0 4/0 0/? ?/0 0/? ?/0 0/0 0/0 0",
                            },
                    },
                    // On Match Found
                    [&]([[maybe_unused]]SignatureContainer& self) {
                        version_status = Unreal::Version::setup(config, self.get_match_address());
                        return version_status.status_code == VersionStatus::SUCCESS;
                    },
                    // On Scan Completed
                    [&]([[maybe_unused]]SignatureContainer& self) {
                        if (version_status.status_code != VersionStatus::SUCCESS)
                        {
                            scan_result.errors.emplace_back(std::format("{}\nWas unable to find AOB for 'Unreal Engine Version'.\nYou need to override the engine version in 'UE4SS-settings.ini.", to_string(version_status.error_message)));
                        }
                    }
            };
            signature_containers_core.emplace_back(unreal_version_finder);
        }

        if (config.scan_overrides.fname_to_string)
        {
            // If we have an override look in the 'Core' module because that's where FName::ToString is
            config.scan_overrides.fname_to_string(signature_containers_core, scan_result);
        }
        else
        {
            // If we don't have an override, look in whatever module has a reference to FName::ToString
            SignatureContainer fname_to_string{
                    {
                            {
                                    // 4.12 - 5.0 EA
                                    "E 8/? ?/? ?/? ?/? ?/4 8/8 B/4 C/2 4/? ?/8 B/F D/4 8/8 5/C 9",
                            },
                            {
                                    // 4.12 - 5.0 EA, Modular games (i.e. Satisfactory)
                                    "F F/1 5/? ?/? ?/? ?/? ?/8 0/3 D/? ?/? ?/? ?/? ?/? ?/7 2/3 A/4 4/3 9/7 4/2 4/4 8",
                            },
                    },
                    // On Match Found
                    [&](SignatureContainer& self) {
                        FNameToStringAddress = Helper::ASM::follow_call(self.get_match_address());
                        ++FNameToStringNumMatches;
                        self.get_did_succeed() = true;
                        return true;
                    },
                    // On Scan Completed
                    [&]([[maybe_unused]]const SignatureContainer& self) {
                    }
            };
            signature_containers_engine.emplace_back(fname_to_string);

            SignatureContainer fname_to_string_backup{
                    {
                            {
                                    // 4.25+ Backup (sometimes the first one fails)
                                    "E 8/? ?/? ?/? ?/? ?/B D/0 1/0 0/0 0/0 0/4 1/3 9/6 E/? ?/0 F/8 E",
                            },
                            {
                                    // 4.25+ Backup, Modular games (i.e. Satisfactory) (sometimes the first one fails)
                                    "F F/1 5/? ?/? ?/? ?/? ?/4 1/B E/0 1/0 0/0 0/0 0/4 5/3 9/7 5/4 8",
                            },
                    },
                    // On Match Found
                    [&](SignatureContainer& self) {
                        FNameToStringAddress = Helper::ASM::follow_call(self.get_match_address());
                        self.get_did_succeed() = true;
                        ++FNameToStringNumMatches;
                        return true;
                    },
                    // On Scan Completed
                    [&]([[maybe_unused]]const SignatureContainer& self) {
                    }
            };
            signature_containers_coreuobject.emplace_back(fname_to_string_backup);
        }

        if (config.scan_overrides.static_construct_object)
        {
            config.scan_overrides.static_construct_object(signature_containers_umg, scan_result);
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
        signature_container_map.emplace(ScanTarget::Engine, signature_containers_engine);
        signature_container_map.emplace(ScanTarget::UMG, signature_containers_umg);
        signature_container_map.emplace(ScanTarget::MainExe, signature_containers_mainexe);

        SinglePassScanner::start_scan(signature_container_map);

        // Special error handler for FName::ToString
        // Needed because it checks in more than one module and the scanner isn't setup for that
        if (FNameToStringNumMatches > 1)
        {
            scan_result.info_messages.emplace(STR("Found FName::ToString in both Engine & CoreUObject modules, using the first match\n"));
        }

        if (FNameToStringNumMatches >= 1 && FNameToStringAddress)
        {
            FName::to_string_internal.assign_address(FNameToStringAddress);
            scan_result.success_messages.emplace_back(std::format(STR("FName::ToString address: {} <- Built-in\n"), static_cast<void*>(FNameToStringAddress)));
        }
        else
        {
            scan_result.errors.emplace_back("Was unable to find AOB for 'FName::ToString'\nYou can supply your own in 'UE4SS_Signatures/FName_ToString.lua'");
        }

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

        // FName:FName has to be in the second pass because we need access to FName::ToString which is found in the first pass
        if (config.scan_overrides.fname_constructor)
        {
            config.scan_overrides.fname_constructor(signature_containers_core, scan_result);
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

        // FMemory stuff needs to be scanned in the second pass
        // This is because we need access to the engine version which we don't until after the first pass
        if (config.scan_overrides.fmemory_free)
        {
            config.scan_overrides.fmemory_free(signature_containers_core, scan_result);
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
                        FMalloc::UnrealStaticGMalloc = std::bit_cast<FMalloc**>(next_instruction + *offset);
                        GMalloc = *FMalloc::UnrealStaticGMalloc;

                        return true;
                    },
                    // On Scan Completed
                    [&](const SignatureContainer& self) {
                        if (!self.get_did_succeed())
                        {
                            scan_result.errors.emplace_back("Was unable to find AOB for 'FMemory::Free'\nYou can supply your own in 'UE4SS_Signatures/FMemory_Free.lua");
                        }
                    }
            };
            signature_containers_core.emplace_back(fmemory_free);
        }

        if (config.scan_overrides.guobjectarray)
        {
            config.scan_overrides.guobjectarray(signature_containers_coreuobject, scan_result);
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