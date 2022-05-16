std::unordered_map<std::wstring, int32_t> UField::MemberOffsets{};

UField* UField::GetNext()
{
    static auto offset = MemberOffsets.find(STR("Next"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UField::Next' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<UField*>(this, offset->second);
}
const UField* UField::GetNext() const
{
    static auto offset = MemberOffsets.find(STR("Next"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UField::Next' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<const UField*>(this, offset->second);
}

