std::unordered_map<std::wstring, int32_t> FArrayProperty::MemberOffsets{};

FProperty* FArrayProperty::GetInner()
{
    static auto offset = MemberOffsets.find(STR("Inner"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'FArrayProperty::Inner' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<FProperty*>(this, offset->second);
}
const FProperty* FArrayProperty::GetInner() const
{
    static auto offset = MemberOffsets.find(STR("Inner"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'FArrayProperty::Inner' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<const FProperty*>(this, offset->second);
}

