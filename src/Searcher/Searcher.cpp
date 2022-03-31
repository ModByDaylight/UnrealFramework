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
        if (auto It = AllInstanceSearchers.find(Class->HashObject()); It != AllInstanceSearchers.end())
        {
            Output::send(STR("Fast searcher found\n"));
            return *It->second.get();
        }
        else
        {
            Output::send(STR("No fast searcher found, using slow\n"));
            return Searcher<DefaultSlowInstanceSearcher>::Get();
        }
    }

    void ObjectSearcherSlow(UClass* Class, ObjectSearcherForEachPredicate Predicate)
    {
        UObjectGlobals::ForEachUObject([&](UObject* Object, [[maybe_unused]]int32_t ChunkIndex, [[maybe_unused]]int32_t ObjectIndex) {
            if (!Object || Object->IsUnreachable()) { return LoopAction::Continue; }
            if (Class && !Object->IsA(Class)) { return LoopAction::Continue; }
            return Predicate(Object);
        });
    }

    void ObjectSearcherFast(UClass* Class, const ObjectSearcherForEachPredicate& Predicate, std::vector<const FUObjectItem*>& Pool)
    {
        for (const auto& Item : Pool)
        {
            if (!Item || Item->IsUnreachable()) { continue; }
            if (!static_cast<UClass*>(Item->GetUObject())->IsChildOf(Class)) { continue; }
            if (Predicate(Item->GetUObject()) == LoopAction::Break) { break; }
        }
    }
}
