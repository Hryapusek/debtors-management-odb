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
  void addValue(const std::string &name, int val, const std::string &description = "");
  void rollback(const std::string &name);
  std::vector< Operation > getOperations(const std::string &name);
}

#endif
