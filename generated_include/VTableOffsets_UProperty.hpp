struct RC_UE_API VTableOffsets
{
    static uint32_t GetCPPTypeForwardDeclaration;
    static uint32_t UProperty_Destructor;
    static uint32_t DestroyValueInternal;
    static uint32_t Serialize;
    static uint32_t LinkInternal;
    static uint32_t PassCPPArgsByRef;
    static uint32_t GetCPPMacroType;
    static uint32_t Identical;
    static uint32_t ContainsObjectReference;
    static uint32_t GetCPPType;
    static uint32_t InstanceSubobjects;
    static uint32_t SerializeItem;
    static uint32_t NetSerializeItem;
    static uint32_t ExportTextItem;
    static uint32_t ImportText_Internal;
    static uint32_t CopyValuesInternal;
    static uint32_t __vecDelDtor;
    static uint32_t GetValueTypeHashInternal;
    static uint32_t CopySingleValueToScriptVM;
    static uint32_t CopyCompleteValueToScriptVM;
    static uint32_t CopySingleValueFromScriptVM;
    static uint32_t CopyCompleteValueFromScriptVM;
    static uint32_t ClearValueInternal;
    static uint32_t InitializeValueInternal;
    static uint32_t GetID;
    static uint32_t GetMinAlignment;
    //static uint32_t ContainsWeakObjectReference;
    static uint32_t EmitReferenceInfo;
    static uint32_t SameType;
    static uint32_t ConvertFromType;
    static uint32_t IsPostLoadThreadSafe;
    static uint32_t SupportsNetSharedSerialization;
};
