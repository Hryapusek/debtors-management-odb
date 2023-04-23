#ifndef TOKEN_STREAM_HPP
#define TOKEN_STREAM_HPP
#include <vector>
#include <string>
#include <iostream>
#include "ioctl.hpp"
#include "db_types/debtor.hpp"

/**
 * @ingroup ioctl
 * @brief This class is used to extract IO structs from stream.
 * @details It is much more comfortable to have class TokenStream
 *    to control input. Mostly used in commandsIO.cpp file.
 */
class TokenStream
{
public:
/**
 * @brief Construct a new Token Stream object
 * 
 * @param[in] in istream with commands and arguments.
 */
  TokenStream(std::istream &in) : in_(in) {}
  TokenStream(const TokenStream &) = delete;
  TokenStream(TokenStream &&) = delete;
  /**
   * @brief This one reads Debtor from stream in format
   *  {id, "name", debt}. One of id, name can be ommited. Debt also can be ommited. 
   * 
   * @param[in, out] d DebtorIO struct that will contain data. 
   * @return Stream to continue work with it.
   */
  TokenStream &operator>>(DebtorIO &d);
  /**
   * @brief Reads command from stream. Returns eof and no fail if eof of stream was met.
   * 
   * @param[in, out] cmd CommandIO struct used to wrap std::string command.
   * @return Stream to continue work with it.
   */
  TokenStream &operator>>(CommandIO cmd);
  /**
   * @brief This one reads Operation from stream in format
   *  {id, "name", valueChange, "description"}. One of id, name can be ommited. Description also can be ommited. 
   * 
   * @param[in, out] opIO OperationIO struct that will contain data. 
   * @return Stream to continue work with it.
   */
  TokenStream &operator>>(OperationIO &opIO);
  /// @brief Skips line till newline symbol including or eof.
  void skipRestLine();
  /// @brief Checks if end of line was reached.
  /// @return True if end of line was reached.
  bool eol();
  /// @brief Checks eof of given stream.
  /// @return istream.eof()
  inline bool eof() {
    return in_.eof();
  }
  /// @brief Checks fail of given stream.
  /// @return istream.fail()
  inline bool fail() {
    return in_.fail();
  }
  /// @brief Clears failbit of given stream.
  inline void clear() {
    in_.clear(in_.rdstate() & ~std::ios::failbit);
  }
  /// @brief Checks if there anything to read on current line.
  /// @return !(eof() || eol());
  operator bool();
private:
  std::istream &in_;
  void readID(unsigned int &id, bool &readID);
  void readString(std::string &name, bool &readString);
  void readInt(int &value, bool &readInt);
};

#endif