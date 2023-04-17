#include "db_api.hpp"
#include "dbconnector.hpp"
#include "db_types/debtor-odb.hxx"
#include <odb/pgsql/database.hxx>
#include <odb/transaction.hxx>
#include <odb/session.hxx>


using namespace odb::core;

namespace api
{
  std::vector< Debtor > getDebtors()
  {
    auto &db = DbConnector::db;
    odb::session s;
    transaction t(db->begin());
    result< Debtor > res(db->query< Debtor >());
    std::vector< Debtor > debtors;
    std::copy(res.begin(), res.end(), std::back_inserter(debtors));
    t.commit();
    return debtors;
  }

  boost::optional< Debtor > getDebtor(const std::string &name)
  {
    auto &db = DbConnector::db;
    odb::session s;
    transaction t(db->begin());
    auto res = db->query_one< Debtor >(query< Debtor >::name == name);
    if (res == nullptr)
      return boost::optional< Debtor >();
    Debtor d = *res;
    t.commit();
    return d;
  }

  Debtor getDebtor(unsigned long id)
  {
    auto &db = DbConnector::db;
    odb::session s;
    transaction t(db->begin());
    auto res = db->query_one< Debtor >(query< Debtor >::id == id);
    if (res == nullptr)
      throw std::logic_error("Debtor::getDebtor(unsigned long id): User with id " + std::to_string(id) + " was not found");
    Debtor d = *res;
    t.commit();
    return d;
  }

  void addDebtor(Debtor d)
  {
    auto &db = DbConnector::db;
    odb::session s;
    transaction t(db->begin());
    db->persist(d);
    t.commit();
  }

  void removeDebtor(const std::string &name)
  {
    auto &db = DbConnector::db;
    odb::session s;
    transaction t(db->begin());
    auto dp = db->query_one< Debtor >(query< Debtor >::name == name);
    if (dp == nullptr)
      throw std::logic_error("User with name " + name + " was not found");
    db->erase(db->load< Debtor>(dp->id()));
    t.commit();
  }

  void removeDebtor(unsigned long id)
  {
    auto &db = DbConnector::db;
    odb::session s;
    transaction t(db->begin());
    db->erase(db->load< Debtor >(id));
    t.commit();
  }

  void addValue(std::string name, int val)
  {
    auto &db = DbConnector::db;
    odb::session s;
    transaction t(db->begin());
    auto debtor = db->load< Debtor >((db->query_value< Debtor >(query< Debtor >::name == name)).id());
    debtor->debt(debtor->debt() + val);
    db->update(debtor);
    Operation op(debtor, val, 0, "test");
    db->persist< Operation >(op);
    t.commit();
  }
}
