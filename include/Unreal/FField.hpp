#ifndef RC_UNREAL_FFIELD_HPP
#define RC_UNREAL_FFIELD_HPP

#include <Constructs/Loop.hpp>

#include <Unreal/Common.hpp>
#include <Unreal/NameTypes.hpp>
#include <Unreal/UnrealVersion.hpp>

namespace RC::Unreal
{
    class UClass;
    class UStruct;
    class FFieldVariant;
    class FFieldClassVariant;

    template<typename SupposedFField>
    concept FFieldDerivative = std::is_convertible_v<SupposedFField, ::RC::Unreal::FField>;

    class RC_API FField
    {
    public:
        using MemberOffsets = ::RC::Unreal::StaticOffsetFinder::MemberOffsets;

    private:

    public:
        // Maps to UObject::get_class in <4.25
        // Meaning that it works no matter what for getting field types using 'get_fname'
        [[nodiscard]] auto get_ffieldclass() const -> class FFieldClass*;

        // For properties
        // TODO: Rename ?
        template<typename ChildType = FField*>
        auto get_next() -> ChildType
        {
            return Helper::Casting::offset_deref<ChildType>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::XField_Next));
        }

        //auto get_ffield_fname() -> FName;
        //auto get_ffield_fname() const -> FName;
        //auto get_ffield_name() -> std::wstring;
        // The following 4 functions map to 'UObject::get_fname' in <4.25
        // These are hiding the UObject versions because the UObject versions aren't safe for FField
        auto get_fname() -> FName;
        auto get_fname() const -> FName;
        auto get_name() -> File::StringType;
        auto get_name() const -> File::StringType;
        auto get_type_fname() -> FName;
        auto get_type_name() -> std::wstring;

        auto is_child_of(FFieldClassVariant field) -> bool;

        template<FFieldDerivative FFieldDerivedType>
        auto is_child_of() -> bool
        {
            return is_child_of(FFieldDerivedType::static_class());
        }

        // TODO: Figure out if these should return 'Outer' instead of 'Owner' in <4.25
        auto get_owner() -> UStruct*;
        auto get_owner() const -> UStruct*;

        // 4.25+ only, don't ever call directly
        auto get_owner_variant() -> FFieldVariant;
        auto get_owner_variant() const -> FFieldVariant;

        auto get_outermost_owner() -> UObject*;

        // Use only after confirming that the version of UE is <4.25
        auto as_object() const -> const class UField*;
        auto as_object() -> class UField*;
    };

    class RC_API FFieldClass
    {
    public:
        FName name;
        uint64_t id;
        uint64_t cast_flags;
        EClassFlags class_flags;
        FFieldClass* super_class;
        FField* default_object;
        FField* construct_fn;
        //FThreadSafeCounter unique_name_index_counter;

    public:
        auto get_fname() -> FName;
    };

    class RC_API FFieldVariant
    {
        union FFieldObjectUnion
        {
            FField* field;
            UObject* object;
        } container;

        bool is_object;

    public:
        FFieldVariant(FField* field) : is_object(false)
        {
            container.field = field;
        }

        FFieldVariant(UObject* object) : is_object(true)
        {
            container.object = object;
        }

    public:
        auto is_valid() -> bool
        {
            return container.object;
        }

        auto get_owner_variant() -> FFieldVariant
        {
            if (is_uobject())
            {
                return container.object->get_outer();
            }
            else
            {
                return container.field->get_owner_variant();
            }
        }

        auto is_uobject() -> bool
        {
            return is_object;
        }

        auto to_field() -> FField*
        {
            if (!is_uobject())
            {
                return container.field;
            }
            else
            {
                return nullptr;
            }
        }

        auto to_uobject() -> UObject*
        {
            if (is_uobject())
            {
                return container.object;
            }
            else
            {
                return nullptr;
            }
        }
    };

    class RC_API FFieldClassVariant
    {
    private:
        union FFieldClassObjectUnion
        {
            FFieldClass* field;
            UClass* object;
        } container;

        bool is_object;

    public:
        FFieldClassVariant(FFieldClass* field) : is_object(false)
        {
            container.field = field;
        }

        FFieldClassVariant(UClass* object) : is_object(true)
        {
            container.object = object;
        }

        auto operator=(FFieldClass* new_field) -> void
        {
            is_object = false;
            container.field = new_field;
        }

        auto operator=(UClass* new_object) -> void
        {
            is_object = true;
            container.object = new_object;
        }

    public:
        auto is_valid() -> bool
        {
            return container.object;
        }

        auto is_uobject() -> bool
        {
            return Version::is_below(4, 25) || is_object;
        }

        auto to_field() -> FFieldClass*
        {
            if (!is_uobject())
            {
                return container.field;
            }
            else
            {
                return nullptr;
            }
        }

        auto to_uobject() -> UClass*
        {
            if (is_uobject())
            {
                return container.object;
            }
            else
            {
                return nullptr;
            }
        }

        auto to_flat() -> uintptr_t
        {
            return reinterpret_cast<uintptr_t>(container.object);
        }
    };
}

#endif //RC_UNREAL_FFIELD_HPP
