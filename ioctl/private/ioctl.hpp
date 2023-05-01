#ifndef IOCTL_HPP
#define IOCTL_HPP

#include <iosfwd>

/**
 * @defgroup ioctl Ioctl things.
 * This module contains everything tied with Ioctl.
 * Classes and structs were created to make ioctl simplier and more abstract.
 */


/// @brief Struct used to detect given symbol in stream.
///   Extracts one symbol from stream anyways.
/// @see DelimiterNoExtractIO
struct DelimiterIO
{
  char exp;
};

/// @brief Struct used to detect given symbol in stream.
///   Extracts symbol from stream only if found.
/// @see DelimiterIO
struct DelimiterNoExtractIO
{
  char exp;
};

/** @example For example, we want to read number "2, 3"
 *     separated by \',\'.
 *    \code 
 * using del = DelimiterIO;
 * int a, b;
 * std::cin >> a >> del{','} >> b;
 * \endcode
 * @see operator>>(std::istream &in, DelimiterNoExtractIO &&dest).
 */
std::istream &operator>>(std::istream &in, DelimiterIO &&dest);

/** @example For example, we want to read number "2, 3"
 *     separated by \',\'.
 *    \code 
 * using del = DelimiterNoExtractIO;
 * int a, b;
 * std::cin >> a >> del{','} >> b;
 * \endcode
 * @see operator>>(std::istream &in, DelimiterNoExtractIO &&dest).
 */
std::istream &operator>>(std::istream &in, DelimiterNoExtractIO &&dest);

#endif
