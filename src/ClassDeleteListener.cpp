#include <Unreal/ClassDeleteListener.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/UClass.hpp>
#include <DynamicOutput/DynamicOutput.hpp>

namespace RC::Unreal
{
    FClassDeleteListener FClassDeleteListener::SparseDelegateObjectListener{};

    void FClassDeleteListener::NotifyUObjectDeleted(const UObjectBase* Object, int32 Index)
    {
    }

    void FClassDeleteListener::OnUObjectArrayShutdown()
    {
        UObjectArray::RemoveUObjectDeleteListener(this);
    }

}
