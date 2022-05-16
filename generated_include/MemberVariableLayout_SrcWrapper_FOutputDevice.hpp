std::unordered_map<std::wstring, int32_t> FOutputDevice::MemberOffsets{};

bool FOutputDevice::GetbAutoEmitLineTerminator()
{
    static auto offset = MemberOffsets.find(STR("bAutoEmitLineTerminator"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'FOutputDevice::bAutoEmitLineTerminator' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<bool>(this, offset->second);
}
const bool FOutputDevice::GetbAutoEmitLineTerminator() const
{
    static auto offset = MemberOffsets.find(STR("bAutoEmitLineTerminator"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'FOutputDevice::bAutoEmitLineTerminator' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<const bool>(this, offset->second);
}

bool FOutputDevice::GetbSuppressEventTag()
{
    static auto offset = MemberOffsets.find(STR("bSuppressEventTag"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'FOutputDevice::bSuppressEventTag' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<bool>(this, offset->second);
}
const bool FOutputDevice::GetbSuppressEventTag() const
{
    static auto offset = MemberOffsets.find(STR("bSuppressEventTag"));
    if (offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'FOutputDevice::bSuppressEventTag' that doesn't exist in this engine version."}; }
    return Helper::Casting::ptr_cast_deref<const bool>(this, offset->second);
}

