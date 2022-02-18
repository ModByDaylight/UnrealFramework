#ifndef UE4SS_VIRTUALFUNCTIONHELPER_H
#define UE4SS_VIRTUALFUNCTIONHELPER_H

#include <unordered_map>
#include <stdexcept>
#include <type_traits>

/**
 * Used to store shared objects with the same identifier
 * Primary use case is bypassing the fact that templates are instantiated per-DLL,
 * so you cannot really use them to store static shared data
 */
class SharedObjectManager {
public:
    template<typename T>
    static inline T& get_shared_object(const char* unique_identifier)
    {
        auto* existing_value = reinterpret_cast<T*>(get_shared_object_internal(unique_identifier));
        if (existing_value != nullptr)
        {
            return *existing_value;
        }
        auto* new_value = new T();
        set_shared_object_internal(unique_identifier, new_value);
        return *new_value;
    }
private:
    static void* get_shared_object_internal(const char* id);
    static void set_shared_object_internal(const char* id, void* new_value);
};

/**
 * Allows accessing type of the object of the provided type
 * Will be used by the virtual call machinery to correctly dispatch virtual function calls
 *
 * The class should provide following properties:
 *  -typename ClassType - type of the object class
 *  -template<typename U> static auto static_class() -> ClassType - retrieves the class of the template object type U
 *  -static auto get_class_super_class(ClassType) -> ClassType - retrieves the superclass of the provided class
 *  -static auto register_late_bind_callback(void(*Callback)()) - registers the callback to be called after static_class() is safe to call
 *  -static auto is_class_valid(ClassType) -> bool - returns true if the provided class object is valid
 *
 *  The class extending it should be declared as the typedef named TypeAccessor of your object type
 */
class BaseTypeAccessor {
};

template<typename TCallable, TCallable Callable>
class VirtualFunctionInvoker;

template<typename ReceiverType, typename ReturnValueType, typename... ArgumentTypes, ReturnValueType(ReceiverType::*MemberFuncPtr)(ArgumentTypes...)>
class VirtualFunctionInvoker<ReturnValueType(ReceiverType::*)(ArgumentTypes...), MemberFuncPtr>
{
private:
    using DispatcherFuncType = ReturnValueType(*)(ReceiverType* self, ArgumentTypes... args);
    using TypeAccessor = typename ReceiverType::TypeAccessor;
    using ObjectClassType = typename TypeAccessor::ClassType;
    using DispatchMapType = std::unordered_map<ObjectClassType, DispatcherFuncType>;

    inline static auto get_dispatch_map() -> DispatchMapType&
    {
        return SharedObjectManager::get_shared_object<DispatchMapType>(__FUNCDNAME__);
    }
public:
    /** Registers the dispatch function associated with the provided class */
    inline static auto register_dispatch_func(ObjectClassType clazz, DispatcherFuncType dispatch_func)
    {
        get_dispatch_map().insert({clazz, dispatch_func});
    }

    /** Dispatches the virtual function call on the provided object */
    inline static auto dispatch_call(ReceiverType* receiver, ArgumentTypes... args) -> ReturnValueType
    {
        ObjectClassType object_class = TypeAccessor::get_object_class(receiver);
        DispatchMapType& dispatch_map = get_dispatch_map();

        DispatcherFuncType dispatcher_func = nullptr;
        while (TypeAccessor::is_class_valid(object_class))
        {
            const auto iterator = dispatch_map.template find<ObjectClassType>(object_class);
            if (iterator != dispatch_map.end())
            {
                dispatcher_func = iterator->second;
                break;
            }
            object_class = TypeAccessor::get_class_super_class(object_class);
        }

        if (dispatcher_func == nullptr)
        {
            throw std::runtime_error("Pure virtual function not implemented");
        }

        if constexpr(std::is_same_v<ReturnValueType, void>)
        {
            dispatcher_func(receiver, args...);
        }
        else
        {
            return dispatcher_func(receiver, args...);
        }
    }
};

template<typename TThunkCallable, typename TImplCallable, TThunkCallable ThunkCallable, TImplCallable ImplCallable>
class VirtualFunctionRegistrar;

template<typename BaseReceiverType, typename ImplReceiverType, typename ReturnValueType, typename... ArgumentTypes,
        ReturnValueType(BaseReceiverType::*BaseFunctionPtr)(ArgumentTypes...),
        ReturnValueType(ImplReceiverType::*ImplFunctionPtr)(ArgumentTypes...)>
class VirtualFunctionRegistrar<ReturnValueType(BaseReceiverType::*)(ArgumentTypes...), ReturnValueType(ImplReceiverType::*)(ArgumentTypes...), BaseFunctionPtr, ImplFunctionPtr> {
private:
    using InvokerType = VirtualFunctionInvoker<ReturnValueType(BaseReceiverType::*)(ArgumentTypes...), BaseFunctionPtr>;
    using TypeAccessor = typename BaseReceiverType::TypeAccessor;
public:
    /** Registers the dispatch function inside of the invoker */
    inline VirtualFunctionRegistrar()
    {
        TypeAccessor::register_late_bind_callback(&bind_virtual_function);
    }
private:
    /** Performs the actual binding of the virtual function to the static class */
    static auto bind_virtual_function() -> void
    {
        InvokerType::register_dispatch_func(TypeAccessor::template static_class<ImplReceiverType>(), &dispatch_call);
    }

    /** Dispatches the call to the member function implementation in the impl class */
    static auto dispatch_call(BaseReceiverType* self, ArgumentTypes... args) -> ReturnValueType
    {
        if constexpr(std::is_same_v<ReturnValueType, void>)
        {
            (*static_cast<ImplReceiverType*>(self).*ImplFunctionPtr)(args...);
        }
        else
        {
            return (*static_cast<ImplReceiverType*>(self).*ImplFunctionPtr)(args...);
        }
    }
};

/** Joins 2 preprocessor tokens together into a single token */
#define PREPROCESSOR_JOIN(First, Second) First##Second
#define PREPROCESSOR_JOIN3(First, Second, Third) First##Second##Third

/** Defines declarations needed for overriding virtual functions */
#define DECLARE_VIRTUAL_TYPE(ClassName) \
    template<typename InClassName, typename TFunctionType, TFunctionType Function> \
    friend class VirtualFunctionRegistrarHolder;     \
    template<typename TCallable, TCallable Callable> \
    friend class VirtualFunctionInvoker;             \
    template<typename TThunkCallable, typename TImplCallable, TThunkCallable ThunkCallable, TImplCallable ImplCallable> \
    friend class VirtualFunctionRegistrar;           \
    using TypeAccessor = VirtualTypeBaseClass::TypeAccessor; \

/** Defines the basic data needed for the root virtual class */
#define DECLARE_VIRTUAL_TYPE_BASE(ClassName, TypeAccessorClass) \
    using TypeAccessor = TypeAccessorClass;          \
    using VirtualTypeBaseClass = ClassName;          \
    DECLARE_VIRTUAL_TYPE(ClassName)                   \
protected:

/** Registers the implementation override for the provided function */
#define IMPLEMENT_VIRTUAL_FUNCTION(ClassName, FunctionName) \
template<typename ClassName, typename TFunctionType, TFunctionType Function> \
class VirtualFunctionRegistrarHolder;                       \
template<>                                                  \
class VirtualFunctionRegistrarHolder<ClassName, decltype(&ClassName::FunctionName), &ClassName::FunctionName> \
{                                                           \
public:                                                     \
   using InnerType = VirtualFunctionRegistrar<decltype(&ClassName::FunctionName), decltype(&ClassName::PREPROCESSOR_JOIN(FunctionName, _Impl)), &ClassName::FunctionName, &ClassName::PREPROCESSOR_JOIN(FunctionName, _Impl)>; \
};                                                          \
static VirtualFunctionRegistrarHolder<ClassName, decltype(&ClassName::FunctionName), &ClassName::FunctionName>::InnerType PREPROCESSOR_JOIN3(__virtual_function_registrar_, ClassName, FunctionName); \

/** Performs a call to the virtual function with the provided name */
#define CALL_VIRTUAL_FUNCTION(Self, FunctionName, ...) \
    VirtualFunctionInvoker<decltype(&std::remove_reference_t<decltype(*Self)>::FunctionName), &std::remove_reference_t<decltype(*Self)>::FunctionName>::dispatch_call(Self, __VA_ARGS__)

#endif //UE4SS_VIRTUALFUNCTIONHELPER_H
