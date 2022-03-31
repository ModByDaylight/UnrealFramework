#ifndef RC_UNREAL_CLASS_SEARCHER_HPP
#define RC_UNREAL_CLASS_SEARCHER_HPP

#include <functional>
#include <memory>
#include <vector>

#include <Constructs/Loop.hpp>

namespace RC::Unreal
{
    class UClass;
    struct FUObjectItem;

    using ClassSearcherForEachPredicate = std::function<LoopAction(UClass*)>;
    void ClassSearcherFast(UClass* Class, const ClassSearcherForEachPredicate& Predicate, std::vector<const FUObjectItem*>& Pool);

    using DefaultSlowClassSearcher = void;

    struct ClassSearcherBase
    {
        virtual size_t PoolSize() = 0;
        virtual void ForEach(const ClassSearcherForEachPredicate& Predicate) = 0;
        virtual void ForEach(UClass* Class, const ClassSearcherForEachPredicate& Predicate) = 0;
    };

    // Slow searcher that only iterates classes
    template<typename T>
    struct ClassSearcher : ClassSearcherBase
    {
    public:
        static inline std::vector<const FUObjectItem*> Pool{};

        // Pointer to the default generic slow searcher.
        static inline std::unique_ptr<ClassSearcherBase> UnderlyingSearcher;

    public:
        static auto& Get()
        {
            return *UnderlyingSearcher.get();
        }

        size_t PoolSize() override { return Pool.size(); };

        void ForEach(const ClassSearcherForEachPredicate& Predicate) override
        {
            ClassSearcherFast(UClass::StaticClass(), Predicate, Pool);
        }

        void ForEach(UClass* Class, const ClassSearcherForEachPredicate& Predicate) override
        {
            ClassSearcherFast(Class, Predicate, Pool);
        }
    };

    extern std::unordered_map<size_t, std::unique_ptr<ClassSearcherBase>> AllClassSearchers;

    ClassSearcherBase& FindClassSearcher(UClass* Class);

    template<typename Class>
    ClassSearcherBase& FindClassSearcher()
    {
        return FindClassSearcher(Class::StaticClass());
    }
}

#endif //RC_UNREAL_CLASS_SEARCHER_HPP
