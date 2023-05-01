#ifndef TOKEN_STREAM_HPP
#define TOKEN_STREAM_HPP
#include <vector>
#include <string>
#include <iostream>
#include "private/ioctl.hpp"
#include "../db_types/debtor.hpp"
#include "../commandsIO/token_stream_interface.hpp"

/**
 * @ingroup ioctl
 * @brief This class is used to extract IO structs from stream.
 * @details It is much more comfortable to have class TokenStream
 *    to control input. Mostly used in commandsIO.cpp file.
 */
class TokenStream : public TokenStreamInterface
{
public:
  TokenStream(std::istream &in) : in_(in) {}
  TokenStream(const TokenStream &) = delete;
  TokenStream(TokenStream &&) = delete;
  void read(CommandIO&);
  /// @brief Reads in format {id, "name"}.
  ///   One of args can be ommited.
  /// @overload
  void read(DebtorIO&);
  /// @brief Reads in format {"name", intValue}.
  ///   intValue can be ommited.
  /// @overload
  void read(NewDebtorIO&);
  void read(DebtorNumIO&);
  void read(OperationIO&);
  void skipCurrentCommand();
  bool eol();
  inline bool eof() {
    return in_.eof();
  }
  inline bool fail() {
    return in_.fail();
  }
  inline void clear() {
    in_.clear(in_.rdstate() & ~std::ios::failbit);
  }
  operator bool();
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