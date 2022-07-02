#include "UnrealVFS/Paths.hpp"
#include "Unreal/PlatformMisc.hpp"

NS_RC_UE_START

namespace UE4Paths_Private {
    auto IsSlashOrBackslash    = [](TCHAR C) { return C == TEXT('/') || C == TEXT('\\'); };
    auto IsNotSlashOrBackslash = [](TCHAR C) { return C != TEXT('/') && C != TEXT('\\'); };

    FString ConvertRelativePathToFullInternal(FString&& BasePath, FString&& InPath) {
        FString FullyPathed;
        if ( FPaths::IsRelative(InPath) ) {
            FullyPathed = MoveTemp(BasePath);
            FullyPathed /= MoveTemp(InPath);
        } else {
            FullyPathed = MoveTemp(InPath);
        }

        FPaths::NormalizeFilename(FullyPathed);
        FPaths::CollapseRelativeDirectories(FullyPathed);

        if (FullyPathed.Len() == 0) {
            // Empty path is not absolute, and '/' is the best guess across all the platforms.
            // This substituion is not valid for Windows of course; however CollapseRelativeDirectories() will not produce an empty
            // absolute path on Windows as it takes care not to remove the drive letter.
            FullyPathed = TEXT("/");
        }
        return FullyPathed;
    }
}

FString FPaths::LaunchDir() {
    return FString(FPlatformMisc::LaunchDir());
}

FString FPaths::EngineDir() {
    return FString(FPlatformMisc::EngineDir());
}

FString FPaths::RootDir() {
    return FString(FPlatformMisc::RootDir());
}

FString FPaths::ProjectDir() {
    return FString(FPlatformMisc::ProjectDir());
}

FString FPaths::ProjectContentDir() {
    return FPaths::ProjectDir() + TEXT("Content/");
}

FString FPaths::ProjectConfigDir() {
    return FPaths::ProjectDir() + TEXT("Config/");
}

FString FPaths::GetExtension( const FString& InPath, bool bIncludeDot ) {
    const FString Filename = GetCleanFilename(InPath);
    int32 DotPos = Filename.Find(TEXT("."), ESearchCase::CaseSensitive, ESearchDir::FromEnd);
    if (DotPos != INDEX_NONE) {
        return Filename.Mid(DotPos + (bIncludeDot ? 0 : 1));
    }
    return TEXT("");
}

FString FPaths::GetCleanFilename(const FString& InPath) {
    static_assert(INDEX_NONE == -1, "INDEX_NONE assumed to be -1");

    int32 EndPos   = InPath.FindLastCharByPredicate(UE4Paths_Private::IsNotSlashOrBackslash) + 1;
    int32 StartPos = InPath.FindLastCharByPredicate(UE4Paths_Private::IsSlashOrBackslash) + 1;

    FString Result = (StartPos <= EndPos) ? InPath.Mid(StartPos, EndPos - StartPos) : TEXT("");
    return Result;
}

FString FPaths::GetCleanFilename(FString&& InPath) {
    static_assert(INDEX_NONE == -1, "INDEX_NONE assumed to be -1");

    int32 EndPos   = InPath.FindLastCharByPredicate(UE4Paths_Private::IsNotSlashOrBackslash) + 1;
    int32 StartPos = InPath.FindLastCharByPredicate(UE4Paths_Private::IsSlashOrBackslash) + 1;

    if (StartPos <= EndPos) {
        InPath.RemoveAt(EndPos, InPath.Len() - EndPos, false);
        InPath.RemoveAt(0, StartPos, false);
    } else {
        InPath.Empty();
    }
    return MoveTemp(InPath);
}

template<typename T>
FString GetBaseFilenameImpl(T&& InPath, bool bRemovePath) {
    FString Wk = bRemovePath ? FPaths::GetCleanFilename(Forward<T>(InPath)) : Forward<T>(InPath);

    // remove the extension
    const int32 ExtPos = Wk.Find(TEXT("."), ESearchCase::CaseSensitive, ESearchDir::FromEnd);

    if (ExtPos != INDEX_NONE) {
        // determine the position of the path/leaf separator
        int32 LeafPos = INDEX_NONE;
        if (!bRemovePath) {
            LeafPos = Wk.FindLastCharByPredicate(UE4Paths_Private::IsSlashOrBackslash);
        }

        if (LeafPos == INDEX_NONE || ExtPos > LeafPos) {
            Wk.LeftInline(ExtPos);
        }
    }
    return Wk;
}

FString FPaths::GetBaseFilename(const FString& InPath, bool bRemovePath) {
    return GetBaseFilenameImpl(InPath, bRemovePath);
}

FString FPaths::GetBaseFilename(FString&& InPath, bool bRemovePath) {
    return GetBaseFilenameImpl(MoveTemp(InPath), bRemovePath);
}

FString FPaths::GetPath(const FString& InPath) {
    int32 Pos = InPath.FindLastCharByPredicate(UE4Paths_Private::IsSlashOrBackslash);

    FString Result;
    if (Pos != INDEX_NONE) {
        Result = InPath.Left(Pos);
    }

    return Result;
}

FString FPaths::GetPath(FString&& InPath) {
    int32 Pos = InPath.FindLastCharByPredicate(UE4Paths_Private::IsSlashOrBackslash);

    FString Result;
    if (Pos != INDEX_NONE) {
        InPath.RemoveAt(Pos, InPath.Len() - Pos, false);
        Result = MoveTemp(InPath);
    }

    return Result;
}

FString FPaths::GetPathLeaf(const FString& InPath) {
    static_assert(INDEX_NONE == -1, "INDEX_NONE assumed to be -1");

    int32 EndPos   = InPath.FindLastCharByPredicate(UE4Paths_Private::IsNotSlashOrBackslash) + 1;
    int32 StartPos = InPath.FindLastCharByPredicate(UE4Paths_Private::IsSlashOrBackslash, EndPos) + 1;

    FString Result = InPath.Mid(StartPos, EndPos - StartPos);
    return Result;
}

FString FPaths::GetPathLeaf(FString&& InPath) {
    static_assert(INDEX_NONE == -1, "INDEX_NONE assumed to be -1");

    int32 EndPos   = InPath.FindLastCharByPredicate(UE4Paths_Private::IsNotSlashOrBackslash) + 1;
    int32 StartPos = InPath.FindLastCharByPredicate(UE4Paths_Private::IsSlashOrBackslash, EndPos) + 1;

    InPath.RemoveAt(EndPos, InPath.Len() - EndPos, false);
    InPath.RemoveAt(0, StartPos, false);

    return MoveTemp(InPath);
}

FString FPaths::ChangeExtension(const FString& InPath, const FString& InNewExtension) {
    int32 Pos = INDEX_NONE;
    if (InPath.FindLastChar(TEXT('.'), Pos)) {
        const int32 PathEndPos = InPath.FindLastCharByPredicate(UE4Paths_Private::IsSlashOrBackslash);
        if (PathEndPos != INDEX_NONE && PathEndPos > Pos) {
            // The dot found was part of the path rather than the name
            Pos = INDEX_NONE;
        }
    }

    if (Pos != INDEX_NONE) {
        FString Result = InPath.Left(Pos);

        if (InNewExtension.Len() && InNewExtension[0] != '.') {
            Result += '.';
        }

        Result += InNewExtension;
        return Result;
    }
    return InPath;
}

FString FPaths::SetExtension(const FString& InPath, const FString& InNewExtension) {
    int32 Pos = INDEX_NONE;
    if (InPath.FindLastChar(TEXT('.'), Pos)) {
        const int32 PathEndPos = InPath.FindLastCharByPredicate(UE4Paths_Private::IsSlashOrBackslash);
        if (PathEndPos != INDEX_NONE && PathEndPos > Pos) {
            // The dot found was part of the path rather than the name
            Pos = INDEX_NONE;
        }
    }

    FString Result = Pos == INDEX_NONE ? InPath : InPath.Left(Pos);

    if (InNewExtension.Len() && InNewExtension[0] != '.') {
        Result += '.';
    }

    Result += InNewExtension;
    return Result;
}

bool FPaths::IsDrive(const FString& InPath) {
    FString ConvertedPathString = InPath;

    ConvertedPathString = ConvertedPathString.Replace(TEXT("/"), TEXT("\\"), ESearchCase::CaseSensitive);
    const TCHAR* ConvertedPath= *ConvertedPathString;

    // Does Path refer to a drive letter or BNC path?
    if (ConvertedPath[0] == TCHAR(0)) {
        return true;
    } else if (towupper(ConvertedPath[0]) != towlower(ConvertedPath[0]) && ConvertedPath[1] == TEXT(':') && ConvertedPath[2] == 0) {
        return true;
    } else if (wcscmp(ConvertedPath, TEXT("\\")) == 0) {
        return true;
    } else if (wcscmp(ConvertedPath,TEXT("\\\\")) == 0) {
        return true;
    } else if (ConvertedPath[0]==TEXT('\\') && ConvertedPath[1] == TEXT('\\') && !std::wcschr(ConvertedPath + 2, TEXT('\\'))) {
        return true;
    } else {
        // Need to handle cases such as X:\A\B\..\..\C\..
        // This assumes there is no actual filename in the path (ie, not c:\DIR\File.ext)!
        FString TempPath(ConvertedPath);
        // Make sure there is a '\' at the end of the path
        if (TempPath.Find(TEXT("\\"), ESearchCase::CaseSensitive, ESearchDir::FromEnd) != (TempPath.Len() - 1)) {
            TempPath += TEXT("\\");
        }

        FString CheckPath = TEXT("");
        int32 ColonSlashIndex = TempPath.Find(TEXT(":\\"), ESearchCase::CaseSensitive);
        if (ColonSlashIndex != INDEX_NONE) {
            // Remove the 'X:\' from the start
            CheckPath = TempPath.Right(TempPath.Len() - ColonSlashIndex - 2);
        }
        else {
            // See if the first two characters are '\\' to handle \\Server\Foo\Bar cases
            if (TempPath.StartsWith(TEXT("\\\\"), ESearchCase::CaseSensitive)) {
                CheckPath = TempPath.Right(TempPath.Len() - 2);
                // Find the next slash
                int32 SlashIndex = CheckPath.Find(TEXT("\\"), ESearchCase::CaseSensitive);
                if (SlashIndex != INDEX_NONE) {
                    CheckPath.RightInline(CheckPath.Len() - SlashIndex  - 1, false);
                } else {
                    CheckPath.Empty();
                }
            }
        }

        if (CheckPath.Len() > 0) {
            // Replace any remaining '\\' instances with '\'
            CheckPath.ReplaceInline(TEXT("\\\\"), TEXT("\\"), ESearchCase::CaseSensitive);

            int32 CheckCount = 0;
            int32 SlashIndex = CheckPath.Find(TEXT("\\"), ESearchCase::CaseSensitive);
            while (SlashIndex != INDEX_NONE) {
                FString FolderName = CheckPath.Left(SlashIndex);
                if (FolderName == TEXT("..")) {
                    // It's a relative path, so subtract one from the count
                    CheckCount--;
                } else {
                    // It's a real folder, so add one to the count
                    CheckCount++;
                }
                CheckPath.RightInline(CheckPath.Len() - SlashIndex  - 1, false);
                SlashIndex = CheckPath.Find(TEXT("\\"), ESearchCase::CaseSensitive);
            }
            if (CheckCount <= 0) {
                // If there were the same number or greater relative to real folders, it's the root dir
                return true;
            }
        }
    }

    // It's not a drive...
    return false;
}

bool FPaths::IsRelative(const FString& InPath) {
    // The previous implementation of this function seemed to handle normalized and unnormalized paths, so this one does too for legacy reasons.
    const uint32 PathLen = InPath.Len();
    const bool IsRooted = PathLen &&
                           ((InPath[0] == '/') ||												// Root of the current directory on Windows, root on UNIX-likes.  Also covers "\\", considering normalization replaces "\\" with "//".
                           (PathLen >= 2 && (													// Check it's safe to access InPath[1]!
                                   ((InPath[0] == '\\') && (InPath[1] == '\\'))					// Root of the current directory on Windows. Also covers "\\" for UNC or "network" paths.
                                   || (InPath[1] == ':' && iswalpha(InPath[0]))				// Starts with "<DriveLetter>:"
                           )));
    return !IsRooted;
}

void FPaths::NormalizeFilename(FString& InPath) {
    InPath.ReplaceInline(TEXT("\\"), TEXT("/"), ESearchCase::CaseSensitive);

    FPlatformMisc::NormalizePath(InPath);
}

void FPaths::NormalizeDirectoryName(FString& InPath)
{
    InPath.ReplaceInline(TEXT("\\"), TEXT("/"), ESearchCase::CaseSensitive);
    if (InPath.EndsWith(TEXT("/"), ESearchCase::CaseSensitive) && !InPath.EndsWith(TEXT("//"), ESearchCase::CaseSensitive) && !InPath.EndsWith(TEXT(":/"), ESearchCase::CaseSensitive))
    {
        // overwrite trailing slash with terminator
        InPath.GetCharArray()[InPath.Len() - 1] = TEXT('\0');
        // shrink down
        InPath.TrimToNullTerminator();
    }

    FPlatformMisc::NormalizePath(InPath);
}

bool FPaths::CollapseRelativeDirectories(FString& InPath) {
    const TCHAR ParentDir[] = TEXT("/..");
    const int32 ParentDirLength = wcslen(ParentDir); // To avoid hardcoded values

    for (;;) {
        // An empty path is finished
        if (InPath.IsEmpty()) {
            break;
        }

        // Consider empty paths or paths which start with .. or /.. as invalid
        if (InPath.StartsWith(TEXT(".."), ESearchCase::CaseSensitive) || InPath.StartsWith(ParentDir)) {
            return false;
        }

        // If there are no "/.."s left then we're done
        int32 Index = InPath.Find(ParentDir, ESearchCase::CaseSensitive);
        if (Index == -1) {
            break;
        }

        // Ignore folders beginning with dots
        for (;;) {
            if (InPath.Len() <= Index + ParentDirLength || InPath[Index + ParentDirLength] == TEXT('/')) {
                break;
            }

            Index = InPath.Find(ParentDir, ESearchCase::CaseSensitive, ESearchDir::FromStart, Index + ParentDirLength);
            if (Index == -1) {
                break;
            }
        }

        if (Index == -1) {
            break;
        }

        int32 PreviousSeparatorIndex = Index;
        for (;;) {
            // Find the previous slash
            PreviousSeparatorIndex = std::max(0, InPath.Find( TEXT("/"), ESearchCase::CaseSensitive, ESearchDir::FromEnd, PreviousSeparatorIndex - 1));

            // Stop if we've hit the start of the string
            if (PreviousSeparatorIndex == 0) {
                break;
            }

            // Stop if we've found a directory that isn't "/./"
            if ((Index - PreviousSeparatorIndex) > 1 && (InPath[PreviousSeparatorIndex + 1] != TEXT('.') || InPath[PreviousSeparatorIndex + 2] != TEXT('/'))) {
                break;
            }
        }

        // If we're attempting to remove the drive letter, that's illegal
        int32 Colon = InPath.Find(TEXT(":"), ESearchCase::CaseSensitive, ESearchDir::FromStart, PreviousSeparatorIndex);
        if (Colon >= 0 && Colon < Index) {
            return false;
        }
        InPath.RemoveAt(PreviousSeparatorIndex, Index - PreviousSeparatorIndex + ParentDirLength, false);
    }

    InPath.ReplaceInline(TEXT("./"), TEXT(""), ESearchCase::CaseSensitive);
    InPath.TrimToNullTerminator();
    return true;
}

FString FPaths::RemoveDuplicateSlashes(const FString& InPath) {
    FString Result = InPath;
    RemoveDuplicateSlashes(Result);
    return MoveTemp(Result);
}

void FPaths::RemoveDuplicateSlashes(FString& InPath) {
    TCHAR* Text = InPath.GetCharArray();
    if (!Text) {
        return;
    }
    const TCHAR* const TwoSlashStr = TEXT("//");
    const TCHAR SlashChr = TEXT('/');

    TCHAR* const EditStart = std::wcsstr(Text, TwoSlashStr);
    if (!EditStart) {
        return;
    }
    const TCHAR* const TextEnd = Text + InPath.Len();

    // Since we know we've found TwoSlashes, point the initial Write head at the spot where the second slash is (which we shall skip), and point the Read head at the next character after the second slash
    TCHAR* Write = EditStart + 1;	// The position to write the next character of the output
    const TCHAR* Read = Write + 1;	// The next character of the input to read

    for (; Read < TextEnd; ++Read) {
        TCHAR NextChar = *Read;
        if (Write[-1] != SlashChr || NextChar != SlashChr) {
            *Write++ = NextChar;
        }
        else {
            // Skip the NextChar when adding on a slash after an existing slash, e.g // or more generally before/////after
            //                                                                       WR                         W  R
        }
    }
    *Write = TEXT('\0');
    InPath.TrimToNullTerminator();
}

FString FPaths::CreateStandardFilename(const FString& InPath) {
    // if this is an empty path, use the relative base dir
    if (InPath.Len() == 0) {
        FString BaseDir = FPlatformMisc::BaseDir();
        // if the base directory is nothing then this function will recurse infinitely instead of returning nothing.
        if (BaseDir.Len() == 0)
            return BaseDir;
        return FPaths::CreateStandardFilename(BaseDir);
    }

    FString WithSlashes = InPath;
    WithSlashes.ReplaceInline(TEXT("\\"), TEXT("/"), ESearchCase::CaseSensitive);

    const FString RootDirectory = FPaths::RootDir();

    // look for paths that cannot be made relative, and are therefore left alone
    // UNC (windows) network path
    bool bCannotBeStandardized = InPath.StartsWith(TEXT("\\\\"), ESearchCase::CaseSensitive);
    // windows drive letter path that doesn't start with base dir
    bCannotBeStandardized |= ((InPath.Len() > 1) && (InPath[1] == ':') && !WithSlashes.StartsWith(RootDirectory));
    // Unix style absolute path that doesn't start with base dir
    bCannotBeStandardized |= (WithSlashes.GetCharArray()[0] == '/' && !WithSlashes.StartsWith(RootDirectory));

    // if it can't be standardized, just return itself
    if (bCannotBeStandardized)
    {
        return InPath;
    }

    // make an absolute path
    FString Standardized = FPaths::ConvertRelativePathToFull(WithSlashes);

    // remove duplicate slashes
    FPaths::RemoveDuplicateSlashes(Standardized);
    // make it relative to Engine\Binaries\Platform
    Standardized.ReplaceInline(*RootDirectory, *FPaths::GetRelativePathToRoot());
    return Standardized;
}

void FPaths::MakeStandardFilename(FString& InPath) {
    InPath = FPaths::CreateStandardFilename(InPath);
}

void FPaths::MakePlatformFilename( FString& InPath ) {
    InPath.ReplaceInline(TEXT( "\\" ), FPlatformMisc::GetDefaultPathSeparator(), ESearchCase::CaseSensitive);
    InPath.ReplaceInline(TEXT( "/" ), FPlatformMisc::GetDefaultPathSeparator(), ESearchCase::CaseSensitive);
}

bool FPaths::MakePathRelativeTo( FString& InPath, const TCHAR* InRelativeTo ) {
    FString Target = FPaths::ConvertRelativePathToFull(InPath);
    FString Source = FPaths::GetPath(FPaths::ConvertRelativePathToFull(InRelativeTo));

    Source.ReplaceInline(TEXT("\\"), TEXT("/"), ESearchCase::CaseSensitive);
    Target.ReplaceInline(TEXT("\\"), TEXT("/"), ESearchCase::CaseSensitive);

    TArray<FString> TargetArray;
    Target.ParseIntoArray(TargetArray, TEXT("/"), true);
    TArray<FString> SourceArray;
    Source.ParseIntoArray(SourceArray, TEXT("/"), true);

    if (TargetArray.Num() && SourceArray.Num()) {
        // Check for being on different drives
        if ((TargetArray[0].Len() > 1) && (TargetArray[0][1] == TEXT(':')) && (SourceArray[0].Len() > 1) && (SourceArray[0][1] == TEXT(':'))) {
            if (towupper(TargetArray[0][0]) != towupper(SourceArray[0][0])) {
                // The Target and Source are on different drives... No relative path available.
                return false;
            }
        }
    }

    while (TargetArray.Num() && SourceArray.Num() && TargetArray[0] == SourceArray[0]) {
        TargetArray.RemoveAt(0);
        SourceArray.RemoveAt(0);
    }
    FString Result;
    for (int32 Index = 0; Index < SourceArray.Num(); Index++) {
        Result += TEXT("../");
    }
    for (int32 Index = 0; Index < TargetArray.Num(); Index++) {
        Result += TargetArray[Index];
        if (Index + 1 < TargetArray.Num()) {
            Result += TEXT("/");
        }
    }

    InPath = MoveTemp(Result);
    return true;
}

FString FPaths::ConvertRelativePathToFull(const FString& InPath) {
    return UE4Paths_Private::ConvertRelativePathToFullInternal(FString(FPlatformMisc::BaseDir()), FString(InPath));
}

FString FPaths::ConvertRelativePathToFull(FString&& InPath) {
    return UE4Paths_Private::ConvertRelativePathToFullInternal(FString(FPlatformMisc::BaseDir()), MoveTemp(InPath));
}

FString FPaths::ConvertRelativePathToFull(const FString& BasePath, const FString& InPath) {
    return UE4Paths_Private::ConvertRelativePathToFullInternal(CopyTemp(BasePath), CopyTemp(InPath));
}

FString FPaths::ConvertRelativePathToFull(const FString& BasePath, FString&& InPath) {
    return UE4Paths_Private::ConvertRelativePathToFullInternal(CopyTemp(BasePath), MoveTemp(InPath));
}

FString FPaths::ConvertRelativePathToFull(FString&& BasePath, const FString& InPath) {
    return UE4Paths_Private::ConvertRelativePathToFullInternal(MoveTemp(BasePath), CopyTemp(InPath));
}

FString FPaths::ConvertRelativePathToFull(FString&& BasePath, FString&& InPath) {
    return UE4Paths_Private::ConvertRelativePathToFullInternal(MoveTemp(BasePath), MoveTemp(InPath));
}

FString FPaths::GetInvalidFileSystemChars() {
    // Windows has the most restricted file system, and since we're cross platform, we have to respect the limitations of the lowest common denominator
    // # isn't legal. Used for revision specifiers in P4/SVN, and also not allowed on Windows anyway
    // @ isn't legal. Used for revision/label specifiers in P4/SVN
    // ^ isn't legal. While the file-system won't complain about this character, Visual Studio will
    static const TCHAR RestrictedChars[] = TEXT("/?:&\\*\"<>|%#@^");
    return RestrictedChars;
}

FString FPaths::MakeValidFileName(const FString& InString, TCHAR InReplacementChar /*= 0*/) {
    const FString RestrictedChars = GetInvalidFileSystemChars();

    const int InLen = InString.Len();

    TArray<TCHAR> Output;
    Output.AddUninitialized(InLen + 1);

    // first remove all invalid chars
    for (int i = 0; i < InLen; i++) {
        int32 Unused = 0;
        if (RestrictedChars.FindChar(InString[i], Unused)) {
            Output[i] = InReplacementChar;
        } else {
            Output[i] = InString[i];
        }
    }

    Output[InLen] = TEXT('\0');

    if (InReplacementChar == 0) {
        int CurrentChar = 0;

        // compact the string by replacing any null entries with the next non-null entry
        int iFill = 0;
        for (int iChar = 0; iChar < InLen; iChar++) {
            if (Output[iChar] == 0) {
                // adjust our fill index if we passed it
                if (iFill < iChar) {
                    iFill = iChar;
                }

                // scan forward
                while (++iFill < InLen) {
                    if (Output[iFill] != 0) {
                        break;
                    }
                }

                if (iFill < InLen) {
                    // take this char and null it out
                    Output[iChar] = Output[iFill];
                    Output[iFill] = TEXT('\0');
                }
            }
        }
    }
    return FString(Output.GetData());
}

void FPaths::Split( const FString& InPath, FString& PathPart, FString& FilenamePart, FString& ExtensionPart ) {
    PathPart = GetPath(InPath);
    FilenamePart = GetBaseFilename(InPath);
    ExtensionPart = GetExtension(InPath);
}

const FString& FPaths::GetRelativePathToRoot() {
    static bool bRelativePathToRootInitialized = false;
    static FString RelativePathToRoot;

    if (!bRelativePathToRootInitialized) {
        FString RootDirectory = FPaths::RootDir();
        FString BaseDirectory = FPlatformMisc::BaseDir();

        // this is how to go from the base dir back to the root
        RelativePathToRoot = RootDirectory;
        FPaths::MakePathRelativeTo(RelativePathToRoot, *BaseDirectory);

        // Ensure that the path ends w/ '/'
        if ((RelativePathToRoot.Len() > 0) && !RelativePathToRoot.EndsWith(TEXT("/"), ESearchCase::CaseSensitive) &&
            !RelativePathToRoot.EndsWith(TEXT("\\"), ESearchCase::CaseSensitive)) {
            RelativePathToRoot += TEXT("/");
        }

        bRelativePathToRootInitialized = true;
    }
    return RelativePathToRoot;
}

void FPaths::CombineInternal(FString& OutPath, const TCHAR** Pathes, int32 NumPathes) {
    check(Pathes != nullptr && NumPathes > 0);

    int32 OutStringSize = 0;

    for (int32 i=0; i < NumPathes; ++i) {
        OutStringSize += wcslen(Pathes[i]) + 1;
    }

    OutPath.Empty(OutStringSize);
    OutPath += Pathes[0];

    for (int32 i=1; i < NumPathes; ++i) {
        OutPath /= Pathes[i];
    }
}

bool FPaths::IsSamePath(const FString& PathA, const FString& PathB) {
    FString TmpA = FPaths::ConvertRelativePathToFull(PathA);
    FString TmpB = FPaths::ConvertRelativePathToFull(PathB);

    FPaths::RemoveDuplicateSlashes(TmpA);
    FPaths::RemoveDuplicateSlashes(TmpB);

    return _wcsicmp(*TmpA, *TmpB) == 0;
}

bool FPaths::IsUnderDirectory(const FString& InPath, const FString& InDirectory) {
    FString Path = FPaths::ConvertRelativePathToFull(InPath);

    FString Directory = FPaths::ConvertRelativePathToFull(InDirectory);
    if (Directory.EndsWith(TEXT("/"))) {
        Directory.RemoveAt(Directory.Len() - 1);
    }

    int Compare = _wcsnicmp(*Path, *Directory, Directory.Len());

    return Compare == 0 && (Path.Len() == Directory.Len() || Path[Directory.Len()] == TEXT('/'));
}

NS_RC_UE_END