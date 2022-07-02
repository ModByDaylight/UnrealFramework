#ifndef RC_UNREAL_FSTRING_HPP
#define RC_UNREAL_FSTRING_HPP

#include <Function/Function.hpp>
#include <Unreal/Common.hpp>
#include <Unreal/TArray.hpp>

#define MAX_SPRINTF 1024

/** Case-insensitive version of the std::wcstr */
FORCEINLINE wchar_t* wcstri(const wchar_t* haystack, const wchar_t* needle) {
    do {
        const wchar_t* h = haystack;
        const wchar_t* n = needle;
        while (towlower(*h) == towlower(*n) && *n) {
            h++;
            n++;
        }
        if (*n == 0) {
            return (wchar_t*) haystack;
        }
    } while (*haystack++);
    return nullptr;
}

namespace RC::Unreal
{
    namespace ESearchCase {
        enum Type {
            IgnoreCase,
            CaseSensitive
        };
    }

    namespace ESearchDir {
        enum Type {
            FromStart,
            FromEnd
        };
    }

    class RC_UE_API FString {
    private:
        TArray<TCHAR> Data;
    public:
        FString() = default;
        FString(const FString&) = default;
        FString& operator=(const FString&) = default;

        FORCEINLINE FString(const FString& Other, int32 ExtraSlack)
                : Data(Other.Data, ExtraSlack + ((Other.Data.Num() || !ExtraSlack) ? 0 : 1)){
        }

        FORCEINLINE FString(const TCHAR* Src, int32 ExtraSlack = 0) {
            if (Src && *Src) {
                int32 SrcLen = (int32) wcslen(Src) + 1;
                Data.Reserve(SrcLen + ExtraSlack);
                Data.AddUninitialized(SrcLen);
                FMemory::Memcpy(Data.GetData(), Src, SrcLen * sizeof(TCHAR));
            } else {
                Reserve(ExtraSlack);
            }
        }

        FORCEINLINE explicit FString(int32 InCount, const TCHAR* InSrc) {
            if (InSrc && InCount > 0 && *InSrc) {
                Data.Reserve(InCount + 1);
                Data.AddUninitialized(InCount + 1);

                FMemory::Memcpy(Data.GetData(), InSrc, InCount * sizeof(TCHAR));
                *(Data.GetData() + (Data.Num() - 1)) = TEXT('\0');
            }
        }

        /** To allow more efficient memory handling, automatically adds one for the string termination. */
        FORCEINLINE void Reserve(int32 CharacterCount) {
            if (CharacterCount > 0) {
                Data.Reserve(CharacterCount + 1);
            }
        }

        /** Empties the string of all contents */
        FORCEINLINE void Empty(int32 Slack = 0) {
            Data.Empty(Slack);
        }

        /** Append a string and return a reference to this */
        FORCEINLINE FString& Append(const TCHAR* Str, int32 Count) {
            AppendChars(Str, Count);
            return *this;
        }

        /**
         * Appends a character range without null-terminators in it
         * @param Str can be null if Count is 0. Can be unterminated, Str[Count] isn't read.
         */
        FORCEINLINE void AppendChars(const TCHAR* Str, int32 Count) {
            if (!Count) {
                return;
            }
            const int32 OldNum = Data.Num();
            // Reserve enough space - including an extra gap for a null terminator if we don't already have a string allocated
            Data.AddUninitialized(Count + (OldNum ? 0 : 1));

            TCHAR* Dest = Data.GetData() + OldNum - (OldNum ? 1 : 0);

            // Copy characters to end of string, overwriting null terminator if we already have one
            FMemory::Memcpy(Dest, Str, Count * sizeof(TCHAR));

            // (Re-)establish the null terminator
            Dest[Count] = '\0';
        }

        /** Append a valid null-terminated string and return a reference to this */
        FORCEINLINE FString& Append(const TCHAR* Str) {
            AppendChars(Str, (int32_t) wcslen(Str));
            return *this;
        }

        /** Append a string and return a reference to this */
        FORCEINLINE FString& Append(const FString& Str) {
            AppendChars(Str.GetCharArray(), Str.Len());
            return *this;
        }

        /** Append a single character and return a reference to this */
        FORCEINLINE FString& AppendChar(TCHAR InChar) {
            if (InChar != 0) {
                int32 InsertIndex = (Data.Num() > 0) ? Data.Num()-1 : 0;
                int32 InsertCount = (Data.Num() > 0) ? 1 : 2;

                Data.AddUninitialized(InsertCount);
                Data[InsertIndex] = InChar;
                Data[InsertIndex+1] = 0;
            }
            return *this;
        }

        /**
         * Removes characters within the string.
         *
         * @param Index           The index of the first character to remove.
         * @param Count           The number of characters to remove.
         * @param bAllowShrinking Whether or not to reallocate to shrink the storage after removal.
         */
        FORCEINLINE void RemoveAt(int32 Index, int32 Count = 1, bool bAllowShrinking = true) {
            Data.RemoveAt(Index, std::clamp(Count, 0, Len()-Index), bAllowShrinking);
        }

        /**
         * Test whether this string starts with given string.
         *
         * @param SearchCase		Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
         * @return true if this string begins with specified text, false otherwise
         */
        FORCEINLINE bool StartsWith(const TCHAR* InPrefix, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase) const {
            if (SearchCase == ESearchCase::IgnoreCase) {
                return InPrefix && *InPrefix && !_wcsnicmp(**this, InPrefix, wcslen(InPrefix));
            } else {
                return InPrefix && *InPrefix && !wcsncmp(**this, InPrefix, wcslen(InPrefix));
            }
        }

        /**
         * Test whether this string starts with given string.
         *
         * @param SearchCase		Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
         * @return true if this string begins with specified text, false otherwise
         */
        FORCEINLINE bool StartsWith(const FString& InPrefix, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase) const {
            if(SearchCase == ESearchCase::IgnoreCase) {
                return InPrefix.Len() > 0 && !_wcsnicmp(**this, *InPrefix, InPrefix.Len());
            } else {
                return InPrefix.Len() > 0 && !wcsncmp(**this, *InPrefix, InPrefix.Len());
            }
        }

        /**
         * Breaks up a delimited string into elements of a string array.
         *
         * @param	InArray		The array to fill with the string pieces
         * @param	pchDelim	The string to delimit on
         * @param	InCullEmpty	If 1, empty strings are not added to the array
         *
         * @return	The number of elements in InArray
         */
        int32 ParseIntoArray(TArray<FString>& OutArray, const TCHAR* pchDelim, const bool InCullEmpty) const {
            // Make sure to delimit string is not null or empty
            check(pchDelim);
            OutArray.Empty();
            const TCHAR *Start = **this;
            const int32 DelimLength = wcslen(pchDelim);

            if (Start && *Start != TEXT('\0') && DelimLength) {
                while(const TCHAR* At = std::wcsstr(Start, pchDelim)) {
                    if (!InCullEmpty || At-Start) {
                        OutArray.Emplace((int32) (At-Start),Start);
                    }
                    Start = At + DelimLength;
                }
                if (!InCullEmpty || *Start) {
                    OutArray.Emplace(Start);
                }
            }
            return OutArray.Num();
        }

        /**
        * Converts all characters in this string to uppercase
        * @return a new string with the characters of this converted to uppercase
        */
        FORCEINLINE FString ToUpper() const {
            FString NewString = *this;
            NewString.ToUpperInline();
            return NewString;
        }

        /** Converts all characters in this string to uppercase */
        void ToUpperInline() {
            const int32 StringLength = Len();
            TCHAR* RawData = Data.GetData();

            for (int32 i = 0; i < StringLength; ++i) {
                RawData[i] = towupper(RawData[i]);
            }
        }

        /**
         * Converts all characters in this string to lowercase
         * @return a new string with the characters of this converted to lowercase
         */
        FORCEINLINE FString ToLower() const {
            FString NewString = *this;
            NewString.ToLowerInline();
            return NewString;
        }

        /** Converts all characters in this string to lowercase */
        FORCEINLINE void ToLowerInline() {
            const int32 StringLength = Len();
            TCHAR* RawData = Data.GetData();

            for (int32 i = 0; i < StringLength; ++i) {
                RawData[i] = towlower(RawData[i]);
            }
        }

        /**
         * Searches the string for a substring, and returns index into this string
         * of the first found instance. Can search from beginning or end, and ignore case or not.
         *
         * @param SubStr			The string array of TCHAR to search for
         * @param StartPosition		The start character position to search from
         * @param SearchCase		Indicates whether the search is case sensitive or not
         * @param SearchDir			Indicates whether the search starts at the beginning or at the end.
         */
        FORCEINLINE int32 Find(const TCHAR* SubStr, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase, ESearchDir::Type SearchDir = ESearchDir::FromStart, int32 StartPosition=INDEX_NONE ) const {
            if (SubStr == nullptr) {
                return INDEX_NONE;
            }
            if( SearchDir == ESearchDir::FromStart) {
                const TCHAR* Start = **this;
                if (StartPosition != INDEX_NONE && Len() > 0) {
                    Start += std::clamp(StartPosition, 0, Len() - 1);
                }
                const TCHAR* Tmp = SearchCase == ESearchCase::IgnoreCase
                                   ? wcstri(Start, SubStr)
                                   : std::wcsstr(Start, SubStr);

                return Tmp ? (int32) (Tmp-**this) : INDEX_NONE;
            } else {
                // if ignoring, do a onetime ToUpper on both strings, to avoid ToUppering multiple
                // times in the loop below
                if (SearchCase == ESearchCase::IgnoreCase) {
                    return ToUpper().Find(*FString(SubStr).ToUpper(), ESearchCase::CaseSensitive, SearchDir, StartPosition);
                } else {
                    const int32 SearchStringLength = std::max(1, (int32) wcslen(SubStr));

                    if ( StartPosition == INDEX_NONE || StartPosition >= Len() ) {
                        StartPosition = Len();
                    }

                    for( int32 i = StartPosition - SearchStringLength; i >= 0; i-- ) {
                        int32 j;
                        for( j=0; SubStr[j]; j++ ) {
                            if( (*this)[i+j]!=SubStr[j] ) {
                                break;
                            }
                        }

                        if( !SubStr[j] ) {
                            return i;
                        }
                    }
                    return INDEX_NONE;
                }
            }
        }

        /**
         * Returns whether this string contains the specified substring.
         *
         * @param SubStr			Find to search for
         * @param SearchCase		Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
         * @param SearchDir			Indicates whether the search starts at the beginning or at the end ( defaults to ESearchDir::FromStart )
         * @return					Returns whether the string contains the substring
         **/
        FORCEINLINE bool Contains(const TCHAR* SubStr, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase, ESearchDir::Type SearchDir = ESearchDir::FromStart ) const {
            return Find(SubStr, SearchCase, SearchDir) != INDEX_NONE;
        }

        /**
         * Returns whether this string contains the specified substring.
         *
         * @param SubStr			Find to search for
         * @param SearchCase		Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
         * @param SearchDir			Indicates whether the search starts at the beginning or at the end ( defaults to ESearchDir::FromStart )
         * @return					Returns whether the string contains the substring
         **/
        FORCEINLINE bool Contains(const FString& SubStr, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase, ESearchDir::Type SearchDir = ESearchDir::FromStart) const {
            return Find(*SubStr, SearchCase, SearchDir) != INDEX_NONE;
        }

        /**
         * Replace all occurrences of SearchText with ReplacementText in this string.
         *
         * @param	SearchText	the text that should be removed from this string
         * @param	ReplacementText		the text to insert in its place
         * @param SearchCase	Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
         *
         * @return	the number of occurrences of SearchText that were replaced.
         */
        FORCEINLINE int32 ReplaceInline(const TCHAR* SearchText, const TCHAR* ReplacementText, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase) {
            int32 ReplacementCount = 0;

            if (Len() > 0
                && SearchText != nullptr && *SearchText != 0
                && ReplacementText != nullptr && (SearchCase == ESearchCase::IgnoreCase || wcscmp(SearchText, ReplacementText) != 0)) {
                const int32 NumCharsToReplace = wcslen(SearchText);
                const int32 NumCharsToInsert = wcslen(ReplacementText);

                if (NumCharsToInsert == NumCharsToReplace) {
                    TCHAR* Pos = SearchCase == ESearchCase::IgnoreCase ? wcstri(&(*this)[0], SearchText) : std::wcsstr(&(*this)[0], SearchText);
                    while (Pos != nullptr)
                    {
                        ReplacementCount++;

                        // FCString::Strcpy now inserts a terminating zero so can't use that
                        for (int32 i = 0; i < NumCharsToInsert; i++)
                        {
                            Pos[i] = ReplacementText[i];
                        }

                        if (Pos + NumCharsToReplace - **this < Len())
                        {
                            Pos = SearchCase == ESearchCase::IgnoreCase ? wcstri(Pos + NumCharsToReplace, SearchText) : std::wcsstr(Pos + NumCharsToReplace, SearchText);
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                else if (Contains(SearchText, SearchCase))
                {
                    FString Copy(MoveTemp(*this));

                    // get a pointer into the character data
                    TCHAR* WritePosition = (TCHAR*)Copy.Data.GetData();
                    // look for From in the remaining string
                    TCHAR* SearchPosition = SearchCase == ESearchCase::IgnoreCase ? wcstri(WritePosition, SearchText) : std::wcsstr(WritePosition, SearchText);
                    while (SearchPosition != nullptr)
                    {
                        ReplacementCount++;

                        // replace the first letter of the From with 0 so we can do a strcpy (FString +=)
                        *SearchPosition = 0;

                        // copy everything up to the SearchPosition
                        (*this) += WritePosition;

                        // copy over the ReplacementText
                        (*this) += ReplacementText;

                        // restore the letter, just so we don't have 0's in the string
                        *SearchPosition = *SearchText;

                        WritePosition = SearchPosition + NumCharsToReplace;
                        SearchPosition = SearchCase == ESearchCase::IgnoreCase ? wcstri(WritePosition, SearchText) : std::wcsstr(WritePosition, SearchText);
                    }

                    // copy anything left over
                    (*this) += WritePosition;
                }
            }

            return ReplacementCount;
        }

        /**
         * Replace all occurrences of a substring in this string
         *
         * @param From substring to replace
         * @param To substring to replace From with
         * @param SearchCase	Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
         * @return a copy of this string with the replacement made
         */
        FORCEINLINE FString Replace(const TCHAR* From, const TCHAR* To, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase) const {
            FString NewString = *this;
            NewString.ReplaceInline(From, To, SearchCase);
            return NewString;
        }

        /**
         * Searches the string for a character
         *
         * @param InChar the character to search for
         * @param Index out the position the character was found at, INDEX_NONE if return is false
         * @return true if character was found in this string, otherwise false
         */
        FORCEINLINE bool FindChar( TCHAR InChar, int32& Index ) const {
            return Data.Find(InChar, Index);
        }

        /**
         * Searches the string for the last occurrence of a character
         *
         * @param InChar the character to search for
         * @param Index out the position the character was found at, INDEX_NONE if return is false
         * @return true if character was found in this string, otherwise false
         */
        FORCEINLINE bool FindLastChar( TCHAR InChar, int32& Index ) const {
            return Data.FindLast(InChar, Index);
        }

        /**
         * Searches an initial substring for the last occurrence of a character which matches the specified predicate.
         *
         * @param Pred Predicate that takes TCHAR and returns true if TCHAR matches search criteria, false otherwise.
         * @param Count The number of characters from the front of the string through which to search.
         *
         * @return Index of found TCHAR, INDEX_NONE otherwise.
         */
        template <typename Predicate>
        FORCEINLINE int32 FindLastCharByPredicate(Predicate Pred, int32 Count) const {
            check(Count >= 0 && Count <= this->Len());
            return Data.FindLastByPredicate(Pred, Count);
        }

        /**
         * Searches the string for the last occurrence of a character which matches the specified predicate.
         *
         * @param Pred Predicate that takes TCHAR and returns true if TCHAR matches search criteria, false otherwise.
         * @param StartIndex Index of element from which to start searching. Defaults to last TCHAR in string.
         *
         * @return Index of found TCHAR, INDEX_NONE otherwise.
         */
        template <typename Predicate>
        FORCEINLINE int32 FindLastCharByPredicate(Predicate Pred) const {
            return Data.FindLastByPredicate(Pred, this->Len());
        }

        /**
         * Test whether this string ends with given string.
         *
         * @param SearchCase		Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
         * @return true if this string ends with specified text, false otherwise
         */
        FORCEINLINE bool EndsWith(const TCHAR* InSuffix, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase) const {
            if (!InSuffix || *InSuffix == TEXT('\0')) {
                return false;
            }

            int32 ThisLen   = this->Len();
            int32 SuffixLen = wcslen(InSuffix);
            if (SuffixLen > ThisLen) {
                return false;
            }

            const TCHAR* StrPtr = Data.GetData() + ThisLen - SuffixLen;
            if (SearchCase == ESearchCase::IgnoreCase) {
                return !_wcsicmp(StrPtr, InSuffix);
            } else {
                return !wcscmp(StrPtr, InSuffix);
            }
        }

        /**
         * Test whether this string ends with given string.
         *
         * @param SearchCase		Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
         * @return true if this string ends with specified text, false otherwise
         */
        FORCEINLINE bool EndsWith(const FString& InSuffix, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase) const {
            if( SearchCase == ESearchCase::IgnoreCase ) {
                return InSuffix.Len() > 0 &&
                       Len() >= InSuffix.Len() &&
                       !_wcsicmp( &(*this)[ Len() - InSuffix.Len() ], *InSuffix );
            } else {
                return InSuffix.Len() > 0 &&
                       Len() >= InSuffix.Len() &&
                       !wcscmp( &(*this)[ Len() - InSuffix.Len() ], *InSuffix );
            }
        }

        FORCEINLINE void InsertAt(int32 Index, TCHAR Character) {
            if (Character != 0) {
                if (Data.Num() == 0) {
                    *this += Character;
                } else {
                    Data.Insert(Character, Index);
                }
            }
        }

        FORCEINLINE void InsertAt(int32 Index, const FString& Characters) {
            if (Characters.Len()) {
                if (Data.Num() == 0) {
                    *this += Characters;
                } else {
                    Data.Insert(Characters.Data.GetData(), Characters.Len(), Index);
                }
            }
        }

        FORCEINLINE bool IsEmpty() const {
            return Data.Num() <= 1;
        }

        /** Converts the string to the C-string */
        FORCEINLINE const TCHAR* operator*() const {
            return Data.Num() ? Data.GetData() : STR("");
        }

        /** Returns the character at the provided index */
        FORCEINLINE const TCHAR& operator[](int32 Index) const {
            return Data[Index];
        }

        /** Returns the character at the provided index */
        FORCEINLINE TCHAR& operator[](int32 Index) {
            return Data[Index];
        }

        /** Get the length of the string, excluding terminating character */
        FORCEINLINE int32 Len() const {
            return Data.Num() ? Data.Num() - 1 : 0;
        }

        FORCEINLINE const TCHAR* GetCharArray() const { return Data.GetData(); }
        FORCEINLINE TCHAR* GetCharArray() { return Data.GetData(); }

        FORCEINLINE static FString ConcatFStrings(const FString& Lhs, const FString& Rhs) {
            if (Lhs.IsEmpty()) {
                return Rhs;
            }
            int32 RhsLen = Rhs.Len();

            FString Result(Lhs, RhsLen);
            Result.AppendChars(Rhs.Data.GetData(), RhsLen);
            return Result;
        }

        /**
         * Trims the inner array after the null terminator.
         */
        FORCEINLINE void TrimToNullTerminator() {
            if(Data.Num()) {
                int32 DataLen = wcslen(Data.GetData());
                check(DataLen == 0 || DataLen < Data.Num());
                int32 Len = DataLen > 0 ? DataLen+1 : 0;

                check(Len <= Data.Num());
                Data.RemoveAt(Len, Data.Num()-Len);
            }
        }

        /**
         * Concatenate this path with given path ensuring the / character is used between them
         *
         * @param Str path array of TCHAR to be concatenated onto the end of this
         * @return reference to path
         */
        FORCEINLINE FString& operator/=(const TCHAR* Str) {
            PathAppend(Str, wcslen(Str));
            return *this;
        }

        /**
         * Concatenate this path with given path ensuring the / character is used between them
         * 
         * @param Str path FString to be concatenated onto the end of this
         * @return reference to path
         */
        FORCEINLINE FString& operator/=(const FString& Str) {
            PathAppend(Str.Data.GetData(), Str.Len());
            return *this;
        }

        /**
         * Concatenate this path with given path ensuring the / character is used between them
         *
         * @param Lhs Path to concatenate onto.
         * @param Rhs Path to concatenate.
         * @return new FString of the path
         */
        FORCEINLINE friend FString operator/(const FString& Lhs, const TCHAR* Rhs) {
            int32 StrLength = wcslen(Rhs);

            FString Result(Lhs, StrLength + 1);
            Result.PathAppend(Rhs, StrLength);
            return Result;
        }

        /**
         * Concatenate this path with given path ensuring the / character is used between them
         *
         * @param Lhs Path to concatenate onto.
         * @param Rhs Path to concatenate.
         * @return new FString of the path
         */
        FORCEINLINE friend FString operator/(FString&& Lhs, const TCHAR* Rhs) {
            int32 StrLength = wcslen(Rhs);

            FString Result(MoveTemp(Lhs), StrLength + 1);
            Result.PathAppend(Rhs, StrLength);
            return Result;
        }

        /**
         * Concatenate this path with given path ensuring the / character is used between them
         *
         * @param Lhs Path to concatenate onto.
         * @param Rhs Path to concatenate.
         * @return new FString of the path
         */
        FORCEINLINE friend FString operator/(const FString& Lhs, const FString& Rhs) {
            int32 StrLength = Rhs.Len();

            FString Result(Lhs, StrLength + 1);
            Result.PathAppend(Rhs.Data.GetData(), StrLength);
            return Result;
        }

        /**
         * Concatenate this path with given path ensuring the / character is used between them
         *
         * @param Lhs Path to concatenate onto.
         * @param Rhs Path to concatenate.
         * @return new FString of the path
         */
        FORCEINLINE friend FString operator/(FString&& Lhs, const FString& Rhs) {
            int32 StrLength = Rhs.Len();

            FString Result(MoveTemp(Lhs), StrLength + 1);
            Result.PathAppend(Rhs.Data.GetData(), StrLength);
            return Result;
        }

        /**
         * Concatenate this path with given path ensuring the / character is used between them
         *
         * @param Lhs Path to concatenate onto.
         * @param Rhs Path to concatenate.
         * @return new FString of the path
         */
        FORCEINLINE friend FString operator/(const TCHAR* Lhs, const FString& Rhs) {
            int32 StrLength = Rhs.Len();

            FString Result(FString(Lhs), StrLength + 1);
            Result.PathAppend(Rhs.Data.GetData(), Rhs.Len());
            return Result;
        }

        /**
         * Concatenate this path with given path ensuring the / character is used between them
         *
         * @param Str       Pointer to an array of TCHARs (not necessarily null-terminated) to be concatenated onto the end of this.
         * @param StrLength Exact number of characters from Str to append.
         */
        FORCEINLINE void PathAppend(const TCHAR* Str, int32 StrLength) {
            int32 DataNum = Data.Num();
            if (StrLength == 0) {
                if (DataNum > 1 && Data[DataNum - 2] != TEXT('/') && Data[DataNum - 2] != TEXT('\\')) {
                    Data[DataNum - 1] = TEXT('/');
                    Data.Add(TEXT('\0'));
                }
            } else {
                if (DataNum > 0) {
                    if (DataNum > 1 && Data[DataNum - 2] != TEXT('/') && Data[DataNum - 2] != TEXT('\\') && *Str != TEXT('/')) {
                        Data[DataNum - 1] = TEXT('/');
                    } else {
                        Data.Pop(false);
                        --DataNum;
                    }
                }
                Reserve(DataNum + StrLength);
                Data.Append(Str, StrLength);
                Data.Add(TEXT('\0'));
            }
        }

        /**
         * Concatenate two FStrings.
         *
         * @param Lhs The FString on the left-hand-side of the expression.
         * @param Rhs The FString on the right-hand-side of the expression.
         * @return The concatenated string.
         */
        FORCEINLINE friend FString operator+(const FString& Lhs, const FString& Rhs) {
            return ConcatFStrings(Lhs, Rhs);
        }

        FORCEINLINE friend FString& operator+=(FString& Lhs, const FString& Rhs) {
            Lhs.Append(Rhs);
            return Lhs;
        }

        FORCEINLINE friend FString& operator+=(FString& Lhs, const TCHAR* Rhs) {
            Lhs.Append(Rhs);
            return Lhs;
        }

        FORCEINLINE friend FString& operator+=(FString& Lhs, const TCHAR Rhs) {
            Lhs.AppendChar(Rhs);
            return Lhs;
        }

        FORCEINLINE friend bool operator==(const FString& Lhs, const FString& Rhs) {
            return _wcsicmp(*Lhs, *Rhs) == 0;
        }

        FORCEINLINE friend bool operator==(const FString& Lhs, const TCHAR* Rhs) {
            return _wcsicmp(*Lhs, Rhs) == 0;
        }

        FORCEINLINE friend bool operator==(const TCHAR* Lhs, const FString& Rhs) {
            return _wcsicmp(Lhs, *Rhs) == 0;
        }

        FORCEINLINE friend bool operator>(const FString& Lhs, const FString& Rhs) {
            return _wcsicmp(*Lhs, *Rhs) > 0;
        }

        FORCEINLINE friend bool operator>(const FString& Lhs, const TCHAR* Rhs) {
            return _wcsicmp(*Lhs, Rhs) > 0;
        }

        FORCEINLINE friend bool operator>(const TCHAR* Lhs, const FString& Rhs) {
            return _wcsicmp(Lhs, *Rhs) > 0;
        }

        FORCEINLINE friend bool operator<(const FString& Lhs, const FString& Rhs) {
            return _wcsicmp(*Lhs, *Rhs) < 0;
        }

        FORCEINLINE friend bool operator<(const FString& Lhs, const TCHAR* Rhs) {
            return _wcsicmp(*Lhs, Rhs) < 0;
        }

        FORCEINLINE friend bool operator<(const TCHAR* Lhs, const FString& Rhs) {
            return _wcsicmp(Lhs, *Rhs) < 0;
        }

        /** Returns the left most given number of characters */
        FORCEINLINE FString Left(int32 Count) const {
            return FString(std::clamp(Count,0,Len()), **this);
        }

        /** Modifies the string such that it is now the left most given number of characters */
        FORCEINLINE void LeftInline(int32 Count, bool bAllowShrinking = true) {
            const int32 Length = Len();
            Count = std::clamp(Count, 0, Length);
            RemoveAt(Count, Length-Count, bAllowShrinking);
        }

        /** Returns the left most characters from the string chopping the given number of characters from the end */
        FORCEINLINE FString LeftChop(int32 Count) const {
            const int32 Length = Len();
            return FString(std::clamp(Length-Count,0, Length), **this);
        }

        /** Modifies the string such that it is now the left most characters chopping the given number of characters from the end */
        FORCEINLINE void LeftChopInline(int32 Count, bool bAllowShrinking = true) {
            const int32 Length = Len();
            RemoveAt(std::clamp(Length-Count, 0, Length), Count, bAllowShrinking);
        }

        /** Returns the string to the right of the specified location, counting back from the right (end of the word). */
        FORCEINLINE FString Right(int32 Count) const {
            const int32 Length = Len();
            return FString(**this + Length-std::clamp(Count,0,Length));
        }

        /** Modifies the string such that it is now the right most given number of characters */
        FORCEINLINE void RightInline(int32 Count, bool bAllowShrinking = true) {
            const int32 Length = Len();
            RemoveAt(0, Length - std::clamp(Count,0,Length), bAllowShrinking);
        }

        /** Returns the string to the right of the specified location, counting forward from the left (from the beginning of the word). */
        FORCEINLINE FString RightChop(int32 Count) const {
            const int32 Length = Len();
            return FString(**this + Length-std::clamp(Length-Count,0, Length));
        }

        /** Modifies the string such that it is now the string to the right of the specified location, counting forward from the left (from the beginning of the word). */
        FORCEINLINE void RightChopInline(int32 Count, bool bAllowShrinking = true) {
            RemoveAt(0, Count, bAllowShrinking);
        }

        /** Returns the substring from Start position for Count characters. */
        FORCEINLINE FString Mid(int32 Start, int32 Count = INT32_MAX) const {
            FString Result;
            if (Count >= 0)
            {
                const int32 Length = Len();
                const int32 RequestedStart = Start;
                Start = std::clamp(Start, 0, Length);
                const int32 End = (int32) std::clamp((int64)Count + RequestedStart, (int64)Start, (int64)Length);
                Result = FString(End-Start, **this + Start);
            }
            return Result;
        }

        /**
         * Constructs FString object similarly to how classic sprintf works.
         *
         * @param Format	Format string that specifies how FString should be built optionally using additional args. Refer to standard printf format.
         * @param ...		Depending on format function may require additional arguments to build output object.
         *
         * @returns FString object that was constructed using format and additional parameters.
         */
        template <typename... Types>
        static FString Printf(const TCHAR* Fmt, Types... Args) {
            int BufferLength = _snwprintf(nullptr, 0, Fmt, Args...) + 1;
            FString ResultString;
            ResultString.Data.AddZeroed(BufferLength);

            _snwprintf(ResultString.Data.GetData(), BufferLength, Fmt, Args...);
            return ResultString;
        }

        /**
         * DO NOT USE DIRECTLY
         * STL-like iterators to enable range-based for loop support.
         */
        FORCEINLINE TCHAR* begin()             { auto Result = Data.begin();                                 return Result; }
        FORCEINLINE const TCHAR* begin() const { auto Result = Data.begin();                                 return Result; }
        FORCEINLINE TCHAR* end()               { auto Result = Data.end();   if (Data.Num()) { --Result; }   return Result; }
        FORCEINLINE const TCHAR* end() const   { auto Result = Data.end();   if (Data.Num()) { --Result; }   return Result; }
    };
}

#endif //RC_UNREAL_FSTRING_HPP
