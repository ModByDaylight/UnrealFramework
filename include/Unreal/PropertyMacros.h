#ifndef UE4SS_PROPERTYMACROS_H
#define UE4SS_PROPERTYMACROS_H

#include <string>
#include "Unreal/Common.hpp"
#include "UObjectMacros.h"

#define DECLARE_PROPERTY_CLASS(ClassName) \
    DECLARE_FIELD_CLASS(ClassName);       \
private:                                  \
    auto static initialize_value_dispatcher(FProperty* self, void* dest) -> void; \
    auto static destroy_value_dispatcher(FProperty* self, void* value) -> void;   \
    auto static clear_value_dispatcher(FProperty* self, void* value) -> void;     \
    auto static copy_value_dispatcher(FProperty* self, void* dest, const void* src) -> void; \
    auto static identical_dispatcher(FProperty* self, const void* A, const void* B, int32_t compare_flags) -> bool; \
    auto static export_text_dispatcher(FProperty* self, std::wstring& value_string, const void* property_value, const void* default_value, UObject* parent, int32_t port_flags) -> void; \
    auto static import_text_dispatcher(FProperty* self, const wchar_t* buffer, void* data, int32_t port_flags, UObject* owner_object) -> const wchar_t*;                                 \
    auto static get_property_value_int64_dispatcher(FProperty* self, void* value) -> int64_t; \
    auto static get_property_value_double_dispatcher(FProperty* self, void* value) -> double; \
    auto static is_floating_point_dispatcher(FProperty* self) -> bool; \
    auto static get_property_value_object_dispatcher(FProperty* self, void* value) -> UObject*; \
    template<typename T>                  \
    friend class TPropertyDispatchTableRegistrar;\

#define IMPLEMENT_PROPERTY_CLASS(ClassName) \
    IMPLEMENT_FIELD_CLASS(ClassName);       \
    auto ClassName::initialize_value_dispatcher(FProperty* self, void* dest) -> void { RC::Unreal::cast_field<ClassName>(self)->initialize_value_implementation(dest); } \
    auto ClassName::destroy_value_dispatcher(FProperty* self, void* value) -> void { RC::Unreal::cast_field<ClassName>(self)->destroy_value_implementation(value); } \
    auto ClassName::clear_value_dispatcher(FProperty* self, void* value) -> void { RC::Unreal::cast_field<ClassName>(self)->clear_value_implementation(value); } \
    auto ClassName::copy_value_dispatcher(FProperty* self, void* dest, const void* src) -> void { RC::Unreal::cast_field<ClassName>(self)->copy_value_implementation(dest, src); } \
    auto ClassName::identical_dispatcher(FProperty* self, const void* A, const void* B, int32_t compare_flags) -> bool { return RC::Unreal::cast_field<ClassName>(self)->identical_implementation(A, B, compare_flags); } \
    auto ClassName::export_text_dispatcher(FProperty* self, std::wstring& value_string, const void* property_value, const void* default_value, UObject* parent, int32_t port_flags) -> void { RC::Unreal::cast_field<ClassName>(self)->export_text_implementation(value_string, property_value, default_value, parent, port_flags); } \
    auto ClassName::import_text_dispatcher(FProperty* self, const wchar_t* buffer, void* data, int32_t port_flags, UObject* owner_object) -> const wchar_t* { return RC::Unreal::cast_field<ClassName>(self)->import_text_implementation(buffer, data, port_flags, owner_object); };                                                  \
    auto ClassName::get_property_value_int64_dispatcher(FProperty* self, void* value) -> int64_t { return RC::Unreal::cast_field<ClassName>(self)->get_property_value_int64_implementation(value); } \
    auto ClassName::get_property_value_double_dispatcher(FProperty* self, void* value) -> double { return RC::Unreal::cast_field<ClassName>(self)->get_property_value_double_implementation(value); } \
    auto ClassName::is_floating_point_dispatcher(FProperty* self) -> bool { return RC::Unreal::cast_field<ClassName>(self)->is_floating_point_implementation(); } \
    auto ClassName::get_property_value_object_dispatcher(FProperty* self, void* value) -> UObject* { return RC::Unreal::cast_field<ClassName>(self)->get_property_value_object_implementation(value); }  \
    static RC::Unreal::Internal::TPropertyDispatchTableRegistrar<ClassName> __property_dispatch_table_registrar_##ClassName;\

namespace RC::Unreal
{
    class FProperty;
    class UObject;
}

namespace RC::Unreal::Internal
{
    namespace FPropertyDispatchers
    {
        using InitializeValue = void (*)(FProperty *property, void *dest);
        using DestroyValue = void (*)(FProperty *property, void *value);
        using CopyValue = void (*)(FProperty *property, void *dest, const void *src);
        using ClearValue = void (*)(FProperty *property, void *value);
        using Identical = bool (*)(FProperty *property, const void *A, const void *B, int32_t compare_flags);
        using ExportText = void (*)(FProperty *property, std::wstring &value_string, const void *property_value, const void *default_value, UObject *parent, int32_t port_flags);
        using ImportText = const wchar_t *(*)(FProperty *property, const wchar_t *buffer, void *data, int32_t port_flags, UObject *owner_object);
        using GetPropertyValueInt64 = int64_t(*)(FProperty* property, void* value);
        using GetPropertyValueDouble = double(*)(FProperty* property, void* value);
        using IsFloatingPoint = bool(*)(FProperty* property);
        using GetPropertyValueObject = UObject*(*)(FProperty* property, void* value);
    }

    struct RC_UE_API PropertyDispatchTable
    {
        FPropertyDispatchers::InitializeValue initialize_value;
        FPropertyDispatchers::DestroyValue destroy_value;
        FPropertyDispatchers::CopyValue copy_value;
        FPropertyDispatchers::ClearValue clear_value;
        FPropertyDispatchers::Identical identical;
        FPropertyDispatchers::ExportText export_text;
        FPropertyDispatchers::ImportText import_text;
        FPropertyDispatchers::GetPropertyValueInt64 get_property_value_int64;
        FPropertyDispatchers::GetPropertyValueDouble get_property_value_double;
        FPropertyDispatchers::IsFloatingPoint is_floating_point;
        FPropertyDispatchers::GetPropertyValueObject get_property_value_object;
    };

    class RC_UE_API FPropertyDispatchTableRegistrar
    {
    protected:
        FPropertyDispatchTableRegistrar(FFieldClassVariant(*field_class)(), const PropertyDispatchTable& dispatch_table);
    };

    template<typename T>
    class TPropertyDispatchTableRegistrar : public FPropertyDispatchTableRegistrar
    {
    public:
        inline TPropertyDispatchTableRegistrar() : FPropertyDispatchTableRegistrar(&T::static_class, PropertyDispatchTable{
            .initialize_value = &T::initialize_value_dispatcher,
            .destroy_value = &T::destroy_value_dispatcher,
            .clear_value = &T::clear_value_dispatcher,
            .identical = &T::identical_dispatcher,
            .export_text = &T::export_text_dispatcher,
            .import_text = &T::import_text_dispatcher,
            .get_property_value_int64 = &T::get_property_value_int64_dispatcher,
            .get_property_value_double = &T::get_property_value_double_dispatcher,
            .is_floating_point = &T::is_floating_point_dispatcher,
            .get_property_value_object = &T::get_property_value_object_dispatcher
        }) {
        }
    };
}

#endif //UE4SS_PROPERTYMACROS_H
