#ifndef RC_UNREAL_FFIELD_HPP
#define RC_UNREAL_FFIELD_HPP

#include <Constructs/Loop.hpp>
#include <Unreal/Common.hpp>
#include <Unreal/NameTypes.hpp>
#include <Unreal/TArray.hpp>
#include <Unreal/UnrealVersion.hpp>
#include <Unreal/VirtualFunctionHelper.h>

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

    namespace Internal {
        class FFieldTypeAccessor;
    }

    /**
     * Represents an FField object on the UE versions above UE4.25
     * On the earlier versions it will be falling back to the UField behavior, allowing the version
     * independent data to the FField data, which is primarily used for working with FProperties
     *
     * This class CANNOT be used to represent UField* objects on UE 4.25+, use UField instead!!
     */
    class RC_UE_API FField
    {
        DECLARE_FIELD_CLASS(FField);
        DECLARE_VIRTUAL_TYPE_BASE(FField, Internal::FFieldTypeAccessor);

    public:
#include <VTableOffsets_FField.hpp>

    public:
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
        inline auto is_a() -> bool
        {
            return is_a(FFieldDerivedType::static_class());
        }

        /**
         * Returns the owner of this field
         * In versions below 4.25, it would always represent the object
         */
        auto get_owner_variant() -> FFieldVariant;

        /**
         * Returns the first UObject owner of this field of the provided type
         */
        auto get_typed_owner(UClass* owner_type) -> UObject*;

        /**
         * Templated version of the get_typed_owner, will also cast the
         * returned object pointer automatically
         */
        template<UObjectDerivative T>
        inline auto get_typed_owner() -> T*
        {
            return cast_object<T>(get_typed_owner(T::static_class()));
        }
    private:
        friend class UStruct;

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

        using FArchive = void*; // Remove if/when we have an FArchive implementation, for now, probably a bad idea to call
        auto serialize(FArchive& ar) -> void;

        auto post_load() -> void;

        auto get_preload_dependencies(TArray<UObject*>& out_deps) -> void;

        auto begin_destroy() -> void;

        using FReferenceCollector = void*; // Remove if/when we have an FArchive implementation, for now, probably a bad idea to call
        auto add_referenced_objects(FReferenceCollector& collector) -> void;

        auto add_cpp_property(class FProperty* property) -> void;

        auto bind() -> void;

        auto post_duplicate(const FField& in_field) -> void;

        auto get_inner_field_by_name(const FName& in_name) -> FField*;

        auto get_inner_fields(TArray<FField*>& out_fields) -> void;
    };

    /**
     * A union representing a FField object or an UObject
     * Used primarily to represent a FField object hierarchy, since FField owner can represent both
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
     * Methods here allow working with FField classes in underlying type independent matter
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
        /**
         * Constructs the FFieldClassVariant from the provided FFieldClass object
         * @param field a valid FFieldClass object
         */
        FFieldClassVariant(FFieldClass* field);

        /**
         * Constructs the FFieldClassVariant from the provided UClass object
         * @param object a valid UClass object
         */
        FFieldClassVariant(UClass* object);

        /**
         * Constructs an invalid instance of the FFieldClassVariant representing a nullptr
         * The resulting instance cannot be de-referenced into neither of the types
         */
        FFieldClassVariant();
    public:
        /**
         * Checks whenever this variant represents a valid class and not a nullptr
         * @return true if this is a valid class
         */
        auto is_valid() const -> bool;

        /**
         * Retrieves the class name, independently of the underlying type
         */
        auto get_fname() const -> FName;

        /**
         * Retrieves the superclass of the class, independently of the underlying type
         */
        auto get_super_class() const -> FFieldClassVariant;

        /**
         * Determines whenever the property class is a children of the provided class
         * This will always return false when used to compare uclasses and field classes
         */
        auto is_child_of(FFieldClassVariant uclass) const -> bool;

        /**
         * Retrieves the class name, independently of the underlying type
         */
        inline auto get_name() const -> std::wstring
        {
            return get_fname().to_string();
        }

        /** Equality operator for FFieldClassVariant, they are equal if they both point to the same class */
        auto operator==(const RC::Unreal::FFieldClassVariant& rhs) const -> bool;

        /** Computes the hash of the object */
        auto hash_object() const -> size_t;
    private:
        friend class UStruct;
        friend class FField;

        auto is_uclass() const -> bool;
        auto is_field_class() const -> bool;

        auto to_field_class() const -> FFieldClass*;
        auto to_uclass() const -> UClass*;
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
        auto get_fname() const -> FName;

        auto get_super_class() const -> FFieldClass*;

        auto is_child_of(FFieldClass* field_class) const -> bool;

        inline auto get_name() const -> std::wstring
        {
            return get_fname().to_string();
        }
    };

    /** Casts the field to the specified type after performing the type checking */
    template<FFieldDerivative T>
    inline auto cast_field(FField* field) -> T* {
        return field != nullptr && field->is_a<T>() ? static_cast<T*>(field) : nullptr;
    }

    namespace Internal
    {
        /** Virtual function type accessor for FField objects */
        class FFieldTypeAccessor : public BaseTypeAccessor
        {
        private:
            static std::vector<void(*)()> late_bind_callbacks;
            static bool type_system_initialized;
        public:
            using ClassType = FFieldClassVariant;

            template<typename T>
            inline static auto static_class() -> FFieldClassVariant
            {
                return T::static_class();
            }
            static auto get_object_class(FField* field) -> FFieldClassVariant;
            static auto get_class_super_class(FFieldClassVariant field_class) -> FFieldClassVariant;
            static auto is_class_valid(FFieldClassVariant field_class) -> bool;
            static auto register_late_bind_callback(void(*callback)()) -> void;
            static auto on_type_system_initialized() -> void;
        };
    }
}

template<>
struct std::hash<RC::Unreal::FFieldClassVariant>
{
    std::size_t operator()(const RC::Unreal::FFieldClassVariant& s) const noexcept
    {
        return s.hash_object();
    }
};

#endif //RC_UNREAL_FFIELD_HPP
