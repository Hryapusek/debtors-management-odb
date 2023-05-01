#include "db_api.hpp"
#include "dbconnector.hpp"
#include "../db_types/debtor.hpp"
#include "../db_types/debtor-odb.hxx"
#include "../db_types/operation.hpp"
#include "../db_types/operation-odb.hxx"
#include <odb/pgsql/database.hxx>
#include <odb/transaction.hxx>
#include <odb/session.hxx>


using namespace odb::core;

namespace api
{
  void removeDebtor_(std::shared_ptr<Debtor> debtor)
  {
    auto &db = DbConnector::db;
    db->erase(debtor);
  }

  void addValue_(std::shared_ptr<Debtor> debtor, int val, const std::string &description)
  {
    auto &db = DbConnector::db;
    debtor->debt(debtor->debt() + val);
    db->update(debtor);
    Operation op(debtor, val, description);
    db->persist< Operation >(op);
  }

  void rollback_(std::shared_ptr<Debtor> debtor)
  {
    auto &db = DbConnector::db;
    const auto &operations = debtor->operations();
    if (operations.empty())
      throw api_exception("Can not rollback: no operations found");
    std::shared_ptr< Operation > lastOperationPtr = operations.back().lock();
    debtor->debt(debtor->debt() - lastOperationPtr->valueChange());
    db->update(debtor);
    db->erase< Operation >(lastOperationPtr);
  }

  std::vector< Operation > getOperations_(unsigned int id)
  {
    auto &db = DbConnector::db;
    result< Operation > res(db->query< Operation >(query< Operation >::debtor == id));
    std::vector< Operation > operations;
    std::copy(res.begin(), res.end(), std::back_inserter(operations));
    return operations;
  }

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
      return boost::none;
    Debtor d = *res;
    t.commit();
    return d;
  }

  boost::optional< Debtor > getDebtor(unsigned long id)
  {
    auto &db = DbConnector::db;
    odb::session s;
    transaction t(db->begin());
    auto res = db->query_one< Debtor >(query< Debtor >::id == id);
    if (res == nullptr)
      return boost::none;
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
      throw debtor_not_found("Debtor with name " + name + " was not found");
    removeDebtor_(debtor);
    t.commit();
  }

  void removeDebtor(unsigned long id)
  {
    auto &db = DbConnector::db;
    odb::session s;
    transaction t(db->begin());
    auto debtor = db->query_one< Debtor >(query< Debtor >::id == id);
    if (debtor == nullptr)
      throw debtor_not_found("Debtor with id " + std::to_string(id) + " was not found");
    removeDebtor_(debtor);
    t.commit();
  }

  void addValue(const std::string &name, int val, const std::string &description)
  {
    auto &db = DbConnector::db;
    odb::session s;
    transaction t(db->begin());
    auto debtor = db->query_one< Debtor >(query< Debtor >::name == name);
    if (debtor == nullptr)
      throw debtor_not_found("Debtor with name " + name + " was not found");
    addValue_(debtor, val, description);
    t.commit();
  }

  void addValue(unsigned int id, int val, const std::string &description)
  {
    auto &db = DbConnector::db;
    odb::session s;
    transaction t(db->begin());
    auto debtor = db->query_one< Debtor >(query< Debtor >::id == id);
    if (debtor == nullptr)
      throw debtor_not_found("Debtor with id " + std::to_string(id) + " was not found");
    addValue_(debtor, val, description);
    t.commit();
  }

  void rollback(const std::string &name)
  {
    auto &db = DbConnector::db;
    odb::session s;
    transaction t(db->begin());
    auto debtor = db->query_one< Debtor >(query< Debtor >::name == name);
    if (debtor == nullptr)
      throw debtor_not_found("Debtor with name " + name + " was not found");
    rollback_(debtor);
    t.commit();
  }

  void rollback(unsigned int id)
  {
    auto &db = DbConnector::db;
    odb::session s;
    transaction t(db->begin());
    auto debtor = db->query_one< Debtor >(query< Debtor >::id == id);
    if (debtor == nullptr)
      throw debtor_not_found("Debtor with id " + std::to_string(id) + " was not found");
    rollback_(debtor);
    t.commit();
  }

  std::vector< Operation > getOperations(const std::string &name)
  {
    auto &db = DbConnector::db;
    Debtor d;
    {
      auto res = getDebtor(name);
      if (!res)
        throw debtor_not_found("Debtor with name " + name + " was not found");
      d = res.get();
    }
    odb::session s;
    transaction t(db->begin());
    std::vector< Operation > operations = getOperations_(d.id());
    t.commit();
    return operations;
  }

  std::vector< Operation > getOperations(unsigned int id)
  {
    auto &db = DbConnector::db;
    odb::session s;
    transaction t(db->begin());
    std::vector< Operation > operations = getOperations_(id);
    t.commit();
    return operations;
  }
}
