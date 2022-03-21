#include <Unreal/UObjectGlobals.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UnrealVersion.hpp>
#include <Unreal/VersionedContainer/Container.hpp>

namespace RC::Unreal::UObjectGlobals
{
    auto SetupStaticConstructObjectInternalAddress(void* FunctionAddress) -> void
    {
        GlobalState::StaticConstructObjectInternal.assign_address(FunctionAddress);
        GlobalState::StaticConstructObjectInternalDeprecated.assign_address(FunctionAddress);
    }

    auto VersionIsAtMost(uint32_t Major, uint32_t Minor) -> bool
    {
        return Unreal::Version::IsAtMost(Major, Minor);
    }

    auto StaticFindObjectImpl([[maybe_unused]]UClass* ObjectClass, [[maybe_unused]]UObject* InObjectPackage, const wchar_t* OrigInName, [[maybe_unused]]bool bExactClass) -> UObject*
    {
        UObject* FoundObject{};

        UObjectGlobals::ForEachUObject([&](UObject* Object, [[maybe_unused]]int32_t ChunkIndex, [[maybe_unused]]int32_t ObjectIndex) {
            // This call to 'get_full_name' is a problem because it relies on offsets already being found
            // This function is called before offsets have been found as a way to check if required objects have been initialized
            auto ObjFullName = Object->GetFullName();
            auto ObjFullNameNoType = ObjFullName.substr(ObjFullName.find(STR(" ")) + 1);

            if (ObjFullNameNoType == OrigInName)
            {
                FoundObject = static_cast<UObject*>(Object);
                return LoopAction::Break;
            }
            else
            {
                return LoopAction::Continue;
            }
        });

        return FoundObject;
    }

    auto static IsValidObjectForFindXOf(UObject* object) -> bool
    {
        return !object->HasAnyFlags(static_cast<EObjectFlags>(RF_ClassDefaultObject | RF_ArchetypeObject)) && !object->IsA<UClass>();
    }

    auto FindFirstOf(FName ClassName) -> UObject*
    {
        UObject* ObjectFound{nullptr};

        UObjectGlobals::ForEachUObject([&](UObject* Object, [[maybe_unused]]int32_t ChunkIndex, [[maybe_unused]]int32_t ObjectIndex) {
            UClass* Class = Object->GetClass();

            if (Class->GetFName().Equals(ClassName) && IsValidObjectForFindXOf(Object))
            {
                ObjectFound = Object;
                return LoopAction::Break;

            }

            if (!IsValidObjectForFindXOf(Object)) { return LoopAction::Continue; }

            Class->ForEachSuperStruct([&](UStruct* super_struct) {
                if (super_struct->GetFName().Equals(ClassName))
                {
                    ObjectFound = Object;
                    return LoopAction::Break;
                }

                return LoopAction::Continue;
            });

            return LoopAction::Continue;
        });

        return ObjectFound;
    }

    auto FindFirstOf(const wchar_t* ClassName) -> UObject*
    {
        return FindFirstOf(FName(ClassName));
    }

    auto FindFirstOf(std::wstring_view ClassName) -> UObject*
    {
        return FindFirstOf(FName(ClassName));
    }

    auto FindFirstOf(const std::wstring& ClassName) -> UObject*
    {
        return FindFirstOf(FName(ClassName));
    }

    auto FindFirstOf(std::string_view ClassName) -> UObject*
    {
        return FindFirstOf(FName(to_wstring(ClassName)));
    }

    auto FindFirstOf(const std::string& ClassName) -> UObject*
    {
        return FindFirstOf(FName(to_wstring(ClassName)));
    }

    auto FindAllOf(FName ClassName, std::vector<UObject*>& OutStorage) -> void
    {
        UObjectGlobals::ForEachUObject([&](UObject* Object, [[maybe_unused]]int32_t ChunkIndex, [[maybe_unused]]int32_t ObjectIndex) {
            if (!Object) { return LoopAction::Continue; }

            UClass* Class = Object->GetClass();

            if (Class->GetFName().Equals(ClassName) && IsValidObjectForFindXOf(Object))
            {
                OutStorage.emplace_back(Object);
                return LoopAction::Continue;
            }

            if (!IsValidObjectForFindXOf(Object)) { return LoopAction::Continue; }

            Class->ForEachSuperStruct([&](UStruct* SuperStruct) {
                if (SuperStruct->GetFName().Equals(ClassName))
                {
                    OutStorage.emplace_back(Object);
                    return LoopAction::Break;
                }

                return LoopAction::Continue;
            });

            return LoopAction::Continue;
        });
    }

    auto FindAllOf(const wchar_t* ClassName, std::vector<UObject*>& OutStorage) -> void
    {
        FindAllOf(FName(ClassName), OutStorage);
    }

    auto FindAllOf(std::wstring_view ClassName, std::vector<UObject*>& OutStorage) -> void
    {
        FindAllOf(FName(ClassName), OutStorage);
    }

    auto FindAllOf(const std::wstring& ClassName, std::vector<UObject*>& OutStorage) -> void
    {
        FindAllOf(FName(ClassName), OutStorage);
    }

    auto FindAllOf(std::string_view ClassName, std::vector<UObject*>& OutStorage) -> void
    {
        FindAllOf(FName(to_wstring(ClassName)), OutStorage);
    }

    auto FindAllOf(const std::string& ClassName, std::vector<UObject*>& OutStorage) -> void
    {
        FindAllOf(FName(to_wstring(ClassName)), OutStorage);
    }

    auto FindObjects(size_t NumObjectsToFind, const FName ClassName, const FName ObjectShortName, std::vector<UObject*>& OutStorage, int32 RequiredFlags, int32 BannedFlags) -> void
    {
        bool bCareAboutClass = ClassName != FName(0u, 0u);
        bool bCareAboutName = ObjectShortName != FName(0u, 0u);

        if (!bCareAboutClass && !bCareAboutName)
        {
            throw std::runtime_error{"[UObjectGlobals::find_objects] Must supply class_name, object_short_name, or both"};
        }

        size_t NumObjectsFound{};

        ForEachUObject([&](UObject* Object, int32, int32) {
            bool bNameMatches{};
            if (bCareAboutName && Object->GetFName().Equals(ObjectShortName))
            {
                bNameMatches = true;
            }

            bool bClassMatches{};
            if (bCareAboutClass && Object->GetClass()->GetFName().Equals(ClassName))
            {
                bClassMatches = true;
            }

            if ((bCareAboutClass && bClassMatches && bCareAboutName && bNameMatches) ||
                (!bCareAboutName && bCareAboutClass && bClassMatches) ||
                (!bCareAboutClass && bCareAboutName && bNameMatches))
            {
                bool bRequiredFlagsPasses = RequiredFlags == EObjectFlags::RF_NoFlags || Object->HasAllFlags(static_cast<EObjectFlags>(RequiredFlags));
                bool bBannedFlagsPasses = BannedFlags == EObjectFlags::RF_NoFlags || !Object->HasAnyFlags(static_cast<EObjectFlags>(BannedFlags));

                if (bRequiredFlagsPasses && bBannedFlagsPasses)
                {
                    OutStorage.emplace_back(Object);
                    ++NumObjectsFound;
                }
            }

            if (NumObjectsToFind != 0 && NumObjectsFound >= NumObjectsToFind)
            {
                return LoopAction::Break;
            }
            else
            {
                return LoopAction::Continue;
            }
        });
    }

    auto FindObjects(size_t NumObjectsToFind, const wchar_t* ClassName, const wchar_t* ObjectShortName, std::vector<UObject*>& OutStorage, int32 RequiredFlags, int32 BannedFlags) -> void
    {
        FindObjects(NumObjectsToFind, FName(ClassName), FName(ObjectShortName), OutStorage, RequiredFlags, BannedFlags);
    }

    auto find_objects(size_t NumObjectsToFind, const wchar_t* ClassName, std::vector<UObject*>& OutStorage) -> void
    {
        FindObjects(NumObjectsToFind, FName(ClassName), FName(0u, 0u), OutStorage);
    }

    auto FindObject(const FName ClassName, const FName ObjectShortName, int32 RequiredFlags, int32 BannedFlags) -> UObject*
    {
        std::vector<UObject*> FoundObject{};
        FindObjects(1, ClassName, ObjectShortName, FoundObject, RequiredFlags, BannedFlags);

        if (FoundObject.empty())
        {
            return nullptr;
        }
        else
        {
            return FoundObject[0];
        }
    };

    auto FindObjects(const FName ClassName, const FName ObjectShortName, std::vector<UObject*>& OutStorage, int32 RequiredFlags, int32 BannedFlags) -> void
    {
        FindObjects(0, ClassName, ObjectShortName, OutStorage, RequiredFlags, BannedFlags);
    }

    auto FindObjects(const wchar_t* ClassName, const wchar_t* ObjectShortName, std::vector<UObject*>& OutStorage, int32 RequiredFlags, int32 BannedFlags) -> void
    {
        FindObjects(0, ClassName, ObjectShortName, OutStorage, RequiredFlags, BannedFlags);
    }

    auto FindObject(const wchar_t* ClassName, const wchar_t* ObjectShortName, int32 RequiredFlags, int32 BannedFlags) -> UObject*
    {
        return FindObject(FName(ClassName), FName(ObjectShortName), RequiredFlags, BannedFlags);
    }

    auto ForEachUObject(const std::function<LoopAction(UObject*, int32, int32)>& Callable) -> void
    {
        Container::UnrealVC->UObjectArray_for_each_uobject([&](void* RawObject, int32 ObjectIndex, int32 ChunkIndex)
        {
            return Callable(static_cast<UObject*>(RawObject), ObjectIndex, ChunkIndex);
        });
    };
}

