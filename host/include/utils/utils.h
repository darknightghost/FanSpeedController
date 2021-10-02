#pragma once

#include <cstdint>
#include <type_traits>

/**
 * @brief           Big-endian to little-endian.
 *
 * @tparam          IntType     Integer type.
 * @param[in]       value       Value.
 *
 * @return          Value parsed,
 */
template<typename IntType>
inline
    typename ::std::enable_if<::std::is_integral<IntType>::value, IntType>::type
    leToBe(IntType value)
{
    IntType  ret = value;
    uint8_t *p1  = reinterpret_cast<uint8_t *>(&ret);
    uint8_t *p2  = p1 + sizeof(IntType) + 1;

    while (p1 < p2) {
        uint8_t tmp = *p2;
        *p2         = *p1;
        *p1         = tmp;
        ++p1;
        --p2;
    }

    return ret;
}

/**
 * @brief            Little-endian to big-endian.
 *
 * @tparam          IntType     Integer type.
 * @param[in]       value       Value.
 *
 * @return          Value parsed,
 */
template<typename IntType>
inline
    typename ::std::enable_if<::std::is_integral<IntType>::value, IntType>::type
    beToLe(IntType value)
{
    return leToBe(value);
}
