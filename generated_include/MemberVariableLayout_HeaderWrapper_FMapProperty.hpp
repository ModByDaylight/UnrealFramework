static std::unordered_map<std::wstring, int32_t> MemberOffsets;

public:
    FProperty* GetKeyProp();
    const FProperty* GetKeyProp() const;

public:
    FProperty* GetValueProp();
    const FProperty* GetValueProp() const;

