#include <Unreal/UClass.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/FProperty.hpp>
#include <Helpers/Casting.hpp>
#include <Unreal/TPair.hpp>

namespace RC::Unreal
{
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UScriptStruct);

    using MemberOffsets = ::RC::Unreal::StaticOffsetFinder::MemberOffsets;
    std::unordered_map<std::wstring, uint32_t> UScriptStruct::ICppStructOps::VTableLayoutMap;

#include <MemberVariableLayout_SrcWrapper_UScriptStruct__ICppStructOps.hpp>
#include <MemberVariableLayout_SrcWrapper_UScriptStruct.hpp>

    void UScriptStruct::ICppStructOps::Construct(void *Dest) {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(ICppStructOps, Construct, void, PARAMS(void*), PARAMS(Dest));
    }

    void UScriptStruct::ICppStructOps::Destruct(void *Dest) {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(ICppStructOps, Destruct, void, PARAMS(void*), PARAMS(Dest));
    }

    bool UScriptStruct::ICppStructOps::Copy(void *Dest, const void *Src, int32 ArrayDim) {
        IMPLEMENT_UNREAL_VIRTUAL_WRAPPER(ICppStructOps, Copy, bool, PARAMS(void*, const void*, int32), PARAMS(Dest, Src, ArrayDim));
    }

    auto UScriptStruct::GetSuperScriptStruct() -> UScriptStruct* {
        return Cast<UScriptStruct>(GetSuperStruct());
    }

    void UScriptStruct::CopyScriptStruct(void* InDest, void const* InSrc, int32 ArrayDim) {
        uint8 *Dest = (uint8*)InDest;
        uint8 const* Src = (uint8 const*)InSrc;
        int32 Stride = GetStructureSize();

        if (GetStructFlags() & STRUCT_CopyNative) {
            UScriptStruct::ICppStructOps* TheCppStructOps = GetCppStructOps();

            if (TheCppStructOps->Copy(Dest, Src, ArrayDim)) {
                return;
            }
        }
        if (GetStructFlags() & STRUCT_IsPlainOldData) {
            FMemory::Memcpy(Dest, Src, ArrayDim * Stride);
        } else {
            ForEachPropertyInChain([&](FProperty* Property) {
                for (int32 Index = 0; Index < ArrayDim; Index++) {
                    Property->CopyCompleteValue_InContainer((uint8*)Dest + Index * Stride, (uint8*)Src + Index * Stride);
                }
                return LoopAction::Continue;
            });
        }
    }
}