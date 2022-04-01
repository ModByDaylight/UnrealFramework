#ifndef RC_UNREAL_ACTOR_CLASS_SEARCHER_HPP
#define RC_UNREAL_ACTOR_CLASS_SEARCHER_HPP

#include <Unreal/Searcher/ClassSearcher.hpp>
#include <Unreal/AActor.hpp>

namespace RC::Unreal
{
    // Fast searcher that only iterates AActor classes
    // Currently not working because we have no way of maintaining the pool after initial GUObjectArray iteration.
    //template<>
    //struct ClassSearcher<AActor> : public ClassSearcher<UClass>
    //{
    //public:
    //    static inline std::vector<const FUObjectItem*> Pool{};
//
    //public:
    //    size_t PoolSize() override { return Pool.size(); };
//
    //    void ForEach(const ClassSearcherForEachPredicate& Predicate) override
    //    {
    //        Output::send(STR("ActorClassSearcher::ForEach without class (PoolSize: {})\n"), Pool.size());
    //        ClassSearcherFast(AActor::StaticClass(), Predicate, Pool);
    //    }
    //};
}

#endif // RC_UNREAL_ACTOR_CLASS_SEARCHER_HPP