#ifndef IOCTL_HPP
#define IOCTL_HPP

#include <iosfwd>

/// @ingroup ioctl
/// @brief Struct used to detect given symbol in stream.
///   Extracts one symbol from stream anyways.
/// @see @ref DelimiterNoExtractIO
struct DelimiterIO
{
  char exp;
};

/// @ingroup ioctl
/// @brief Struct used to detect given symbol in stream.
///   Extracts symbol from stream only if found.
/// @see @ref DelimiterIO
struct DelimiterNoExtractIO
{
  char exp;
};

/** 
 * @ingroup ioctl
 * @brief istream operator extracting delimiter.
 * @note Even wrong delimiter will be extracted
 * @see @ref operator>>(std::istream &, DelimiterNoExtractIO &&).
 */
std::istream &operator>>(std::istream &in, DelimiterIO &&dest);

/** 
 * @ingroup ioctl
 * @brief istream operator extracting delimiter.
 * @note No extraction if wrong delimiter found.
 * @see operator>>(std::istream &in, DelimiterNoExtractIO &&dest).
 */
std::istream &operator>>(std::istream &in, DelimiterNoExtractIO &&dest);

#endif
