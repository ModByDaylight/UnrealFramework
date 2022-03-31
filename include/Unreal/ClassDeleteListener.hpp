#ifndef RC_UNREAL_CLASS_DELETE_LISTENER_HPP
#define RC_UNREAL_CLASS_DELETE_LISTENER_HPP

#include <Unreal/UObjectArray.hpp>

namespace RC::Unreal
{
    struct RC_UE_API FClassDeleteListener : public FUObjectDeleteListener
    {
        static FClassDeleteListener SparseDelegateObjectListener;

        void NotifyUObjectDeleted(const class UObjectBase* Object, int32 Index) override;
        void OnUObjectArrayShutdown() override;
    };
}

#endif // RC_UNREAL_CLASS_DELETE_LISTENER_HPP