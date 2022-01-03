#include <Unreal/VersionedContainer/Flags415.hpp>
#include <Unreal/VersionedContainer/Flags.hpp>

namespace RC::Unreal
{
    std::unordered_map<EObjectFlags, Flags415::EObjectFlags_Impl> Flags415::object_flags_converter{
            {EObjectFlags::EObjectFlags_DoesntExist, static_cast<EObjectFlags_Impl>(-1)},
            {EObjectFlags::RF_NoFlags, EObjectFlags_Impl::RF_NoFlags},
            {EObjectFlags::RF_Public, EObjectFlags_Impl::RF_Public},
            {EObjectFlags::RF_Standalone, EObjectFlags_Impl::RF_Standalone},
            {EObjectFlags::RF_MarkAsNative, EObjectFlags_Impl::RF_MarkAsNative},
            {EObjectFlags::RF_Transactional, EObjectFlags_Impl::RF_Transactional},
            {EObjectFlags::RF_ClassDefaultObject, EObjectFlags_Impl::RF_ClassDefaultObject},
            {EObjectFlags::RF_ArchetypeObject, EObjectFlags_Impl::RF_ArchetypeObject},
            {EObjectFlags::RF_Transient, EObjectFlags_Impl::RF_Transient},
            {EObjectFlags::RF_MarkAsRootSet, EObjectFlags_Impl::RF_MarkAsRootSet},
            {EObjectFlags::RF_TagGarbageTemp, EObjectFlags_Impl::RF_TagGarbageTemp},
            {EObjectFlags::RF_NeedInitialization, EObjectFlags_Impl::RF_NeedInitialization},
            {EObjectFlags::RF_NeedLoad, EObjectFlags_Impl::RF_NeedLoad},
            {EObjectFlags::RF_KeepForCooker, EObjectFlags_Impl::RF_KeepForCooker},
            {EObjectFlags::RF_NeedPostLoad, EObjectFlags_Impl::RF_NeedPostLoad},
            {EObjectFlags::RF_NeedPostLoadSubobjects, EObjectFlags_Impl::RF_NeedPostLoadSubobjects},
            {EObjectFlags::RF_NewerVersionExists, EObjectFlags_Impl::RF_NewerVersionExists},
            {EObjectFlags::RF_BeginDestroyed, EObjectFlags_Impl::RF_BeginDestroyed},
            {EObjectFlags::RF_FinishDestroyed, EObjectFlags_Impl::RF_FinishDestroyed},
            {EObjectFlags::RF_BeingRegenerated, EObjectFlags_Impl::RF_BeingRegenerated},
            {EObjectFlags::RF_DefaultSubObject, EObjectFlags_Impl::RF_DefaultSubObject},
            {EObjectFlags::RF_WasLoaded, EObjectFlags_Impl::RF_WasLoaded},
            {EObjectFlags::RF_TextExportTransient, EObjectFlags_Impl::RF_TextExportTransient},
            {EObjectFlags::RF_LoadCompleted, EObjectFlags_Impl::RF_LoadCompleted},
            {EObjectFlags::RF_InheritableComponentTemplate, EObjectFlags_Impl::RF_InheritableComponentTemplate},
            {EObjectFlags::RF_DuplicateTransient, EObjectFlags_Impl::RF_DuplicateTransient},
            {EObjectFlags::RF_StrongRefOnFrame, EObjectFlags_Impl::RF_StrongRefOnFrame},
            {EObjectFlags::RF_NonPIEDuplicateTransient, EObjectFlags_Impl::RF_NonPIEDuplicateTransient},
            {EObjectFlags::RF_Dynamic, EObjectFlags_Impl::RF_Dynamic},
            {EObjectFlags::RF_WillBeLoaded, static_cast<EObjectFlags_Impl>(-1)},
            {EObjectFlags::RF_HasExternalPackage, static_cast<EObjectFlags_Impl>(-1)},
    };

    auto Flags415::to_impl_flags(const std::array<EObjectFlags, EObjectFlags_Max>& flags) -> EObjectFlags_Impl
    {
        EObjectFlags_Impl impl_flags{EObjectFlags_Impl::RF_NoFlags};

        for (const auto& flag : flags)
        {
            int32_t impl_flag = static_cast<int32_t>(object_flags_converter[flag]);

            // If 'impl_flag' is -1 then this implementation of the flags doesn't have this particular flag, so ignore it
            if (impl_flag != -1)
            {
                Flags::set_flags(&impl_flags, static_cast<EObjectFlags_Impl>(impl_flag));
            }
        }

        return impl_flags;
    }
}