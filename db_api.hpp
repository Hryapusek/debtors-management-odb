#ifndef DB_API_HPP
#define DB_API_HPP

#include <vector>
#include <string>
#include <boost/optional.hpp>
#include "db_types/debtor.hpp"
/**
 * @defgroup database_api Database API
 * This module contains all functions that directly interract with database.
 */
namespace api
{
  ///@brief Exception class thrown when no debtors were found in database.
  class debtor_not_found: public std::exception
  {
    std::string msg_;
  public:
    debtor_not_found(const std::string &msg) : msg_(msg) {}
    const char *what() const noexcept override
    { 
      return msg_.c_str();
    }
  };

  /// @brief Exception thrown when operation 
  ///   can not be perfomed by logical causes.
  class api_exception: public std::exception
  {
    std::string msg_;
  public:
    api_exception(const std::string &msg) : msg_(msg) {}
    const char *what() const noexcept override
    { 
      return msg_.c_str();
    }
  };
  /**
   * @brief Gets vector of all Debtors from "Debtors" table. Can be empty.
   * @throws Exceptions tied with database connection.
   * @return Vector of all Debtors.
   * @ingroup database_api
   */
  std::vector< Debtor > getDebtors();
  /**
   * @ingroup database_api
   * @brief Gets debtor by \b name from "Debtors" table.
   *
   * @throws Exceptions tied with database connection.
   * @param[in] name Name that have to be found.
   * @return If exists returns Debtor with this name, otherwise boost::none.
   */
  boost::optional< Debtor > getDebtor(const std::string &name);
  /**
   * @ingroup database_api
   * @brief Gets debtor by \b ID from "Debtors" table.
   * @param[in] id ID that have to be found.
   * @overload
   */
  boost::optional< Debtor > getDebtor(unsigned long id);
  /**
   * @ingroup database_api
   * @brief Tries to insert given Debtor into "Debtors" table.
   *
   * @throws odb::object_already_persistent if debtor with this name already exists.
   * @throws Other exceptions tied with database connection.
   * @param[in] d Debtor to be inserted.
   */
  void addDebtor(Debtor d);
  /**
   * @ingroup database_api
   * @brief Tries to remove debtor with given name.
   *
   * @throws api::debtor_not_found if debtor with given name not found.
   * @throws Other exceptions tied with database connection.
   *
   * @param[in] name Name of the debtor to remove.
   */
  void removeDebtor(const std::string &name);
  /**
   * @ingroup database_api
   * @brief Tries to remove debtor with given id.
   * @param[in] id ID of the debtor to remove.
   * @overload
   */
  void removeDebtor(unsigned long id);
  /**
   * @ingroup database_api
   * @brief Adds value to the debtor with given name. Value can be negative.
   * @details Updates debtor in "Debtors" table and creates an operation in "Operations" table.
   *
   * @throws - api::debtor_not_found if debtor with given name not found.
   * @throws - odb::database_exception if val == 0.
   * @throws - Other exceptions tied with database connection.
   *
   * @param name Name of debtor whos debt changed.
   * @param val Value to be added to debtor's debt.
   * @param description Description of the operation. Empty by default.
   */
  void addValue(const std::string &name, int val, const std::string &description = "");
  /**
   * @ingroup database_api
   * @brief Adds value to the debtor with given ID. Value can be negative.
   * @param id Name of debtor whos debt changed.
   * @overload
   */
  void addValue(unsigned int id, int val, const std::string &description = "");
  /**
   * @ingroup database_api
   * @brief Tries to rollback last operation attached to debtor with given name.
   *
   * @throws api::debtor_not_found if debtor with given name not found.
   * @throws std::logic_error if no operations found.
   * @throws Other exceptions tied with database connection.
   *
   * @param[in] name Name of the debtor to rollback.
   */
  void rollback(const std::string &name);
  /**
   * @ingroup database_api
   * @brief Tries to rollback last operation attached to debtor with given ID.
   * @param[in] id ID of the debtor to rollback.
   * @overload
   */
  void rollback(unsigned int id);
  /**
   * @ingroup database_api
   * @brief Tries to get operations of the debtor with given name.
   *
   * @throws api::debtor_not_found if debtor with given name not found.
   * @throws Other exceptions tied with database connection.
   * @param[in] name Name of the debtor to get operations.
   * @return Vector with debtor's operations.
   */
  std::vector< Operation > getOperations(const std::string &name);
  /**
   * @ingroup database_api
   * @brief Tries to get operations of the debtor with given ID.
   * @param[in] id ID of the debtor to get operations.
   * @overload
   */
  std::vector< Operation > getOperations(unsigned int id);
}

#endif
