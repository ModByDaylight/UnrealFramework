#ifndef RC_UNREAL_USCRIPTSTRUCT_HPP
#define RC_UNREAL_USCRIPTSTRUCT_HPP

#include <Unreal/StaticOffsetFinder.hpp>
#include <Unreal/UStruct.hpp>
#include <Unreal/TypeChecker.hpp>

namespace RC::Unreal {

    class RC_UE_API UScriptStruct : public UStruct {
        DECLARE_EXTERNAL_OBJECT_CLASS(UScriptStruct, CoreUObject);
    public:
        struct RC_UE_API ICppStructOps {
            static std::unordered_map<std::wstring, uint32_t> VTableLayoutMap;
#include <MemberVariableLayout_HeaderWrapper_UScriptStruct__ICppStructOps.hpp>
        public:
            /** Call the C++ constructor **/
            void Construct(void *Dest);
            /** Call the C++ destructor **/
            void Destruct(void *Dest);
            /** Copy this structure */
            bool Copy(void* Dest, void const* Src, int32 ArrayDim);
        };
#include <MemberVariableLayout_HeaderWrapper_UScriptStruct.hpp>
    public:
        /**
         * Returns the struct acting as a parent to this struct
         * Can totally return nullptr since most structs do not have a parent struct
         */
        auto GetSuperScriptStruct() -> UScriptStruct*;

        /**
         * Checks if the struct has any of the provided flags set
         * Returns true if at least one of the provided struct flags is set
         */
        inline auto HasAnyStructFlags(EStructFlags flags_to_check) {
            return (GetStructFlags() & flags_to_check) != 0;
        }

        /**
         * Checks if the struct has all of the provided flags set
         * Returns true only if all of the provided struct flags are set
         */
        inline auto HasAllStructFlags(EStructFlags flags_to_check) {
            return (GetStructFlags() & flags_to_check) == flags_to_check;
        }

        void CopyScriptStruct(void* InDest, void const* InSrc, int32 ArrayDim = 1);
    };
}

#endif //RC_UNREAL_USCRIPTSTRUCT_HPP
