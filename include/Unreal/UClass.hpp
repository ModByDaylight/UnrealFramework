#ifndef RC_UNREAL_UCLASS_HPP
#define RC_UNREAL_UCLASS_HPP

#include <format>

#include <Helpers/Casting.hpp>
#include <Helpers/Casting.hpp>

#include <Unreal/Common.hpp>
#include <Unreal/StaticOffsetFinder.hpp>
#include <Unreal/UStruct.hpp>
#include <Unreal/TypeChecker.hpp>

namespace RC::Unreal
{
    template<typename ArrayInnerType>
    class TArray;

    // TODO: Move to a more appropriate place ?
    struct RC_API FImplementedInterface
    {
        UClass* Class;
        int32_t PointerOffset;
        bool bImplementedByK2;
    };

    class RC_API UClass : public UStruct
    {
    private:
        static inline class UClass* m_static_obj{};

    public:
        // DO NOT USE... MEANT FOR INTERNAL USE ONLY
        auto static set_static_obj_ptr(class UClass* ptr)
        {
            m_static_obj = ptr;
        }

        auto static static_class() -> class UClass*
        {
            if (!m_static_obj) { throw std::runtime_error{"[UClass::get_static_obj_ptr] m_static_obj is nullptr"}; }

            return m_static_obj;
        }

    public:
        struct Offset
        {
            /* Temporary implementation, based on:
                ClassConstructorType ClassConstructor;
                ClassVTableHelperCtorCallerType ClassVTableHelperCtorCaller;
                ClassAddReferencedObjectsType ClassAddReferencedObjects;
                mutable uint32 ClassUnique:31;                                   // 0x??
                uint32 bCooked:1;                                                // 0x??
                EClassFlags ClassFlags;                                          // 0xCC
                EClassCastFlags ClassCastFlags;                                  // 0xD0
                UClass* ClassWithin;                                             // 0xD8
                UObject* ClassGeneratedBy;                                       // 0xE0
                FName ClassConfigName;                                           // 0xE8
                TArray<FRepRecord> ClassReps;                                    // 0xF0
                TArray<UField*> NetFields;                                       // 0x100 -
                int32 FirstOwnedClassRep = 0;                                    // 0x114 -0x4
                UObject* ClassDefaultObject;                                     // 0x118
                void* SparseClassData;                                           // 0x120
                UScriptStruct* SparseClassDataStruct;                            // 0x128
                TMap<FName, UFunction*> FuncMap;                                 // 0x130
                mutable TMap<FName, UFunction*> SuperFuncMap;                    // 0x180
                mutable FRWLock SuperFuncMapLock;                                // 0x??? ; size 0x8
                TArray<FImplementedInterface> Interfaces;                        // 0x1D8 ?
            //*/
            static inline int32_t class_config_name;
        };

    public:
        auto has_any_class_flags(EClassFlags flags_to_check) -> bool;
        auto has_all_class_flags(EClassFlags flags_to_check) -> bool;
        auto get_class_flags() -> EClassFlags;
        auto get_class_within() -> UClass*;
        auto get_class_config_name() -> FName;
        auto get_class_default_object() -> UObject*;
        auto get_interfaces() -> TArray<FImplementedInterface>;

        auto get_super_class() -> UClass*
        {
            return reinterpret_cast<UClass*>(get_super_struct());
        }

        static auto to_string(void* p_this, std::wstring& out_line) -> void;
    };

    class RC_API UBlueprintGeneratedClass : public UClass
    {
    private:
        static inline class UClass* m_static_obj{};

    public:
        // DO NOT USE... MEANT FOR INTERNAL USE ONLY
        auto static set_static_obj_ptr(class UClass* ptr)
        {
            m_static_obj = ptr;
        }

        auto static static_class() -> class UClass*
        {
            if (!m_static_obj) { throw std::runtime_error{"[UBlueprintGeneratedClass::get_static_obj_ptr] m_static_obj is nullptr"}; }

            return m_static_obj;
        }
    };

    class RC_API UAnimBlueprintGeneratedClass : public UClass
    {
    private:
        static inline class UClass* m_static_obj{};

    public:
        // DO NOT USE... MEANT FOR INTERNAL USE ONLY
        auto static set_static_obj_ptr(class UClass* ptr)
        {
            m_static_obj = ptr;
        }

        auto static static_class() -> class UClass*
        {
            if (!m_static_obj) { throw std::runtime_error{"[UAnimBlueprintGeneratedClass::get_static_obj_ptr] m_static_obj is nullptr"}; }

            return m_static_obj;
        }
    };
}


#endif //RC_UNREAL_UCLASS_HPP
