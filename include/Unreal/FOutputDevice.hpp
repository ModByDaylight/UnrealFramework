#ifndef RC_UNREAL_FOUTPUTDEVICE_HPP
#define RC_UNREAL_FOUTPUTDEVICE_HPP

#include <unordered_map>

#include <Unreal/Common.hpp>
#include <Unreal/PrimitiveTypes.hpp>
#include <Unreal/LogVerbosity.hpp>

namespace RC::Unreal
{
    struct FName;
    class FArchive;

    class RC_UE_API FOutputDevice
    {
    protected:
        // For alignment purposes
        uint8_t* vtable_;

        /** Whether to output the 'Log: ' type front... */
        bool bSuppressEventTag;
        /** Whether to output a line-terminator after each log call... */
        bool bAutoEmitLineTerminator;

    public:
        static std::unordered_map<std::wstring, uint32_t> VTableLayoutMap;

    public:
#include <MemberVariableLayout_HeaderWrapper_FOutputDevice.hpp>

        // Wrappers for virtual engine functions
        // Missing the first overload because the generator doesn't support overloads
        void Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const FName& Category, const double Time);
        void Flush();
        void TearDown();
        void Dump(class FArchive& Ar);
        bool IsMemoryOnly() const;
        bool CanBeUsedOnAnyThread() const;
        // Added in 4.21, throws in lesser versions
        bool CanBeUsedOnMultipleThreads() const;

    public:
        void Log(const TCHAR* Str);
    };
}

#endif //RC_UNREAL_FOUTPUTDEVICE_HPP
