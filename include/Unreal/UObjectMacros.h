#ifndef UE4SS_UOBJECTMACROS_H
#define UE4SS_UOBJECTMACROS_H

#pragma clang diagnostic ignored "-Wstring-plus-int"

namespace RC::Unreal {
    class UClass;
    class FFieldClassVariant;
}

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
    auto static static_class() -> RC::Unreal::FFieldClassVariant; \

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
