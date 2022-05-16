std::unordered_map<std::wstring, int32_t> UObjectBase::MemberOffsets{};

UClass* UObjectBase::GetClassPrivate()
{
    static auto offset = MemberOffsets.find(STR("ClassPrivate"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UObjectBase::ClassPrivate' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<UClass*>(this, offset->second);
}
const UClass* UObjectBase::GetClassPrivate() const
{
    static auto offset = MemberOffsets.find(STR("ClassPrivate"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UObjectBase::ClassPrivate' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<const UClass*>(this, offset->second);
}

int32 UObjectBase::GetInternalIndex()
{
    static auto offset = MemberOffsets.find(STR("InternalIndex"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UObjectBase::InternalIndex' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<int32>(this, offset->second);
}
const int32 UObjectBase::GetInternalIndex() const
{
    static auto offset = MemberOffsets.find(STR("InternalIndex"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UObjectBase::InternalIndex' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<const int32>(this, offset->second);
}

FName UObjectBase::GetNamePrivate()
{
    static auto offset = MemberOffsets.find(STR("NamePrivate"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UObjectBase::NamePrivate' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<FName>(this, offset->second);
}
const FName UObjectBase::GetNamePrivate() const
{
    static auto offset = MemberOffsets.find(STR("NamePrivate"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UObjectBase::NamePrivate' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<const FName>(this, offset->second);
}

EObjectFlags UObjectBase::GetObjectFlags()
{
    static auto offset = MemberOffsets.find(STR("ObjectFlags"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UObjectBase::ObjectFlags' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<EObjectFlags>(this, offset->second);
}
const EObjectFlags UObjectBase::GetObjectFlags() const
{
    static auto offset = MemberOffsets.find(STR("ObjectFlags"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UObjectBase::ObjectFlags' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<const EObjectFlags>(this, offset->second);
}

UObject* UObjectBase::GetOuterPrivate()
{
    static auto offset = MemberOffsets.find(STR("OuterPrivate"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UObjectBase::OuterPrivate' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<UObject*>(this, offset->second);
}
const UObject* UObjectBase::GetOuterPrivate() const
{
    static auto offset = MemberOffsets.find(STR("OuterPrivate"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UObjectBase::OuterPrivate' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<const UObject*>(this, offset->second);
}

