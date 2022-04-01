#ifndef RC_UNREAL_SEARCHER_HPP
#define RC_UNREAL_SEARCHER_HPP

#include <functional>

#include <Unreal/UObjectArray.hpp>
#include <Unreal/UClass.hpp>
#include <DynamicOutput/DynamicOutput.hpp>

namespace RC::Unreal
{
    using ObjectSearcherForEachPredicate = std::function<LoopAction(UObject*)>;
    void ObjectSearcherSlow(UClass* Class, ObjectSearcherForEachPredicate Predicate);
    void ObjectSearcherFast(const ObjectSearcherForEachPredicate& Predicate, std::vector<const FUObjectItem*>& Pool);

    struct DefaultSlowInstanceSearcher : public UClass
    {
        static UClass* StaticClass() { return nullptr; }
    };

    struct SearcherBase
    {
        virtual size_t PoolSize() = 0;
        virtual bool IsFast() = 0;

        // Using 'T::StaticClass' to narrow.
        virtual void ForEach(const ObjectSearcherForEachPredicate& Predicate) = 0;

        // Using 'Class' parameter to narrow.
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
        bool IsFast() { return !Pool.empty(); };

        void ForEach(const ObjectSearcherForEachPredicate& Predicate) override
        {
            // If we don't have a pool, then we have to do a full GUObjectArray search.
            // Otherwise, we can do a smaller search just through the pool.
            if (Pool.empty())
            {
                // If T == DefaultSlowSearcher, StaticClass() will return nullptr.
                // We will then search every object, not limited by class.
                ObjectSearcherSlow(T::StaticClass(), Predicate);
            }
            else
            {
                // A pool is already narrowed by class, so we don't need to supply a class here.
                ObjectSearcherFast(Predicate, Pool);
            }
        }

        void ForEach(UClass* Class, const ObjectSearcherForEachPredicate& Predicate) override
        {
            // If we don't have a pool, then we have to do a full GUObjectArray search.
            // Otherwise, we can do a smaller search just through the pool.
            if (Pool.empty())
            {
                ObjectSearcherSlow(Class, Predicate);
            }
            else
            {
                // We're using the pool, but this overload requires that we still pass the class to the underlying searcher.
                ObjectSearcherFast(Predicate, Pool);
            }
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
