std::unordered_map<std::wstring, int32_t> UStruct::MemberOffsets{};

FField* UStruct::GetChildProperties()
{
    static auto offset = MemberOffsets.find(STR("ChildProperties"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::ChildProperties' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<FField*>(this, offset->second);
}
const FField* UStruct::GetChildProperties() const
{
    static auto offset = MemberOffsets.find(STR("ChildProperties"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::ChildProperties' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<const FField*>(this, offset->second);
}

UField* UStruct::GetChildren()
{
    static auto offset = MemberOffsets.find(STR("Children"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::Children' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<UField*>(this, offset->second);
}
const UField* UStruct::GetChildren() const
{
    static auto offset = MemberOffsets.find(STR("Children"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::Children' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<const UField*>(this, offset->second);
}

FProperty* UStruct::GetDestructorLink()
{
    static auto offset = MemberOffsets.find(STR("DestructorLink"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::DestructorLink' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<FProperty*>(this, offset->second);
}
const FProperty* UStruct::GetDestructorLink() const
{
    static auto offset = MemberOffsets.find(STR("DestructorLink"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::DestructorLink' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<const FProperty*>(this, offset->second);
}

int32 UStruct::GetMinAlignment()
{
    static auto offset = MemberOffsets.find(STR("MinAlignment"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::MinAlignment' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<int32>(this, offset->second);
}
const int32 UStruct::GetMinAlignment() const
{
    static auto offset = MemberOffsets.find(STR("MinAlignment"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::MinAlignment' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<const int32>(this, offset->second);
}

FProperty* UStruct::GetPostConstructLink()
{
    static auto offset = MemberOffsets.find(STR("PostConstructLink"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::PostConstructLink' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<FProperty*>(this, offset->second);
}
const FProperty* UStruct::GetPostConstructLink() const
{
    static auto offset = MemberOffsets.find(STR("PostConstructLink"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::PostConstructLink' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<const FProperty*>(this, offset->second);
}

int32 UStruct::GetPropertiesSize()
{
    static auto offset = MemberOffsets.find(STR("PropertiesSize"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::PropertiesSize' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<int32>(this, offset->second);
}
const int32 UStruct::GetPropertiesSize() const
{
    static auto offset = MemberOffsets.find(STR("PropertiesSize"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::PropertiesSize' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<const int32>(this, offset->second);
}

FProperty* UStruct::GetPropertyLink()
{
    static auto offset = MemberOffsets.find(STR("PropertyLink"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::PropertyLink' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<FProperty*>(this, offset->second);
}
const FProperty* UStruct::GetPropertyLink() const
{
    static auto offset = MemberOffsets.find(STR("PropertyLink"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::PropertyLink' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<const FProperty*>(this, offset->second);
}

FProperty* UStruct::GetRefLink()
{
    static auto offset = MemberOffsets.find(STR("RefLink"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::RefLink' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<FProperty*>(this, offset->second);
}
const FProperty* UStruct::GetRefLink() const
{
    static auto offset = MemberOffsets.find(STR("RefLink"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::RefLink' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<const FProperty*>(this, offset->second);
}

TArray<unsigned char,FDefaultAllocator> UStruct::GetScript()
{
    static auto offset = MemberOffsets.find(STR("Script"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::Script' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<TArray<unsigned char,FDefaultAllocator>>(this, offset->second);
}
const TArray<unsigned char,FDefaultAllocator> UStruct::GetScript() const
{
    static auto offset = MemberOffsets.find(STR("Script"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::Script' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<const TArray<unsigned char,FDefaultAllocator>>(this, offset->second);
}

TArray<UObject *,TSizedDefaultAllocator<32> > UStruct::GetScriptAndPropertyObjectReferences()
{
    static auto offset = MemberOffsets.find(STR("ScriptAndPropertyObjectReferences"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::ScriptAndPropertyObjectReferences' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<TArray<UObject *,TSizedDefaultAllocator<32> >>(this, offset->second);
}
const TArray<UObject *,TSizedDefaultAllocator<32> > UStruct::GetScriptAndPropertyObjectReferences() const
{
    static auto offset = MemberOffsets.find(STR("ScriptAndPropertyObjectReferences"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::ScriptAndPropertyObjectReferences' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<const TArray<UObject *,TSizedDefaultAllocator<32> >>(this, offset->second);
}

TArray<UObject *,FDefaultAllocator> UStruct::GetScriptObjectReferences()
{
    static auto offset = MemberOffsets.find(STR("ScriptObjectReferences"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::ScriptObjectReferences' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<TArray<UObject *,FDefaultAllocator>>(this, offset->second);
}
const TArray<UObject *,FDefaultAllocator> UStruct::GetScriptObjectReferences() const
{
    static auto offset = MemberOffsets.find(STR("ScriptObjectReferences"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::ScriptObjectReferences' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<const TArray<UObject *,FDefaultAllocator>>(this, offset->second);
}

UStruct* UStruct::GetSuperStruct()
{
    static auto offset = MemberOffsets.find(STR("SuperStruct"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::SuperStruct' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<UStruct*>(this, offset->second);
}
const UStruct* UStruct::GetSuperStruct() const
{
    static auto offset = MemberOffsets.find(STR("SuperStruct"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UStruct::SuperStruct' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<const UStruct*>(this, offset->second);
}

