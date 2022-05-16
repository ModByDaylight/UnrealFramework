std::unordered_map<std::wstring, int32_t> FStructProperty::MemberOffsets{};

UScriptStruct* FStructProperty::GetStruct()
{
    static auto offset = MemberOffsets.find(STR("Struct"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'FStructProperty::Struct' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<UScriptStruct*>(this, offset->second);
}
const UScriptStruct* FStructProperty::GetStruct() const
{
    static auto offset = MemberOffsets.find(STR("Struct"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'FStructProperty::Struct' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<const UScriptStruct*>(this, offset->second);
}

