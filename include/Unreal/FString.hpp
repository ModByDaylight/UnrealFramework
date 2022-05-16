#ifndef RC_UNREAL_FSTRING_HPP
#define RC_UNREAL_FSTRING_HPP

#include <Function/Function.hpp>
#include <Unreal/Common.hpp>
#include <Unreal/TArray.hpp>

namespace RC::Unreal
{
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
        template<class CharType>
        FORCEINLINE FString& Append(const CharType* Str, int32 Count) {
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

        FORCEINLINE bool IsEmpty() const {
            return Data.Num() <= 1;
        }

        /** Converts the string to the C-string */
        FORCEINLINE const TCHAR* operator*() const {
            return Data.Num() ? Data.GetData() : STR("");
        }

        /** Returns the character at the provided index */
        FORCEINLINE TCHAR operator[](int32 Index) const {
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
         * Concatenate two FStrings.
         *
         * @param Lhs The FString on the left-hand-side of the expression.
         * @param Rhs The FString on the right-hand-side of the expression.
         * @return The concatenated string.
         */
        FORCEINLINE friend FString operator+(const FString& Lhs, const FString& Rhs) {
            return ConcatFStrings(Lhs, Rhs);
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

        FORCEINLINE friend std::weak_ordering operator<=>(const FString& Lhs, const FString& Rhs) {
            int compareResult = _wcsicmp(*Lhs, *Rhs);
            if (compareResult == 0)
                return std::weak_ordering::equivalent;
            return compareResult < 0 ? std::weak_ordering::less : std::weak_ordering::greater;
        }

        FORCEINLINE friend std::weak_ordering operator<=>(const FString& Lhs, const TCHAR* Rhs) {
            int compareResult = _wcsicmp(*Lhs, Rhs);
            if (compareResult == 0)
                return std::weak_ordering::equivalent;
            return compareResult < 0 ? std::weak_ordering::less : std::weak_ordering::greater;
        }

        FORCEINLINE friend std::weak_ordering operator<=>(const TCHAR* Lhs, const FString& Rhs) {
            int compareResult = _wcsicmp(Lhs, *Rhs);
            if (compareResult == 0)
                return std::weak_ordering::equivalent;
            return compareResult < 0 ? std::weak_ordering::less : std::weak_ordering::greater;
        }

        /** Returns the left most given number of characters */
        FORCEINLINE FString Left(int32 Count) const {
            return FString(std::clamp(Count,0,Len()), **this);
        }

        /** Returns the left most characters from the string chopping the given number of characters from the end */
        FORCEINLINE FString LeftChop(int32 Count) const {
            const int32 Length = Len();
            return FString(std::clamp(Length-Count,0, Length), **this);
        }

        /** Returns the string to the right of the specified location, counting back from the right (end of the word). */
        FORCEINLINE FString Right(int32 Count) const {
            const int32 Length = Len();
            return FString(**this + Length-std::clamp(Count,0,Length));
        }

        /** Returns the string to the right of the specified location, counting forward from the left (from the beginning of the word). */
        FORCEINLINE FString RightChop(int32 Count) const {
            const int32 Length = Len();
            return FString(**this + Length-std::clamp(Length-Count,0, Length));
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
    };
}

#endif //RC_UNREAL_FSTRING_HPP
