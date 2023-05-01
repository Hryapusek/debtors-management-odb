#ifndef IOCTL_HPP
#define IOCTL_HPP

#include <iosfwd>
#include <boost/optional.hpp>
#include "db_types/debtor.hpp"
#include "db_types/operation.hpp"
/**
 * @defgroup ioctl Ioctl things.
 * This module contains everything tied with Ioctl.
 * Classes and structs were created to make ioctl simplier and more abstract.
 */

/**
 * @ingroup ioctl
 * @brief Struct used to read Debtor from istream.
 *
 * @details After reading debtor from stream, we don't know
 *    what fields are initialized, for example, id or name.
 *    This struct clearifies this moment.
 * @see OperationIO
 */
struct DebtorIO
{
  using ReadFields = int;
  /// @brief This enum contains bits refered to Debtor fields.
  enum ReadField {
    ID = 1, ///< ID in Debtor class
    NAME = 1 << 1, ///< Name in Debtor class
    DEBT = 1 << 2 ///< Debt in Debtor class
  };
  /// @brief Wrapped Debtor object.
  Debtor debtor;
  /**
   * @brief This one shows what fiels were initialized.
   * @details After successfull reading any of Debtor fields
   *    respective bits are set to 1.
   *
   * @example (readFields & ReadField::ID) == 1 checks if ID field was read from stream.
   */
  ReadFields readFields = 0;
  /// @brief Useful operator to access wrapped object easily.
  Debtor *operator->()
  {
    return &debtor;
  }
};

/// @ingroup ioctl
/// @brief This struct is used to read command from stream.
/// @details Both /cmd and cmd are correct
struct CommandIO
{
  std::string &command;
};

/**
 * @ingroup ioctl
 *
 * @brief Struct is used to read Operation from stream.
 *
 * @details After reading operation from stream, we don't know
 *    what fields are initialized, for example, id or name.
 *    This struct clearifies this moment.
 */
struct OperationIO
{
  using ReadFields = int;
  /// @brief This enum contains bits refered to Operation fields.
  enum ReadField {
    ID = 1, ///< id field.
    NAME = 1 << 1, ///< name field.
    VALUE = 1 << 2, ///< valueChange field.
    DECSRIPTION = 1 << 3 ///< description field.
  };
  /// @brief Debtor ID on which operation perfomed.
  unsigned int id;
  /// @brief Debtor name on which operation perfomed.
  std::string name;
  /// @brief Value that debt was changed.
  int valueChange;
  /// @brief Additional info about operation.
  std::string description;
  /**
   * @brief This one shows what fiels were initialized.
   * @details After successfull reading any of Debtor fields
   *    respective bits are set to 1.
   *
   * @example (readFields & ReadField::ID) == 1 checks if ID field was read from stream.
   */
  ReadFields readFields = 0;
};

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

namespace helpersIO
{
  /**
   * @ingroup ioctl
   * @brief This funciton helps to decide which database api function to call.
   *
   * @details For example, if \b id field was read, \b getDebtor(unsigned \b int \b id) will be called.
   *  Otherwise it will call \b getDebtor(const \b std::string \b &name).
   *
   * @param[in] dIO DebtorIO object that was read.
   * @see api::getDebtor(const std::string &name).
   * @see addValue(OperationIO &opIO).
   */
  boost::optional< Debtor > getDebtor(DebtorIO &dIO);
  /**
   * @ingroup ioctl
   * @brief This funciton helps to decide which database api function to call.
   *
   * @param[in] opIO OperationIO object that was read.
   * @see api::getDebtor(const std::string &name).
   * @see removeDebtor(DebtorIO &dIO).
   */
  void addValue(OperationIO &opIO);
  /**
   * @ingroup ioctl
   * @brief This funciton helps to decide which database api function to call.
   *
   * @param[in] dIO DebtorIO object that was read.
   * @see api::getDebtor(const std::string &name).
   * @see rollback(DebtorIO &dIO).
   */
  void removeDebtor(DebtorIO &dIO);
  /**
   * @ingroup ioctl
   * @brief This funciton helps to decide which database api function to call.
   *
   * @param[in] dIO DebtorIO object that was read.
   * @see api::getDebtor(const std::string &name).
   * @see getOperations(DebtorIO &dIO).
   */
  void rollback(DebtorIO &dIO);
  /**
   * @ingroup ioctl
   * @brief This funciton helps to decide which database api function to call.
   *
   * @param[in] dIO DebtorIO object that was read.
   * @see api::getDebtor(const std::string &name).
   */
  std::vector< Operation > getOperations(DebtorIO &dIO);
}
#endif