#ifndef UE4SS_UOBJECTMACROS_H
#define UE4SS_UOBJECTMACROS_H

#pragma clang diagnostic ignored "-Wstring-plus-int"

namespace RC::Unreal {
    class UClass;
    class FFieldClassVariant;

    using ERenameFlags = uint32;
}

/** Default rename behavior */
#define REN_None				(0x0000)
/** Rename won't call ResetLoaders or flush async loading. You should pass this if you are renaming a deep subobject and do not need to reset loading for the outer package */
#define REN_ForceNoResetLoaders		(0x0001)
/** Just test to make sure that the rename is guaranteed to succeed if an non test rename immediately follows */
#define REN_Test					(0x0002)
/** Indicates that the object (and new outer) should not be dirtied */
#define REN_DoNotDirty				(0x0004)
/** Don't create an object redirector, even if the class is marked RF_Public */
#define REN_DontCreateRedirectors	(0x0010)
/** Don't call Modify() on the objects, so they won't be stored in the transaction buffer */
#define REN_NonTransactional		(0x0020)
/** Force unique names across all packages not just within the scope of the new outer */
#define REN_ForceGlobalUnique		(0x0040)
/** Prevent renaming of any child generated classes and CDO's in blueprints */
#define REN_SkipGeneratedClasses	(0x0080)


#define DECLARE_EXTERNAL_OBJECT_CLASS(ClassName, ModuleName) \
private: \
    static RC::Unreal::UClass* m_static_class; \
    template<typename T>\
    friend class RC::Unreal::Internal::UClassRegistrarTemplate; \
\
    inline static RC::Unreal::UClass** static_class_ptr() { return &ClassName::m_static_class; } \
    inline static const wchar_t* static_package() { return STR("/Script/") STR(#ModuleName); }   \
    inline static const wchar_t* static_class_name() { return (STR(#ClassName) + 1); } \
public: \
    auto static static_class() -> RC::Unreal::UClass*; \

#define IMPLEMENT_EXTERNAL_OBJECT_CLASS(ClassName) \
RC::Unreal::UClass* ClassName::m_static_class = nullptr; \
static RC::Unreal::Internal::UClassRegistrarTemplate<ClassName> __class_registrar_##ClassName;\
\
auto ClassName::static_class() -> RC::Unreal::UClass* \
{ \
    if (!m_static_class) \
    { \
        throw std::runtime_error{"[" #ClassName "::static_class] m_static_class is nullptr"}; \
    } \
    return m_static_class; \
};

#define DECLARE_FIELD_CLASS(ClassName) \
private: \
    static RC::Unreal::FFieldClassVariant m_static_class; \
    template<typename T>\
    friend class RC::Unreal::Internal::FFieldClassRegistrarTemplate; \
\
    inline static RC::Unreal::FFieldClassVariant* static_class_ptr() { return &ClassName::m_static_class; } \
    inline static const wchar_t* static_class_name() { return (STR(#ClassName) + 1); } \
public: \
    auto static static_class() -> RC::Unreal::FFieldClassVariant;    \

#define IMPLEMENT_FIELD_CLASS(ClassName) \
RC::Unreal::FFieldClassVariant ClassName::m_static_class{(RC::Unreal::FFieldClass*) nullptr}; \
static RC::Unreal::Internal::FFieldClassRegistrarTemplate<ClassName> __field_class_registrar_##ClassName;\
\
auto ClassName::static_class() -> RC::Unreal::FFieldClassVariant \
{ \
    if (!m_static_class.is_valid()) \
    { \
        throw std::runtime_error{"[" #ClassName "::static_class] m_static_class is not valid"}; \
    } \
    return m_static_class; \
};

namespace RC::Unreal::Internal
{
    class RC_UE_API UClassRegistrar
    {
    protected:
        UClassRegistrar(RC::Unreal::UClass** out_static_class, const wchar_t* class_name, const wchar_t* package_name);
    };

    class RC_UE_API FFieldClassRegistrar
    {
    protected:
        FFieldClassRegistrar(RC::Unreal::FFieldClassVariant* out_static_class, const wchar_t* class_name);
    };

    template<typename T>
    class UClassRegistrarTemplate : public UClassRegistrar
    {
    public:
        inline UClassRegistrarTemplate() : UClassRegistrar(T::static_class_ptr(), T::static_class_name(), T::static_package()) {
        }
    };

    template<typename T>
    class FFieldClassRegistrarTemplate : public FFieldClassRegistrar
    {
    public:
        inline FFieldClassRegistrarTemplate() : FFieldClassRegistrar(T::static_class_ptr(), T::static_class_name()) {
        }
    };
}

#endif //UE4SS_UOBJECTMACROS_H
