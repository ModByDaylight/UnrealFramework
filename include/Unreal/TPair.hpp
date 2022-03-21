#ifndef RC_UNREAL_TPAIR_HPP
#define RC_UNREAL_TPAIR_HPP

namespace RC::Unreal
{
    template<typename KeyType, typename ValueType>
    struct TPair
    {
        KeyType Key;
        ValueType Value;
    };
}

#endif //RC_UNREAL_TPAIR_HPP
