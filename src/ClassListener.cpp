#include <Unreal/ClassListener.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/Searcher/Searcher.hpp>
#include <Unreal/Searcher/ActorClassSearcher.hpp>

namespace RC::Unreal
{
    FClassCreateListener FClassCreateListener::ClassCreateListener{};
    FClassDeleteListener FClassDeleteListener::ClassDeleteListener{};

    void FClassCreateListener::NotifyUObjectCreated(const UObjectBase* Object, int32 Index)
    {
        auto* ObjectItem = Object->GetObjectItem();

        if (Object->IsA<UClass>())
        {
            Searcher<UClass>::Pool.emplace_back(ObjectItem);
            ClassSearcher<UClass>::Pool.emplace_back(ObjectItem);

            if (static_cast<const UClass*>(Object)->IsChildOf<AActor>())
            {
                ClassSearcher<AActor>::Pool.emplace_back(ObjectItem);
            }
        }
    }

    void FClassCreateListener::OnUObjectArrayShutdown()
    {
        UObjectArray::RemoveUObjectCreateListener(this);
    }

    void FClassDeleteListener::NotifyUObjectDeleted(const UObjectBase* Object, int32 Index)
    {
        auto* ObjectItem = Object->GetObjectItem();

        Searcher<UClass>::Pool.erase(std::remove_if(Searcher<UClass>::Pool.begin(), Searcher<UClass>::Pool.end(), [&](const auto& Item) {
            return Item == ObjectItem;
        }), Searcher<UClass>::Pool.end());

        ClassSearcher<UClass>::Pool.erase(std::remove_if(ClassSearcher<UClass>::Pool.begin(), ClassSearcher<UClass>::Pool.end(), [&](const auto& Item) {
            return Item == ObjectItem;
        }), ClassSearcher<UClass>::Pool.end());

        ClassSearcher<AActor>::Pool.erase(std::remove_if(ClassSearcher<AActor>::Pool.begin(), ClassSearcher<AActor>::Pool.end(), [&](const auto& Item) {
            return Item == ObjectItem;
        }), ClassSearcher<AActor>::Pool.end());
    }

    void FClassDeleteListener::OnUObjectArrayShutdown()
    {
        UObjectArray::RemoveUObjectDeleteListener(this);
    }

}
