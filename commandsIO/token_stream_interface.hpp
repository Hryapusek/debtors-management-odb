#ifndef TOKEN_STREAM_INTERFACE_HPP
#define TOKEN_STREAM_INTERFACE_HPP
#include "structsIO.hpp"

class TokenStreamInterface
{
public:
  virtual void read(CommandIO &) = 0;
  virtual void read(DebtorIO &) = 0;
  virtual void read(NewDebtorIO &) = 0;
  virtual void read(DebtorNumIO &) = 0;
  virtual void read(OperationIO &) = 0;
  virtual void skipCurrentCommand() = 0;
  virtual bool eof() = 0;
  virtual bool eol() = 0;
  virtual bool fail() = 0;
  virtual void clear() = 0;
};

#endif