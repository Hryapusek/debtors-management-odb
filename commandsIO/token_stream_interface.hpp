#ifndef TOKEN_STREAM_INTERFACE_HPP
#define TOKEN_STREAM_INTERFACE_HPP
#include "structsIO.hpp"

/**
 * @brief This is abstract class aka interface for @ref processInput().
 * 
 * @details Other classes should implement this one so they can
 *    be used in processInput function. Function processInput() expects
 *    described below behaviour.
 */
class TokenStreamInterface
{
public:
  /// @brief Get @ref CommandIO.
  /// @note EOF must be set if no command found.
  virtual void read(CommandIO &) = 0;
  /// @brief Get @ref DebtorIO.
  /// @note Failbit must be set if no DebtorIO were passed.
  virtual void read(DebtorIO &) = 0;
  /// @brief Get @ref NewDebtorIO.
  /// @note Failbit must be set if no NewDebtorIO were passed.
  virtual void read(NewDebtorIO &) = 0;
  /// @brief Get @ref DebtorNumIO.
  /// @note Failbit must be set if no DebtorNumIO were passed.
  virtual void read(DebtorNumIO &) = 0;
  /// @brief Get @ref OperationIO.
  /// @note Failbit must be set if no OperationIO were passed.
  virtual void read(OperationIO &) = 0;
  /// @brief Skip current command with args.
  virtual void skipCurrentCommand() = 0;
  /// @brief Check if there is something to get.
  /// @return True if stream is empty.
  virtual bool eof() = 0;
  /// @brief Check if there is something to get from current command args.
  /// @return True if no more args present in current command.
  virtual bool eol() = 0;
  /// @brief Check if last operation failed.
  /// @return True if failed.
  virtual bool fail() = 0;
  /// @brief Rescue stream after failed operation.
  virtual void clear() = 0;
};

#endif