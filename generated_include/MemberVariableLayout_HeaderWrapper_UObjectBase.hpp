static std::unordered_map<std::wstring, int32_t> MemberOffsets;

public:
    UClass* GetClassPrivate();
    const UClass* GetClassPrivate() const;

public:
    int32 GetInternalIndex();
    const int32 GetInternalIndex() const;

public:
    FName GetNamePrivate();
    const FName GetNamePrivate() const;

public:
    EObjectFlags GetObjectFlags();
    const EObjectFlags GetObjectFlags() const;

public:
    UObject* GetOuterPrivate();
    const UObject* GetOuterPrivate() const;

