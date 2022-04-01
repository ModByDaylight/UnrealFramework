#include <Unreal/ClassListener.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/Searcher/ObjectSearcher.hpp>
#include <Unreal/Searcher/ActorClassSearcher.hpp>

namespace RC::Unreal
{
    FClassCreateListener FClassCreateListener::ClassCreateListener{};
    FClassDeleteListener FClassDeleteListener::ClassDeleteListener{};

    void FClassCreateListener::NotifyUObjectCreated(const UObjectBase* Object, int32 Index)
    {
    }

    void FClassCreateListener::OnUObjectArrayShutdown()
    {
        UObjectArray::RemoveUObjectCreateListener(this);
    }

    void FClassDeleteListener::NotifyUObjectDeleted(const UObjectBase* Object, int32 Index)
    {
        auto* ObjectItem = Object->GetObjectItem();

        ClassSearcher<DefaultSlowClassSearcher>::Pool.erase(std::remove_if(ClassSearcher<DefaultSlowClassSearcher>::Pool.begin(), ClassSearcher<DefaultSlowClassSearcher>::Pool.end(), [&](const auto& Item) {
            return Item == ObjectItem;
        }), ClassSearcher<DefaultSlowClassSearcher>::Pool.end());

        ObjectSearcher<UClass>::Pool.erase(std::remove_if(ObjectSearcher<UClass>::Pool.begin(), ObjectSearcher<UClass>::Pool.end(), [&](const auto& Item) {
            return Item == ObjectItem;
        }), ObjectSearcher<UClass>::Pool.end());

        // TODO: Uncomment when we're able to maintain this particular pool.
        ClassSearcher<AActor>::Pool.erase(std::remove_if(ClassSearcher<AActor>::Pool.begin(), ClassSearcher<AActor>::Pool.end(), [&](const auto& Item) {
            return Item == ObjectItem;
        }), ClassSearcher<AActor>::Pool.end());

        ObjectSearcher<AActor>::Pool.erase(std::remove_if(ObjectSearcher<AActor>::Pool.begin(), ObjectSearcher<AActor>::Pool.end(), [&](const auto& Item) {
            return Item == ObjectItem;
        }), ObjectSearcher<AActor>::Pool.end());
    }

    void FClassDeleteListener::OnUObjectArrayShutdown()
    {
        UObjectArray::RemoveUObjectDeleteListener(this);
    }

}
