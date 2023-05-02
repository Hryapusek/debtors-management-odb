#ifndef TOKEN_STREAM_HPP
#define TOKEN_STREAM_HPP
#include <vector>
#include <string>
#include <iostream>
#include "private/ioctl.hpp"
#include "../db_types/debtor.hpp"
#include "../commandsIO/token_stream_interface.hpp"

/** @defgroup ioctl IO control elements.
 *    This group contains everything that is used to work with istream.
*/

/**
 * @ingroup ioctl
 * @brief Class implements @ref TokenStreamInterface so 
 *    @ref processInput could extract commands and structs
 *    from given istream.
 */
class TokenStream : public TokenStreamInterface
{
public:
  TokenStream(std::istream &in) : in_(in) {}
  TokenStream(const TokenStream &) = delete;
  TokenStream(TokenStream &&) = delete;
  /// @brief Reads a word in CommandIO struct 
  ///     and removes start symbol '/' if present.
  /// @note Returns uninitialized struct if eof was met.
  ///     Failbit is not set.
  void read(CommandIO&);
  /// @brief Reads in format {id, "name"}.
  ///   One of args can be ommited.
  /// @overload
  void read(DebtorIO&);
  /// @brief Reads in format {"name", intValue}.
  ///   intValue can be ommited.
  /// @overload
  void read(NewDebtorIO&);
  /// @brief Reads in format {DebtorIO, unsigned_int_value}.
  ///   intValue can be ommited.
  /// @example {{"Lenya"}, 5} {{21}, 10} {{12}}
  /// @overload
  void read(DebtorNumIO&);
  /// @brief Reads in format {DebtorIO, intValue, description}.
  ///   description can be ommited.
  /// @example {{"Lenya"}, 500, "Added 500 to Lenya"}
  ///          {{21}, -100, "Substructed 100 from user with id 21"}
  /// @overload
  void read(OperationIO&);
  /// @brief Skips to the next command, separated by Delimiter symbol.
  /// @note Delimiter symbol is extracted if present. EOF bit could be set.
  void skipCurrentCommand();
  /// @brief Checks if delimiter or were met.
  /// @return True if delimiter or eof were met. False otherwise.
  bool eol();
  /// @brief Returns default istream eof().
  /// @return istream.eof()
  inline bool eof() {
    return in_.eof();
  }
  /// @brief Returns default istream fail().
  /// @return istream.fail()
  inline bool fail() {
    return in_.fail();
  }
  /// @brief Clears failbit of the stream.
  inline void clear() {
    in_.clear(in_.rdstate() & ~std::ios::failbit);
  }
private:
  std::string delimiters_ = "\n";
  std::istream &in_;
  bool readUnsignedInt(unsigned int &id);
  bool readString(std::string &name);
  bool readInt(int &value);
  //Skips isspace(in.peek()) but new line
  std::istream &skipWs();
  bool isDelim(char ch);
  /// @brief Sets failbit if condition true.
  /// @param[in] condition Will be returned 
  /// @return value of condition param
  bool failIf(bool condition);
};

#endif