#ifndef RC_UNREAL_SEARCHER_HPP
#define RC_UNREAL_SEARCHER_HPP

#include <functional>

#include <Unreal/UObjectArray.hpp>
#include <Unreal/UClass.hpp>

namespace RC::Unreal
{
    using ObjectSearcherForEachPredicate = std::function<LoopAction(UObject*)>;
    void ObjectSearcherSlow(UClass* Class, ObjectSearcherForEachPredicate Predicate);
    void ObjectSearcherFast(UClass* Class, const ObjectSearcherForEachPredicate& Predicate, std::vector<const FUObjectItem*>& Pool);

    struct DefaultSlowInstanceSearcher : public UClass
    {
        void StaticClass() {}
    };

    struct SearcherBase
    {
        virtual size_t PoolSize() = 0;
        virtual void ForEach(const ObjectSearcherForEachPredicate& Predicate) = 0;
        virtual void ForEach(UClass* Class, const ObjectSearcherForEachPredicate& Predicate) = 0;
    };

    // Slow generic searcher that iterates GUObjectArray
    template<typename T>
    struct Searcher : SearcherBase
    {
    public:
        static inline std::vector<const FUObjectItem*> Pool{};

        // Pointer to the default generic slow searcher.
        static inline std::unique_ptr<SearcherBase> UnderlyingSearcher;

    public:
        static auto& Get()
        {
            return *UnderlyingSearcher.get();
        }

        size_t PoolSize() override { return Pool.size(); };

        void ForEach(const ObjectSearcherForEachPredicate& Predicate) override
        {
            if constexpr (std::is_same_v<T, DefaultSlowInstanceSearcher>)
            {
                throw std::runtime_error{"Slow searcher cannot call 'ForEach' overload without Class parameter"};
            }
            else
            {
                ObjectSearcherSlow(T::StaticClass(), Predicate);
            }
        }

        void ForEach(UClass* Class, const ObjectSearcherForEachPredicate& Predicate) override
        {
            ObjectSearcherSlow(Class, Predicate);
        }
    };

    extern std::unordered_map<size_t, std::unique_ptr<SearcherBase>> AllInstanceSearchers;

    SearcherBase& FindSearcher(UClass* Class);

    template<typename Class>
    SearcherBase& FindSearcher()
    {
        return FindSearcher(Class::StaticClass());
    }
}

#endif // RC_UNREAL_SEARCHER_HPP
