#ifndef RC_SINGLE_PASS_SIG_SCANNER_HPP
#define RC_SINGLE_PASS_SIG_SCANNER_HPP

#include <array>
#include <vector>
#include <functional>
#include <mutex>
#include <SigScanner/Common.hpp>

#define HI_NIBBLE(b) (((b) >> 4) & 0x0F)
#define LO_NIBBLE(b) ((b) & 0x0F)

namespace RC
{
    enum class ScanTarget
    {
        MainExe,

        AIModule,
        Analytics,
        AnalyticsET,
        AnimationCore,
        AnimGraphRuntime,
        AppFramework,
        ApplicationCore,
        AssetRegistry,
        AudioCaptureCore,
        AudioCaptureRtAudio,
        AudioExtensions,
        AudioMixer,
        AudioMixerCore,
        AudioMixerXAudio2,
        AudioPlatformConfiguration,
        AugmentedReality,
        AVEncoder,
        AVIWriter,
        BuildPatchServices,
        BuildSettings,
        Cbor,
        CEF3Utils,
        Chaos,
        ChaosCore,
        ChaosSolverEngine,
        ChaosSolvers,
        CinematicCamera,
        ClothingSystemRuntimeCommon,
        ClothingSystemRuntimeInterface,
        ClothingSystemRuntimeNv,
        Core,
        CoreUObject,
        CrunchCompression,
        D3D11RHI,
        D3D12RHI,
        Engine,
        EngineMessages,
        EngineSettings,
        EyeTracker,
        FieldSystemCore,
        FieldSystemEngine,
        FieldSystemSimulationCore,
        Foliage,
        GameplayMediaEncoder,
        GameplayTags,
        GameplayTasks,
        GeometryCollectionCore,
        GeometryCollectionEngine,
        GeometryCollectionSimulationCore,
        HeadMountedDisplay,
        HTTP,
        HttpNetworkReplayStreaming,
        HTTPServer,
        Icmp,
        ImageCore,
        ImageWrapper,
        ImageWriteQueue,
        InputCore,
        InputDevice,
        InstallBundleManager,
        InstancedSplines,
        InteractiveToolsFramework,
        Json,
        JsonUtilities,
        Landscape,
        LauncherCheck,
        LauncherPlatform,
        LevelSequence,
        LocalFileNetworkReplayStreaming,
        MaterialShaderQualitySettings,
        Media,
        MediaAssets,
        MediaUtils,
        MeshDescription,
        MeshUtilitiesCommon,
        Messaging,
        MessagingCommon,
        MoviePlayer,
        MovieScene,
        MovieSceneCapture,
        MovieSceneTracks,
        MRMesh,
        NavigationSystem,
        Navmesh,
        NetCore,
        Networking,
        NetworkReplayStreaming,
        NonRealtimeAudioRenderer,
        NullDrv,
        NullNetworkReplayStreaming,
        OpenGLDrv,
        Overlay,
        PacketHandler,
        PakFile,
        PerfCounters,
        PhysicsCore,
        PhysicsSQ,
        PhysXCooking,
        PreLoadScreen,
        Projects,
        PropertyPath,
        RawMesh,
        ReliabilityHandlerComponent,
        RenderCore,
        Renderer,
        RHI,
        RSA,
        SandboxFile,
        Serialization,
        SessionMessages,
        SessionServices,
        SignalProcessing,
        Slate,
        SlateCore,
        SlateNullRenderer,
        SlateRHIRenderer,
        Sockets,
        SoundFieldRendering,
        SSL,
        StaticMeshDescription,
        StreamingPauseRendering,
        SynthBenchmark,
        TimeManagement,
        TraceLog,
        UELibSampleRate,
        UMG,
        VectorVM,
        Voice,
        Voronoi,
        VulkanRHI,
        WebBrowser,
        WindowsPlatformFeatures,
        XAudio2,

        Max,
    };

    auto RC_SIG_SCANNER_API ScanTargetToString(ScanTarget scan_target) -> std::string;
    auto RC_SIG_SCANNER_API ScanTargetToString(size_t scan_target) -> std::string;

    // Describes a module serving as a target for signature scanning
    struct RC_SIG_SCANNER_API SignatureScanModuleInfo {
        uint8_t* StartAddress;
        uint64_t ModuleSize;
    };

    // Static storage to be used across all sig scanner types
    // At the moment the only scanner type that exists is SinglePassScanner
    // In the future there might be a multi-threaded version of SinglePassScanner
    class RC_SIG_SCANNER_API SigScannerStaticData
    {
    public:
        static std::unordered_map<ScanTarget, SignatureScanModuleInfo> m_modules_info;
        static bool m_is_modular;
    };

    struct RC_SIG_SCANNER_API SignatureContainerLight
    {
        // The scanner will set this to whichever signature a match was found for
        size_t index_into_signatures{};

        // The scanner will set this to the address of the current match (if a match was found)
        // This is guaranteed to be non-null when 'callable' is called
        uint8_t* match_address{};
    };

    struct RC_SIG_SCANNER_API SignatureData
    {
        const char* signature;

        // TODO: Add 'ScanTarget' in here and remove 'ScanTarget' from the 'start_scan' function

        // The user-code can cast the custom data to their own enum type before using
        // It will be zero-defaulted in the event of no custom data being supplied
        int32_t custom_data{};
    };

    class RC_SIG_SCANNER_API SignatureContainer
    {
    private:
        // Member variables in this class shouldn't be mutable outside of the scanner internals
        // They cannot simply be private because this class isn't part of the scanner
        // They cannot be const because they need to be mutated by the scanner
        // The solution is to make everything private and give the scanner access by using the 'friend' keyword
        friend class SinglePassScanner;

    private:
        const std::vector<SignatureData> signatures;
        const std::function<bool(SignatureContainer&)> on_match_found;
        const std::function<void(SignatureContainer&)> on_scan_finished;

        // Whether to store the results and pass them to on_scan_completed
        const bool store_results{};

        // The scanner will store results in here if 'store_results' is true
        std::vector<SignatureContainerLight> result_store{};

        // True if the scan was successful, otherwise false
        bool did_succeed{false};

        // The scanner will use this to cancel all future calls to 'callable' if the 'callable' signaled
        bool ignore{};

        // The scanner will set this to whichever signature a match was found for
        size_t index_into_signatures{};

        // The scanner will set this to the address of the current match (if a match was found)
        // This is guaranteed to be non-null when 'callable' is called
        uint8_t* match_address{};

        //Address of the signature matched, relative to the base of the module that is being scanned
        //Not zero on the same conditions when match address is present
        uint64_t relative_match_address{};
        uint8_t* module_base_address{};

        // The scanner will set this to the size of the signature that was matched
        size_t match_signature_size{};

    public:
        template<typename OnMatchFound, typename OnScanFinished>
        SignatureContainer(
                std::vector<SignatureData> sig_param,
                OnMatchFound on_match_found_param,
                OnScanFinished on_scan_finished_param
        ):
                signatures(std::move(sig_param)),
                on_match_found(on_match_found_param),
                on_scan_finished(on_scan_finished_param) {}

        template<typename OnMatchFound, typename OnScanFinished>
        SignatureContainer(
                std::vector<SignatureData> sig_param,
                OnMatchFound on_match_found_param,
                OnScanFinished on_scan_finished_param,
                bool store_results_param
        ):
                signatures(std::move(sig_param)),
                on_match_found(on_match_found_param),
                on_scan_finished(on_scan_finished_param),
                store_results(store_results_param) {}

    public:
        [[nodiscard]] auto get_match_address() const -> uint8_t* { return match_address; }
        [[nodiscard]] auto get_relative_match_address() const -> uint64_t { return relative_match_address; }
        [[nodiscard]] auto get_module_base_address() const -> uint8_t* { return module_base_address; }
        [[nodiscard]] auto get_index_into_signatures() const -> size_t { return index_into_signatures; }
        [[nodiscard]] auto get_did_succeed() -> bool& { return did_succeed; }
        auto mark_scan_succeeded() -> void { this->did_succeed = true; }
        [[nodiscard]] auto get_did_succeed() const -> bool { return did_succeed; }
        [[nodiscard]] auto get_signatures() const -> const std::vector<SignatureData>& { return signatures; }
        [[nodiscard]] auto get_result_store() const -> const std::vector<SignatureContainerLight>& { return result_store; }
        [[nodiscard]] auto get_match_signature_size() const -> size_t { return match_signature_size; }
    };

    class RC_SIG_SCANNER_API SinglePassScanner
    {
    public:
        static int32_t m_num_threads;
        static int32_t m_multithreading_module_size_threshold;
        static std::mutex m_scanner_mutex;
    private:
        auto static string_to_vector(const char* signature) -> std::vector<int>;
        auto static string_to_vector(const std::vector<SignatureData>& signatures) -> std::vector<std::vector<int>>;

    public:
        auto static scanner_work_thread(uint8_t* start_address, uint8_t* end_address, uint8_t* ModuleStartAddress, std::vector<SignatureContainer>& signature_containers) -> void;

        using SignatureContainerMap = std::unordered_map<ScanTarget, std::vector<SignatureContainer>>;
        auto static start_scan(SignatureContainerMap& signature_containers) -> void;
    };
}


#endif //RC_SINGLE_PASS_SIG_SCANNER_HPP
