#include "token_stream.hpp"
#include <boost/regex.hpp>
#include <iostream>

namespace
{
  bool isName(const std::string &name)
  {
    boost::regex ex("[a-zA-Zа-яА-Я]{2,20}");
    return boost::regex_match(name, ex);
  }
}


TokenStream::TokenStream(const std::vector< std::string > &args) :
  args_(args),
  it_(args.begin())
{ }

TokenStream::TokenStream(const std::vector< std::string > &args, std::vector< std::string >::const_iterator it) :
  args_(args),
  it_(it)
{ }

std::string TokenStream::readName()
{
  if (eof_ || fail_)
  {
    fail_ = true;
    return std::string();
  }
  std::string name;
  for (; it_ != args_.end(); ++it_)
  {
    if (!isName(*it_) && !isName(it_->substr(0, it_->size() - 1)))
    {
      fail_ = true;
      return name;
    }
    if (it_->back() == '\\')
    {
      name += *it_ + " ";
      name.erase(---- name.end());
      continue;
    }
    else
    {
      name += *it_;
      ++it_;
      break;
    }
  }
  if (name.empty())
    fail_ = true;
  else if (name.back() == ' ')
    name.pop_back();
  checkEof();
  return name;
}

int TokenStream::readValue()
{
  if (eof_ || fail_)
  {
    fail_ = true;
    return 0;
  }
  try
  {
    int res = std::stoi(*it_);
    ++it_;
    checkEof();
    return res;
  }
  catch (const std::exception &e)
  {
    fail_ = true;
    return 0;
  }
}

Debtor TokenStream::readDebtor(bool valueExpected)
{
  if (eof_ || fail_)
  {
    fail_ = true;
    return Debtor();
  }
  Debtor d;
  d.name(readName());
  d.debt(readValue());
  if (d.name() == "" || (fail_ && valueExpected))
  {
    fail_ = true;
    return Debtor();
  }
  clear();
  checkEof();
  return d;
}

void TokenStream::skip()
{
  ++it_;
  checkEof();
}

TokenStream::operator bool()
{
  return !eof_ && !fail_;
}

inline void TokenStream::checkEof()
{
  eof_ = (it_ == args_.end());
}
