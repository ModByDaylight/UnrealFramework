#ifndef RC_UNREAL_FSCRIPTARRAY_HPP
#define RC_UNREAL_FSCRIPTARRAY_HPP

namespace RC::Unreal
{
    // TODO: Make this more proper
    class FScriptArray
    {
    private:
        void* AllocatorInstance{};
        int32 ArrayNum{};
        int32 ArrayMax{};

    public:
        int32 GetArrayNum() { return ArrayNum; }
        int32 GetArrayMax() { return ArrayMax; }
    };
}

#endif //RC_UNREAL_FSCRIPTARRAY_HPP
