#include "ioctl.hpp"

#include <iostream>
#include "db_api.hpp"

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

boost::optional< Debtor > helpersIO::getDebtor(DebtorIO &dIO)
{
  if (dIO.readFields & dIO.ID)
    return api::getDebtor(dIO->id());
  else
    return api::getDebtor(dIO->name());
}

void helpersIO::addValue(OperationIO &opIO)
{
  if (opIO.readFields & opIO.NAME)
    api::addValue(opIO.name, opIO.valueChange, opIO.description);
  else if (opIO.readFields & opIO.ID)
    api::addValue(opIO.id, opIO.valueChange, opIO.description);
}

void helpersIO::removeDebtor(DebtorIO &dIO)
{
  if (dIO.readFields & dIO.NAME)
    api::removeDebtor(dIO->name());
  else if (dIO.readFields & dIO.ID)
    api::removeDebtor(dIO->id());
}

void helpersIO::rollback(DebtorIO &dIO)
{
  if (dIO.readFields & dIO.NAME)
    api::rollback(dIO->name());
  else if (dIO.readFields & dIO.ID)
    api::rollback(dIO->id());
}

std::vector< Operation > helpersIO::getOperations(DebtorIO &dIO)
{
  if (dIO.readFields & dIO.NAME)
    return api::getOperations(dIO->name());
  else
    return api::getOperations(dIO->id());
}
