#ifndef DB_API_HPP
#define DB_API_HPP

#include <vector>
#include <string>
#include <boost/optional.hpp>
#include "db_types/debtor.hpp"

namespace api
{
  std::vector< Debtor > getDebtors();
  boost::optional< Debtor > getDebtor(const std::string &name);
  Debtor getDebtor(unsigned long id);
  void addDebtor(Debtor d);
  void removeDebtor(const std::string &name);
  void removeDebtor(unsigned long id);
  void addValue(std::string name, int val);
}

#endif
