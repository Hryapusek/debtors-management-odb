#include "db_api.hpp"
#include "dbconnector.hpp"
#include "db_types/debtor.hpp"
#include "db_types/debtor-odb.hxx"
#include "db_types/operation.hpp"
#include "db_types/operation-odb.hxx"
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
    auto debtor = db->query_one< Debtor >(query< Debtor >::name == name);
    if (debtor == nullptr)
      throw std::logic_error("User with name " + name + " was not found");
    db->erase(db->load< Debtor >(debtor->id()));
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

  void addValue(const std::string &name, int val, const std::string &description)
  {
    auto &db = DbConnector::db;
    odb::session s;
    transaction t(db->begin());
    auto debtor = db->query_one< Debtor >(query< Debtor >::name == name);
    if (debtor == nullptr)
      throw std::logic_error("User with name " + name + " was not found");
    debtor->debt(debtor->debt() + val);
    db->update(debtor);
    Operation op(debtor, val, 0, description);
    db->persist< Operation >(op);
    t.commit();
  }

  void rollback(const std::string &name)
  {
    auto &db = DbConnector::db;
    odb::session s;
    transaction t(db->begin());
    auto debtor = db->query_one< Debtor >(query< Debtor >::name == name);
    if (debtor == nullptr)
      throw std::logic_error("User with name " + name + " was not found");
    const auto &operations = debtor->operations();
    if (operations.empty())
      throw std::logic_error("Can not rollback: no operations found");
    std::shared_ptr< Operation > lastOperationPtr = operations.back().lock();
    if (!lastOperationPtr)
      throw std::logic_error("rollback(const std::string &name): empty shared_ptr");
    int valChange = lastOperationPtr->valueChange();
    debtor->debt(debtor->debt() - valChange);
    db->update(debtor);
    db->erase< Operation >(lastOperationPtr);
    t.commit();
  }

  std::vector< Operation > getOperations(const std::string &name)
  {
    auto &db = DbConnector::db;
    Debtor d;
    {
      auto res = getDebtor(name);
      if (!res)
        throw std::logic_error("User with name " + name + " was not found");
      d = res.get();
    }
    odb::session s;
    transaction t(db->begin());
    result< Operation > res(db->query< Operation >(query< Operation >::debtor == d.id()));
    std::vector< Operation > operations;
    std::copy(res.begin(), res.end(), std::back_inserter(operations));
    t.commit();
    return operations;
  }
}
