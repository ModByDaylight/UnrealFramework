static std::unordered_map<std::wstring, int32_t> MemberOffsets;

public:
    int32 GetActiveLevelCollectionIndex();
    const int32 GetActiveLevelCollectionIndex() const;

public:
    uint32 GetAudioDeviceHandle();
    const uint32 GetAudioDeviceHandle() const;

public:
    float GetAudioTimeSeconds();
    const float GetAudioTimeSeconds() const;

public:
    int32 GetBlockTillLevelStreamingCompletedEpoch();
    const int32 GetBlockTillLevelStreamingCompletedEpoch() const;

public:
    double GetBuildStreamingDataTimer();
    const double GetBuildStreamingDataTimer() const;

public:
    uint32 GetCleanupWorldTag();
    const uint32 GetCleanupWorldTag() const;

public:
    FName GetCommittedPersistentLevelName();
    const FName GetCommittedPersistentLevelName() const;

public:
    FName GetDebugDrawTraceTag();
    const FName GetDebugDrawTraceTag() const;

public:
    float GetDeltaRealTimeSeconds();
    const float GetDeltaRealTimeSeconds() const;

public:
    float GetDeltaTimeSeconds();
    const float GetDeltaTimeSeconds() const;

public:
    TArray<UObject *,FDefaultAllocator> GetExtraReferencedObjects();
    const TArray<UObject *,FDefaultAllocator> GetExtraReferencedObjects() const;

public:
    bool GetFullPurgeTriggered();
    const bool GetFullPurgeTriggered() const;

public:
    uint32 GetIsInBlockTillLevelStreamingCompleted();
    const uint32 GetIsInBlockTillLevelStreamingCompleted() const;

public:
    uint32 GetLWILastAssignedUID();
    const uint32 GetLWILastAssignedUID() const;

public:
    double GetLastTimeUnbuiltLightingWasEncountered();
    const double GetLastTimeUnbuiltLightingWasEncountered() const;

public:
    TArray<AActor *,TSizedDefaultAllocator<32> > GetLevelSequenceActors();
    const TArray<AActor *,TSizedDefaultAllocator<32> > GetLevelSequenceActors() const;

public:
    float GetNextSwitchCountdown();
    const float GetNextSwitchCountdown() const;

public:
    FString GetNextURL();
    const FString GetNextURL() const;

public:
    uint32 GetNumInvalidReflectionCaptureComponents();
    const uint32 GetNumInvalidReflectionCaptureComponents() const;

public:
    uint32 GetNumLightingUnbuiltObjects();
    const uint32 GetNumLightingUnbuiltObjects() const;

public:
    uint16 GetNumStreamingLevelsBeingLoaded();
    const uint16 GetNumStreamingLevelsBeingLoaded() const;

public:
    int32 GetNumTextureStreamingDirtyResources();
    const int32 GetNumTextureStreamingDirtyResources() const;

public:
    int32 GetNumTextureStreamingUnbuiltComponents();
    const int32 GetNumTextureStreamingUnbuiltComponents() const;

public:
    uint32 GetNumUnbuiltReflectionCaptures();
    const uint32 GetNumUnbuiltReflectionCaptures() const;

public:
    FVector GetOriginOffsetThisFrame();
    const FVector GetOriginOffsetThisFrame() const;

public:
    float GetPauseDelay();
    const float GetPauseDelay() const;

public:
    TArray<UObject *,FDefaultAllocator> GetPerModuleDataObjects();
    const TArray<UObject *,FDefaultAllocator> GetPerModuleDataObjects() const;

public:
    int32 GetPlayerNum();
    const int32 GetPlayerNum() const;

public:
    TArray<FName,FDefaultAllocator> GetPreparingLevelNames();
    const TArray<FName,FDefaultAllocator> GetPreparingLevelNames() const;

public:
    float GetRealTimeSeconds();
    const float GetRealTimeSeconds() const;

public:
    FString GetStreamingLevelsPrefix();
    const FString GetStreamingLevelsPrefix() const;

public:
    int32 GetStreamingVolumeUpdateDelay();
    const int32 GetStreamingVolumeUpdateDelay() const;

public:
    float GetTimeSeconds();
    const float GetTimeSeconds() const;

public:
    float GetTimeSinceLastPendingKillPurge();
    const float GetTimeSinceLastPendingKillPurge() const;

public:
    float GetUnpausedTimeSeconds();
    const float GetUnpausedTimeSeconds() const;

public:
    TArray<FVector,FDefaultAllocator> GetViewLocationsRenderedLastFrame();
    const TArray<FVector,FDefaultAllocator> GetViewLocationsRenderedLastFrame() const;

public:
    uint32 GetbActorsInitialized();
    const uint32 GetbActorsInitialized() const;

public:
    uint32 GetbAggressiveLOD();
    const uint32 GetbAggressiveLOD() const;

public:
    uint32 GetbAllowAudioPlayback();
    const uint32 GetbAllowAudioPlayback() const;

public:
    bool GetbAllowDeferredPhysicsStateCreation();
    const bool GetbAllowDeferredPhysicsStateCreation() const;

public:
    uint32 GetbAreConstraintsDirty();
    const uint32 GetbAreConstraintsDirty() const;

public:
    uint32 GetbBegunPlay();
    const uint32 GetbBegunPlay() const;

public:
    uint32 GetbCleanedUpWorld();
    const uint32 GetbCleanedUpWorld() const;

public:
    bool GetbCreateRenderStateForHiddenComponents();
    const bool GetbCreateRenderStateForHiddenComponents() const;

public:
    bool GetbDebugDrawAllTraceTags();
    const bool GetbDebugDrawAllTraceTags() const;

public:
    uint32 GetbDebugFrameStepExecution();
    const uint32 GetbDebugFrameStepExecution() const;

public:
    uint32 GetbDebugPauseExecution();
    const uint32 GetbDebugPauseExecution() const;

public:
    bool GetbDoDelayedUpdateCullDistanceVolumes();
    const bool GetbDoDelayedUpdateCullDistanceVolumes() const;

public:
    uint32 GetbDropDetail();
    const uint32 GetbDropDetail() const;

public:
    bool GetbHack_Force_UsesGameHiddenFlags_True();
    const bool GetbHack_Force_UsesGameHiddenFlags_True() const;

public:
    bool GetbInTick();
    const bool GetbInTick() const;

public:
    uint8 GetbInitializedAndNeedsCleanup();
    const uint8 GetbInitializedAndNeedsCleanup() const;

public:
    bool GetbIsBuilt();
    const bool GetbIsBuilt() const;

public:
    uint32 GetbIsCameraMoveableWhenPaused();
    const uint32 GetbIsCameraMoveableWhenPaused() const;

public:
    uint32 GetbIsDefaultLevel();
    const uint32 GetbIsDefaultLevel() const;

public:
    bool GetbIsLevelStreamingFrozen();
    const bool GetbIsLevelStreamingFrozen() const;

public:
    bool GetbIsRunningConstructionScript();
    const bool GetbIsRunningConstructionScript() const;

public:
    uint32 GetbIsTearingDown();
    const uint32 GetbIsTearingDown() const;

public:
    bool GetbIsWorldInitialized();
    const bool GetbIsWorldInitialized() const;

public:
    uint32 GetbKismetScriptError();
    const uint32 GetbKismetScriptError() const;

public:
    uint32 GetbMarkedObjectsPendingKill();
    const uint32 GetbMarkedObjectsPendingKill() const;

public:
    uint32 GetbMatchStarted();
    const uint32 GetbMatchStarted() const;

public:
    uint8 GetbMaterialParameterCollectionInstanceNeedsDeferredUpdate();
    const uint8 GetbMaterialParameterCollectionInstanceNeedsDeferredUpdate() const;

public:
    bool GetbOriginOffsetThisFrame();
    const bool GetbOriginOffsetThisFrame() const;

public:
    uint32 GetbPlayersOnly();
    const uint32 GetbPlayersOnly() const;

public:
    uint32 GetbPlayersOnlyPending();
    const uint32 GetbPlayersOnlyPending() const;

public:
    bool GetbPostTickComponentUpdate();
    const bool GetbPostTickComponentUpdate() const;

public:
    uint32 GetbRequestedBlockOnAsyncLoading();
    const uint32 GetbRequestedBlockOnAsyncLoading() const;

public:
    bool GetbRequiresHitProxies();
    const bool GetbRequiresHitProxies() const;

public:
    bool GetbShouldDelayGarbageCollect();
    const bool GetbShouldDelayGarbageCollect() const;

public:
    bool GetbShouldForceUnloadStreamingLevels();
    const bool GetbShouldForceUnloadStreamingLevels() const;

public:
    bool GetbShouldForceVisibleStreamingLevels();
    const bool GetbShouldForceVisibleStreamingLevels() const;

public:
    bool GetbShouldSimulatePhysics();
    const bool GetbShouldSimulatePhysics() const;

public:
    bool GetbShouldTick();
    const bool GetbShouldTick() const;

public:
    uint32 GetbStartup();
    const uint32 GetbStartup() const;

public:
    uint32 GetbStreamingDataDirty();
    const uint32 GetbStreamingDataDirty() const;

public:
    bool GetbTickNewlySpawned();
    const bool GetbTickNewlySpawned() const;

public:
    uint32 GetbTriggerPostLoadMap();
    const uint32 GetbTriggerPostLoadMap() const;

public:
    uint32 GetbWorldWasLoadedThisTick();
    const uint32 GetbWorldWasLoadedThisTick() const;

