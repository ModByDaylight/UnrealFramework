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

        // Very temporary functionsthat exists because I'm too lazy to implement a full FScriptArray right now
        void SetArrayNum(int32 NewArrayNum) { ArrayNum = NewArrayNum; }
        void SetArrayMax(int32 NewArrayMax) { ArrayMax = NewArrayMax; }
        void* GetAllocatorInstance() { return AllocatorInstance; }
        void SetAllocatorInstance(void* NewAllocatorInstance) { AllocatorInstance = NewAllocatorInstance; }
    };
}

#endif //RC_UNREAL_FSCRIPTARRAY_HPP
