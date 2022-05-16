#ifndef UNREAL_PLATFORMFILEMANAGER_HPP
#define UNREAL_PLATFORMFILEMANAGER_HPP

#include <Unreal/Common.hpp>
#include <Unreal/PrimitiveTypes.hpp>

NS_RC_UE_START

class IPlatformFile;

/**
* Platform File chain manager.
**/
class RC_UE_API FPlatformFileManager {
    /** Currently used platform file. */
    IPlatformFile* TopmostPlatformFile;
public:
    /**
     * Updates the active instance of FPlatformFileManager to the provided one
     * Internal function, should only be called at startup
     */
    static void SetPlatformFileManager(FPlatformFileManager* UnderlyingManager);

    /**
     * Gets the currently used platform file.
     *
     * @return Reference to the currently used platform file.
     */
    IPlatformFile& GetPlatformFile();

    /**
     * Sets the current platform file.
     *
     * @param NewTopmostPlatformFile Platform file to be used.
     */
    void SetPlatformFile(IPlatformFile& NewTopmostPlatformFile);

    /**
     * Finds a platform file in the chain of active platform files.
     *
     * @param Name of the platform file.
     * @return Pointer to the active platform file or nullptr if the platform file was not found.
     */
    IPlatformFile* FindPlatformFile(const TCHAR* Name);

    /**
     * Gets FPlatformFileManager Singleton.
     */
    static FPlatformFileManager& Get();
};

NS_RC_UE_END

#endif //UNREAL_PLATFORMFILEMANAGER_HPP
