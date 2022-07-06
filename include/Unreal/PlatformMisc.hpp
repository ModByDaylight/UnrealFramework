#ifndef UNREAL_PLATFORMMISC_HPP
#define UNREAL_PLATFORMMISC_HPP

#include "Unreal/FString.hpp"

NS_RC_UE_START

class RC_UE_API FPlatformMisc {
public:
    /**
     * Returns the base directory of the "core" engine that can be shared across
     * several games or across games & mods. Shaders and base localization files
     * e.g. reside in the engine directory.
     *
     * @return engine directory
     */
    static const TCHAR* EngineDir();

    /**
     * Returns the root directory of the engine directory tree
     *
     * @return Root directory.
     */
    static const TCHAR* RootDir();

    /**
     * Returns the base directory of the current project by looking at FApp::GetProjectName().
     * This is usually a subdirectory of the installation
     * root directory and can be overridden on the command line to allow self
     * contained mod support.
     *
     * @return base directory
     */
    static const TCHAR* ProjectDir();

    /**
      * Returns the base directory of the process, e.g. the directory in which executable file is located
      *
      * @return Root directory.
      */
    static const TCHAR* BaseDir();

    /**
     * Returns the default path separator for the platform we are currently running on
     *
     * @return the default path separator for the platform
     */
    static const TCHAR* GetDefaultPathSeparator();

    /**
     * Normalizes the path for the currently running platform
     * @param InPath path to be normalized
     */
    static void NormalizePath(FString& InPath);
};

NS_RC_UE_END

#endif //UNREAL_PLATFORMMISC_HPP
