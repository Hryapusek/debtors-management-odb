#include "ioctl.hpp"

#include <iostream>

std::istream &operator>>(std::istream &in, DelimiterIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (c != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream &operator>>(std::istream &in, DelimiterNoExtractIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  if (in.peek() != dest.exp)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  else
  {
    in.ignore(1);
    return in;
  }
}
