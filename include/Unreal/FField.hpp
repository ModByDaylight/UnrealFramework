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
    class FField;
    class FFieldVariant;
    class FFieldClassVariant;
    class FFieldClass;

    template<typename SupposedFField>
    concept FFieldDerivative = std::is_convertible_v<SupposedFField, ::RC::Unreal::FField>;

    /**
     * Represents an FField object on the UE versions above UE4.25
     * On the earlier versions it will be falling back to the UField behavior, allowing the version
     * independent data to the FField data, which is primarily used for working with FProperties
     *
     * This class CANNOT be used to represent UField* objects on UE 4.25+, use UField instead!!
     */
    class RC_UE_API FField
    {
    protected:
        using MemberOffsets = ::RC::Unreal::StaticOffsetFinder::MemberOffsets;
        static FFieldClassVariant m_static_class;
    public:
        /**
         * Returns the FFieldClassVariant representing FField
         */
        auto static static_class() -> FFieldClassVariant;

        /**
         * Returns the class of the field, depending on the UE version it would
         * be either FFieldClass* (on UE4.25+), or UClass* object
         * Regardless of the underlying type, you can retrieve the name using get_name
         * and check the class hierarchy using get_superclass and is_child_of
         */
        auto get_class() -> FFieldClassVariant;

        /**
         * Returns name of this property, as FName instance
         */
        auto get_fname() -> FName;

        /**
        * Returns name of this property, as string
        */
        inline auto get_name() -> std::wstring
        {
            return get_fname().to_string();
        }

        /**
         * Checks whenever this property is of the class specified by the argument
         */
        auto is_a(const FFieldClassVariant& uclass) -> bool;

        template<FFieldDerivative FFieldDerivedType>
        auto is_a() -> bool
        {
            return is_a(FFieldDerivedType::static_class());
        }

        /**
         * Returns the owner of this field
         * In versions below 4.25, it would always represent the object
         */
        auto get_owner_variant() -> FFieldVariant;

        /**
         * Returns the first UObject owner of this field
         *
         * In UE4.25+, it would be the first owner that is UObject
         * In earlier version it would be field's outer
         */
        auto get_outermost_owner() -> UObject*;

    private:
        /**
         * Returns the next FField object in the linked list of class
         * properties defined by the UClass::ChildrenProperties
         * Will throw the exception if the UE version is below 4.25
         */
        auto get_next_ffield_unsafe() -> FField*;

        /**
         * Converts this field to the underlying UField object
         * Conversion is only possible in UE versions below 4.25, so if this function
         * is called on newer UE versions it will throw exception
         */
        auto as_ufield_unsafe() -> class UField*;

        /**
         * Retrieves the underlying FFieldClass of this field if possible
         * Will throw the exception if UE version is below 4.25
         */
        auto get_ffieldclass_unsafe() -> FFieldClass*;

        /**
         * Returns the owner of the FField in the UE versions above 4.25
         * Will throw the exception if UE version is below 4.25
         */
        auto get_ffield_owner_unsafe() -> FFieldVariant;

        /**
        * Returns the name of the FField in the UE versions above 4.25
        * Will throw the exception if UE version is below 4.25
        */
        auto get_ffield_fname_unsafe() -> FName;
    };

    /**
     * A union representing a FField* object or an UObject*
     */
    class RC_UE_API FFieldVariant
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

    /**
     * Represents a FField class independently of the UE version
     * In the UE versions above 4.25 this would be a FFieldClass object,
     * but on earlier version it would be represented by the UClass
     *
     * Methods here allow working with FField classes in underlying type indepedent matter
     */
    class RC_UE_API FFieldClassVariant
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

        /**
         * Retrieves the class name, independently of the underlying type
         */
        auto get_fname() -> FName;

        /**
         * Retrieves the class name, independently of the underlying type
         */
        inline auto get_name() -> std::wstring
        {
            return get_fname().to_string();
        }

        /**
         * Retrieves the superclass of the class, independently of the underlying type
         */
        auto get_super_class() -> FFieldClassVariant;

        /**
         * Determines whenever the property class is a children of the provided class
         * This will always return false when used to compare uclasses and field classes
         */
        auto is_child_of(FFieldClassVariant uclass) -> bool;
    };

    /**
    * Represents a FField clas in the UE versions above 4.25
    * Has a structure similar to the UClass
    */
    class RC_UE_API FFieldClass
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

        auto get_super_class() -> FFieldClass*;

        auto is_child_of(FFieldClass* field_class) -> bool;

        inline auto get_name() -> std::wstring
        {
            return get_fname().to_string();
        }
    };
}

#endif //RC_UNREAL_FFIELD_HPP
