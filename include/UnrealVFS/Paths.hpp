#ifndef UNREAL_PATHS_HPP
#define UNREAL_PATHS_HPP
#include "Unreal/FString.hpp"

NS_RC_UE_START

/**
 * Path helpers for retrieving game dir, engine dir, etc.
 */
class RC_UE_API FPaths {
public:
    /**
     * Returns the base directory of the "core" engine that can be shared across
     * several games or across games & mods. Shaders and base localization files
     * e.g. reside in the engine directory.
     *
     * @return engine directory
     */
    static FString EngineDir();

    /**
     * Returns the root directory of the engine directory tree
     *
     * @return Root directory.
     */
    static FString RootDir();

    /**
     * Returns the base directory of the current project by looking at FApp::GetProjectName().
     * This is usually a subdirectory of the installation
     * root directory and can be overridden on the command line to allow self
     * contained mod support.
     *
     * @return base directory
     */
    static FString ProjectDir();

    /**
     * Returns the content directory of the current game by looking at FApp::GetProjectName().
     *
     * @return content directory
     */
    static FString ProjectContentDir();

    /**
    * Returns the directory the root configuration files are located.
    *
    * @return root config directory
    */
    static FString ProjectConfigDir();

    /**
     * Gets the extension for this filename.
     *
     * @param	bIncludeDot		if true, includes the leading dot in the result
     *
     * @return	the extension of this filename, or an empty string if the filename doesn't have an extension.
     */
    static FString GetExtension( const FString& InPath, bool bIncludeDot=false );

    // Returns the filename (with extension), minus any path information.
    static FString GetCleanFilename(const FString& InPath);

    // Returns the filename (with extension), minus any path information.
    static FString GetCleanFilename(FString&& InPath);

    // Returns the same thing as GetCleanFilename, but without the extension
    static FString GetBaseFilename(const FString& InPath, bool bRemovePath=true );

    // Returns the same thing as GetCleanFilename, but without the extension
    static FString GetBaseFilename(FString&& InPath, bool bRemovePath = true);

    // Returns the path in front of the filename
    static FString GetPath(const FString& InPath);

    // Returns the path in front of the filename
    static FString GetPath(FString&& InPath);

    // Returns the leaf in the path
    static FString GetPathLeaf(const FString& InPath);

    // Returns the leaf in the path
    static FString GetPathLeaf(FString&& InPath);

    /** Changes the extension of the given filename (does nothing if the file has no extension) */
    static FString ChangeExtension(const FString& InPath, const FString& InNewExtension);

    /** Sets the extension of the given filename (like ChangeExtension, but also applies the extension if the file doesn't have one) */
    static FString SetExtension(const FString& InPath, const FString& InNewExtension);

    /** Returns true if this path represents a root drive or volume */
    static bool IsDrive(const FString& InPath);

    /** Returns true if this path is relative to another path */
    static bool IsRelative(const FString& InPath);

    /** Convert all / and \ to TEXT("/") */
    static void NormalizeFilename(FString& InPath);

    /**
     * Checks if two paths are the same.
     *
     * @param PathA First path to check.
     * @param PathB Second path to check.
     *
     * @returns True if both paths are the same. False otherwise.
     */
    static bool IsSamePath(const FString& PathA, const FString& PathB);

    /** Determines if a path is under a given directory */
    static bool IsUnderDirectory(const FString& InPath, const FString& InDirectory);

    /** Normalize all / and \ to TEXT("/") and remove any trailing TEXT("/") if the character before that is not a TEXT("/") or a colon */
    static void NormalizeDirectoryName(FString& InPath);

    /**
     * Takes a fully pathed string and eliminates relative pathing (eg: annihilates ".." with the adjacent directory).
     * Assumes all slashes have been converted to TEXT('/').
     * For example, takes the string:
     *	BaseDirectory/SomeDirectory/../SomeOtherDirectory/Filename.ext
     * and converts it to:
     *	BaseDirectory/SomeOtherDirectory/Filename.ext
     */
    static bool CollapseRelativeDirectories(FString& InPath);

    /**
     * Removes duplicate slashes in paths.
     * Assumes all slashes have been converted to TEXT('/').
     * For example, takes the string:
     *	BaseDirectory/SomeDirectory//SomeOtherDirectory////Filename.ext
     * and converts it to:
     *	BaseDirectory/SomeDirectory/SomeOtherDirectory/Filename.ext
     */
    static void RemoveDuplicateSlashes(FString& InPath);

    FString RemoveDuplicateSlashes(const FString& InPath);

    /**
     * Make fully standard "Unreal" pathname:
     *    - Normalizes path separators [NormalizeFilename]
     *    - Removes extraneous separators  [NormalizeDirectoryName, as well removing adjacent separators]
     *    - Collapses internal ..'s
     *    - Makes relative to Engine\Binaries\<Platform> (will ALWAYS start with ..\..\..)
     */
    static FString CreateStandardFilename(const FString& InPath);

    static void MakeStandardFilename(FString& InPath);

    /** Takes an "Unreal" pathname and converts it to a platform filename. */
    static void MakePlatformFilename(FString& InPath);

    /**
     * Assuming both paths (or filenames) are relative to the same base dir, modifies InPath to be relative to InRelativeTo
     *
     * @param InPath Path to change to be relative to InRelativeTo
     * @param InRelativeTo Path to use as the new relative base
     * @returns true if InPath was changed to be relative
     */
    static bool MakePathRelativeTo( FString& InPath, const TCHAR* InRelativeTo );

    /**
     * Converts a relative path name to a fully qualified name relative to the process BaseDir().
     */
    static FString ConvertRelativePathToFull(const FString& InPath);

    /**
     * Converts a relative path name to a fully qualified name relative to the process BaseDir().
     */
    static FString ConvertRelativePathToFull(FString&& InPath);

    /**
     * Converts a relative path name to a fully qualified name relative to the specified BasePath.
     */
    static FString ConvertRelativePathToFull(const FString& BasePath, const FString& InPath);

    /**
     * Converts a relative path name to a fully qualified name relative to the specified BasePath.
     */
    static FString ConvertRelativePathToFull(const FString& BasePath, FString&& InPath);

    /**
     * Converts a relative path name to a fully qualified name relative to the specified BasePath.
     */
    static FString ConvertRelativePathToFull(FString&& BasePath, const FString& InPath);

    /**
     * Converts a relative path name to a fully qualified name relative to the specified BasePath.
     */
    static FString ConvertRelativePathToFull(FString&& BasePath, FString&& InPath);

    /**
    * Returns a string containing all invalid characters as dictated by the operating system
    */
    static FString GetInvalidFileSystemChars();

    /**
    *	Returns a string that is safe to use as a filename because all items in
    *	GetInvalidFileSystemChars() are removed
    */
    static FString MakeValidFileName(const FString& InString, TCHAR InReplacementChar=0);

    /**
     * Parses a fully qualified or relative filename into its components (filename, path, extension).
     *
     * @param	Path		[out] receives the value of the path portion of the input string
     * @param	Filename	[out] receives the value of the filename portion of the input string
     * @param	Extension	[out] receives the value of the extension portion of the input string
     */
    static void Split( const FString& InPath, FString& PathPart, FString& FilenamePart, FString& ExtensionPart );

    /** Gets the relative path to get from BaseDir to RootDirectory  */
    static const FString& GetRelativePathToRoot();

    template <typename... PathTypes>
    FORCEINLINE static FString Combine(PathTypes&&... InPaths) {
        const TCHAR* Paths[] = { GetTCharPtr(Forward<PathTypes>(InPaths))... };
        FString Out;

        CombineInternal(Out, Paths, UE_ARRAY_COUNT(Paths));
        return Out;
    }
protected:
    static void CombineInternal(FString& OutPath, const TCHAR** Paths, int32 NumPaths);
private:
    struct FStaticData;

    FORCEINLINE static const TCHAR* GetTCharPtr(const TCHAR* Ptr) {
        return Ptr;
    }

    FORCEINLINE static const TCHAR* GetTCharPtr(const FString& Str) {
        return *Str;
    }
};

NS_RC_UE_END

#endif