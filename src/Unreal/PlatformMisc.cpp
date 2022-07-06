#include "Unreal/PlatformMisc.hpp"
#include "UnrealVFS/Paths.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

NS_RC_UE_START

const TCHAR* FPlatformMisc::BaseDir() {
    static FString BaseDir;
    static bool bBaseDirInitialized = false;

    if (!bBaseDirInitialized) {
        bBaseDirInitialized = true;

        TCHAR GameExecutableFilename[1024] {'\0'};
        GetModuleFileNameW(GetModuleHandleW(NULL), GameExecutableFilename, ARRAYSIZE(GameExecutableFilename));
        ///Return the path parent and strip the game executable name
        BaseDir = FPaths::GetPath(GameExecutableFilename);
    }
    return *BaseDir;
}

const TCHAR* FPlatformMisc::ProjectDir() {
    static FString ProjectDir;
    static bool bProjectDirInitialized = false;

    if (!bProjectDirInitialized) {
        bProjectDirInitialized = true;
        ///Since we are always running on a packaged game, project will always be 2 levels up from the base directory
        ///BaseDir would be something like RootDir/ProjectName/Binaries/Win64
        ProjectDir = FPaths::GetPath(FPaths::GetPath(BaseDir()));
    }
    return *ProjectDir;
}

const TCHAR* FPlatformMisc::RootDir() {
    static FString RootDir;
    static bool bRootDirInitialized = false;

    if (!bRootDirInitialized) {
        bRootDirInitialized = true;
        ///Since we are always running on a packaged game, root directory will always be directly behind a project dir
        RootDir = FPaths::GetPath(ProjectDir());
    }
    return *RootDir;
}

const TCHAR* FPlatformMisc::EngineDir() {
    static FString EngineDir;
    static bool bEngineDirInitialized = false;

    if (!bEngineDirInitialized) {
        bEngineDirInitialized = true;
        ///Since we are always running on a packaged game, engine directory will always be inside of the root
        EngineDir = FString(RootDir()) / STR("Engine");
    }
    return *EngineDir;
}

const TCHAR* FPlatformMisc::GetDefaultPathSeparator() {
    return STR("\\");
}

void FPlatformMisc::NormalizePath(FString& InPath) {
}

NS_RC_UE_END
