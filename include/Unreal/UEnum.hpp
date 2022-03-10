#ifndef RC_UNREAL_UENUM_HPP
#define RC_UNREAL_UENUM_HPP

#include <format>

#include <Unreal/Common.hpp>
#include <Unreal/FField.hpp>
#include <Unreal/TypeChecker.hpp>
#include <Unreal/UField.hpp>
#include <Unreal/TArray.hpp>
#include <Unreal/TPair.hpp>

namespace RC::Unreal
{
    // The "UEnum::Names" pair is "TPair<FName, uint8>" in earlier engine versions
    // This can cause a problem, therefore, you should be careful when using this pair
    // This is a case where we perhaps should abstract this array and deny direct access to the TArray
    using FEnumNamePair = TPair<FName, int64_t>;
    using FEnumNameArray = TArray<FEnumNamePair>;

    class RC_UE_API UEnum : public UField
    {
        DECLARE_EXTERNAL_OBJECT_CLASS(UEnum, CoreUObject)
    public:
        enum class ECppForm
        {
            /**
             * Regular enums are declared using the normal enum keyword
             * Their constants are unscoped and usually prefixed with enum name
             */
            Regular,
            /**
             * Namespaced enums use the namespace for enum name + enum keyword for type
             * They are scoped to their namespace
             */
            Namespaced,
            /**
             * Enum classes are C++11 enum class enumerations that are properly scoped
             * They can also specify any integral underlying type
             */
            EnumClass
        };

        /** EEnumFlags, up-to-date with UE 5.0 EA */
        enum class EEnumFlags
        {
            None,
            /**
             * Enumeration represents a set of bitflags
             */
            Flags = 0x00000001,
            /**
             * There's a newer version of this UUserDefinedEnum available
             * in the engine and this one is obsolete
             */
            NewerVersionExists = 0x00000002,
        };
    public:
        /**
         * Returns the array of enumeration constants and their values
         * Keep in mind that enumeration can have gaps as well as negative values
         * The last entry of the array will always be a _MAX constant
         */
        auto get_enum_names() -> FEnumNameArray&;

        /**
         * Returns the C++ form using which the enum has been originally declared
         * See ECppForm for possible values
         */
        auto get_cpp_form() -> ECppForm;

        /**
         * Returns the flags set on the enumeration object
         * Will return EEnumFlags::None on older UE versions
         */
        auto get_enum_flags() -> EEnumFlags;

        /**
         * Retrieves the enumeration index associated with the provided enum name
         * Returns INDEX_NONE if there is no enumeration constant with the provided name
         */
        auto get_index_by_name(const std::wstring& name) -> int32_t;

        auto get_value_by_name(const std::wstring& name) -> int64_t;
    };

    class RC_UE_API UUserDefinedEnum : public UEnum
    {
        DECLARE_EXTERNAL_OBJECT_CLASS(UUserDefinedEnum, Engine);
    };
}


#endif //RC_UNREAL_UENUM_HPP
