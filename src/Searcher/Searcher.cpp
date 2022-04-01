#include <Unreal/Searcher/Searcher.hpp>
#include <Unreal/AActor.hpp>
#include <DynamicOutput/DynamicOutput.hpp>

namespace RC::Unreal
{
    std::unordered_map<size_t, std::unique_ptr<SearcherBase>> AllInstanceSearchers;

    SearcherBase& FindSearcher(UClass* Class)
    {
        // Attempt to find a fast searcher.
        // We use the default slow searcher if a fast searcher doesn't exist.
        if (!Class)
        {
            return Searcher<DefaultSlowInstanceSearcher>::Get();
        }
        else if (auto It = AllInstanceSearchers.find(Class->HashObject()); It != AllInstanceSearchers.end())
        {
            return *It->second.get();
        }
        else
        {
            return Searcher<DefaultSlowInstanceSearcher>::Get();
        }
    }

    void ObjectSearcherSlow(UClass* Class, ObjectSearcherForEachPredicate Predicate)
    {
        UObjectGlobals::ForEachUObject([&](UObject* Object, [[maybe_unused]]int32_t ChunkIndex, [[maybe_unused]]int32_t ObjectIndex) {
            if (!Object || Object->IsUnreachable()) { return LoopAction::Continue; }
            // We need to narrow by class here because we're searching GUObjectArray which contains every single object in the game.
            if (Class && !Object->IsA(Class)) { return LoopAction::Continue; }
            return Predicate(Object);
        });
    }

    void ObjectSearcherFast(const ObjectSearcherForEachPredicate& Predicate, std::vector<const FUObjectItem*>& Pool)
    {
        for (const auto& Item : Pool)
        {
            if (!Item || Item->IsUnreachable()) { continue; }
            // We don't need to narrow by class here because the pool only contains objects of the right class..
            if (Predicate(Item->GetUObject()) == LoopAction::Break) { break; }
        }
    }
}
