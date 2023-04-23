#include "token_stream.hpp"
#include <boost/regex.hpp>
#include "ioctl.hpp"

namespace
{
  bool isName(const std::string &name)
  {
    boost::regex ex("[a-zA-Zа-яА-Я]{2,20}");
    return boost::regex_match(name, ex);
  }

  //Skips isspace(in.peek()) but new line
  std::istream &skipWs(std::istream &in)
  {
    while (!in.fail() && !in.eof() && in.peek() != '\n' && isspace(in.peek()))
      in.ignore(1);
    return in;
  }

}

TokenStream::operator bool()
{
  return !(eof() || eol());
}

TokenStream &TokenStream::operator>>(CommandIO cmd)
{
  if (std::ws(in_).eof())
    return *this;
  std::string tempCommand;
  in_ >> tempCommand;
  if (in_.fail())
    return *this;
  if (tempCommand[0] == '/')
    tempCommand = tempCommand.substr(1);
  cmd.command = std::move(tempCommand);
  return *this;
}

TokenStream &TokenStream::operator>>(OperationIO &opIO)
{
  using del = DelimiterNoExtractIO;
  skipWs(in_);
  std::istream::sentry s(in_);
  if (!s || in_.eof() || in_.peek() == '\n')
  {
    in_.setstate(std::ios::failbit);
    return *this;
  }
  OperationIO tOpIO;
  in_ >> del{ '{' };
  bool isRead = false;
  readID(tOpIO.id, isRead);
  if (isRead)
  {
    tOpIO.readFields = tOpIO.readFields | tOpIO.ID;
    in_ >> del{ ',' };
  }
  readString(tOpIO.name, isRead);
  if (isRead)
  {
    tOpIO.readFields = tOpIO.readFields | tOpIO.NAME;
    in_ >> del{ ',' };
  }
  if (!(tOpIO.readFields & (DebtorIO::ID | DebtorIO::NAME)))
  {
    in_.setstate(std::ios::failbit);
    return *this;
  }
  readInt(tOpIO.valueChange, isRead);
  if (!isRead)
  {
    in_.setstate(std::ios::failbit);
    return *this;
  }
  tOpIO.readFields = tOpIO.readFields | tOpIO.VALUE;
  if (in_.peek() == ',')
    in_.ignore(1);
  readString(tOpIO.description, isRead);
  if (isRead)
    tOpIO.readFields = tOpIO.readFields | tOpIO.DECSRIPTION;
  if (skipWs(in_).peek() != '}')
  {
    in_.setstate(std::ios::failbit);
    return *this;
  }
  in_.ignore(1);
  if (in_)
    opIO = std::move(tOpIO);
  return *this;
}

TokenStream &TokenStream::operator>>(DebtorIO &dIO)
{
  using del = DelimiterNoExtractIO;
  skipWs(in_);
  std::istream::sentry s(in_);
  if (!s || in_.eof() || in_.peek() == '\n')
  {
    in_.setstate(std::ios::failbit);
    return *this;
  }
  DebtorIO tDebtorIO;
  in_ >> del{ '{' };
  unsigned int id;
  bool isRead = false;
  readID(id, isRead);
  if (isRead)
  {
    tDebtorIO.debtor.id(id);
    tDebtorIO.readFields = tDebtorIO.readFields | tDebtorIO.ID;
  }
  if (in_.peek() == ',')
    in_ >> del{ ',' }
  ;
  std::string name;
  readString(name, isRead);
  if (isRead)
  {
    tDebtorIO.debtor.name(name);
    tDebtorIO.readFields = tDebtorIO.readFields | tDebtorIO.NAME;
  }
  if (!(tDebtorIO.readFields & (DebtorIO::ID | DebtorIO::NAME)))
  {
    in_.setstate(std::ios::failbit);
    return *this;
  }
  if (in_.peek() == ',')
  {
    in_.ignore(1);
    int debt;
    readInt(debt, isRead);
    if (isRead)
    {
      tDebtorIO.debtor.debt(debt);
      tDebtorIO.readFields = tDebtorIO.readFields | tDebtorIO.DEBT;
    }
  }
  if (skipWs(in_).peek() != '}')
  {
    in_.setstate(std::ios::failbit);
    return *this;
  }
  in_.ignore(1);
  if (in_)
    dIO = tDebtorIO;
  return *this;
}

void TokenStream::skipRestLine()
{
  while (!in_.eof() && in_.peek() != '\n')
    in_.ignore(1);
  in_.ignore(1);
}

bool TokenStream::eol()
{
  skipWs(in_);
  return in_.peek() == '\n';
}

void TokenStream::readString(std::string &name, bool &readString)
{
  readString = false;
  std::string tName;
  skipWs(in_);
  if (in_.peek() == '"')
  {
    in_.ignore(1);
    char ch;
    while (!in_.eof() && in_ && in_.peek() != '"' && in_.peek() != '\n')
    {
      in_ >> ch;
      tName.push_back(ch);
    }
    if (!in_ || in_.eof() || in_.peek() == '\n')
    {
      in_.setstate(std::ios::failbit);
      return;
    }
    in_.ignore(1);
    name = tName;
    readString = true;
  }
}

void TokenStream::readID(unsigned int &id, bool &readID)
{
  readID = false;
  unsigned int tId;
  skipWs(in_);
  if (isdigit(in_.peek()))
  {
    in_ >> tId;
    if (!in_)
    {
      in_.setstate(std::ios::failbit);
      return;
    }
    id = tId;
    readID = true;
  }
}

void TokenStream::readInt(int &value, bool &readInt)
{
  readInt = false;
  int tValue;
  skipWs(in_);
  if (isdigit(in_.peek()) || in_.peek() == '-')
  {
    in_ >> tValue;
    if (!in_)
    {
      return;
    }
    value = tValue;
    readInt = true;
  }
}
