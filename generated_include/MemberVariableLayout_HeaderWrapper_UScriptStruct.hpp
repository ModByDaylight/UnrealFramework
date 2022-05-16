static std::unordered_map<std::wstring, int32_t> MemberOffsets;

public:
UScriptStruct::ICppStructOps*& GetCppStructOps();
const UScriptStruct::ICppStructOps*& GetCppStructOps() const;

public:
EStructFlags& GetStructFlags();
const EStructFlags& GetStructFlags() const;

public:
bool& GetbCppStructOpsFromBaseClass();
const bool& GetbCppStructOpsFromBaseClass() const;

public:
bool& GetbPrepareCppStructOpsCompleted();
const bool& GetbPrepareCppStructOpsCompleted() const;
