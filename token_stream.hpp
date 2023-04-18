#ifndef TOKEN_STREAM_HPP
#define TOKEN_STREAM_HPP
#include <vector>
#include <string>
#include "db_types/debtor.hpp"

class TokenStream
{
public:
  TokenStream(const std::vector< std::string > &args);
  TokenStream(const std::vector< std::string > &args, std::vector< std::string >::const_iterator it);
  std::string readName();
  int readValue();
  Debtor readDebtor(bool valueExpected = false);
  void skip();
  std::vector< std::string >::const_iterator base() { return it_; }
  bool eof() { return eof_; }
  bool fail() { return fail_; }
  void clear() { fail_ = false; }
  operator bool();
private:
  const std::vector< std::string > &args_;
  std::vector< std::string >::const_iterator it_;
  bool fail_ = false;
  bool eof_ = false;
  inline void checkEof();
};

#endif