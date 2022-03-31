#include <Unreal/Searcher/ClassSearcher.hpp>
#include <Unreal/UObjectArray.hpp>
#include <Unreal/UClass.hpp>
#include <DynamicOutput/DynamicOutput.hpp>

namespace RC::Unreal
{
    std::unordered_map<size_t, std::unique_ptr<ClassSearcherBase>> AllClassSearchers;

    ClassSearcherBase& FindClassSearcher(UClass* Class)
    {
        // Attempt to find a fast searcher.
        // We use the default slow searcher if a fast searcher doesn't exist.
        if (auto It = AllClassSearchers.find(Class->HashObject()); It != AllClassSearchers.end())
        {
            return *It->second.get();
        }
        else
        {
            return ClassSearcher<DefaultSlowClassSearcher>::Get();
        }
    }

    void ClassSearcherFast(UClass* Class, const ClassSearcherForEachPredicate& Predicate, std::vector<const FUObjectItem*>& Pool)
    {
        for (const auto& Item : Pool)
        {
            if (!Item || Item->IsUnreachable()) { continue; }
            if (Class && (!Item->GetUObject()->IsA<UClass>() || !static_cast<UClass*>(Item->GetUObject())->IsChildOf(Class))) { continue; }
            if (Predicate(static_cast<UClass*>(Item->GetUObject())) == LoopAction::Break) { break; }
        }
    }
}
