#include "token_stream.hpp"
#include <boost/regex.hpp>

namespace
{
  bool isName(const std::string &name)
  {
    boost::regex ex("[a-zA-Zа-яА-Я]{2,20}");
    return boost::regex_match(name, ex);
  }
}

std::istream &TokenStream::skipWs()
{
  while (!in_.fail() && !in_.eof() && !isDelim(in_.peek()) && isspace(in_.peek()))
    in_.ignore(1);
  return in_;
}

bool TokenStream::isDelim(char ch)
{
  return delimiters_.find_first_of(in_.peek()) != std::string::npos;
}

void TokenStream::skipCurrentCommand()
{
  while (!in_.eof() && !isDelim(in_.peek()))
    in_.ignore(1);
  in_.ignore(1);
}

bool TokenStream::eol()
{
  return isDelim(skipWs().peek());
}

bool TokenStream::readString(std::string &name)
{
  if (skipWs().peek() != '"')
    return false;
  std::string tName;
  in_.ignore(1);
  char ch;
  while (!in_.eof() && in_ && in_.peek() != '"' && !isDelim(in_.peek()))
  {
    in_.get(ch);
    tName.push_back(ch);
  }
  if (failIf(!in_ || in_.eof() || isDelim(in_.peek())))
    return false;
  in_.ignore(1);
  name = tName;
  return true;
}

bool TokenStream::readUnsignedInt(unsigned int &id)
{
  if (!isdigit(skipWs().peek()))
    return false;
  unsigned int tId;
  in_ >> tId;
  if (!in_)
    return false;
  id = tId;
  return true;
}

bool TokenStream::readInt(int &value)
{
  if (skipWs().peek() != '-' && !isdigit(in_.peek()))
    return false;
  int tValue;
  in_ >> std::noskipws >> tValue >> std::skipws;
  if (!in_)
    return false;
  value = tValue;
  return true;
}

bool TokenStream::failIf(bool condition)
{
  if (condition)
    in_.setstate(std::ios::failbit);
  return condition;
}

void TokenStream::read(CommandIO &cmd)
{
  if (std::ws(in_).eof())
    return;
  std::string tempCommand;
  in_ >> std::noskipws >> tempCommand >> std::skipws;
  if (tempCommand[0] == '/')
    tempCommand = tempCommand.substr(1);
  cmd.command = std::move(tempCommand);
  return;
}

void TokenStream::read(DebtorIO &d)
{
  using del = DelimiterNoExtractIO;
  skipWs();
  std::istream::sentry s(in_, true);
  if (failIf(!s || in_.eof() || isDelim(in_.peek())))
    return;
  DebtorIO tDebtorIO;
  in_ >> del{ '{' };
  unsigned int id;
  std::string name;
  if (readUnsignedInt(id))
    tDebtorIO.id = id;
  if (skipWs().peek() == ',')
    in_.ignore(1);
  if (readString(name))
    tDebtorIO.name = name;
  if (failIf(!tDebtorIO.id.has_value() && !tDebtorIO.name.has_value()))
    return;
  if (failIf(skipWs().peek() != '}'))
    return;
  in_.ignore(1);
  if (in_)
    d = tDebtorIO;
  return;
}

void TokenStream::read(NewDebtorIO &d)
{
  using del = DelimiterNoExtractIO;
  skipWs();
  std::istream::sentry s(in_, true);
  if (failIf(!s || in_.eof() || isDelim(in_.peek())))
    return;
  NewDebtorIO tDebtorIO;
  in_ >> del{ '{' };
  std::string name;
  int debt;
  if (readString(name))
    tDebtorIO.name = name;
  if (failIf(!tDebtorIO.name.has_value()))
    return;
  if (skipWs().peek() == ',')
    in_.ignore(1);
  if (readInt(debt))
    tDebtorIO.debt = debt;
  if (failIf(skipWs().peek() != '}'))
    return;
  in_.ignore(1);
  if (in_)
    d = tDebtorIO;
  return;
}

void TokenStream::read(DebtorNumIO &d)
{
  using del = DelimiterNoExtractIO;
  skipWs();
  std::istream::sentry s(in_, true);
  if (failIf(!s || in_.eof() || isDelim(in_.peek())))
    return;
  in_ >> del{ '{' };
  DebtorNumIO tDebtorIO;
  read(tDebtorIO.debtor);
  if (skipWs().peek() == ',')
    in_.ignore(1);
  unsigned int num;
  if (readUnsignedInt(num))
    tDebtorIO.num = num;
  if (failIf(skipWs().peek() != '}'))
    return;
  in_.ignore(1);
  if (in_)
    d = tDebtorIO;
  return;
}

void TokenStream::read(OperationIO &op)
{
  using del = DelimiterNoExtractIO;
  skipWs();
  std::istream::sentry s(in_, true);
  if (failIf(!s || in_.eof() || isDelim(in_.peek())))
    return;
  in_ >> del{ '{' };
  OperationIO tOperationIO;
  read(tOperationIO.debtor);
  if (skipWs().peek() == ',')
    in_.ignore(1);
  int valueChange;
  if (failIf(!readInt(valueChange)))
    return;
  tOperationIO.valueChange = valueChange;
  if (skipWs().peek() == ',')
    in_.ignore(1);
  std::string description;
  if (readString(description))
    tOperationIO.description = description;
  if (failIf(skipWs().peek() != '}'))
    return;
  in_.ignore(1);
  if (in_)
    op = tOperationIO;
  return;
}
