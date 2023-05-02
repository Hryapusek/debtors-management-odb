/*
 * Here is Debtor class used to interract with database
 * This file will be used by odb compiler to generate files that will work with database
 */
#ifndef DEBTOR_HPP
#define DEBTOR_HPP
#include <string>
#include <memory>
#include <vector>
#include <odb/core.hxx>
#include <odb/tr1/memory.hxx>

class Operation;
#include "operation.hpp"

/**
 * @addtogroup database_api
 * @{
 */

#pragma db object table("Debtors") pointer(std::shared_ptr) session
/**
 * @brief This class used to interract with database. Related to "Debtors" table.
 * @details ODB compiler will generate debtor-odb.cxx .hxx .ixx files using this one.
 * @see Operation
*/
class Debtor
{
  using str_c = const std::__cxx11::basic_string<char>;
public:

  ///@brief Construct a new Debtor object.
  Debtor () : name_(""), debt_(0) { }
  /// @brief Constructs a new Debtor with given name and debt.
  /// @details No chekings are made.
  /// @param[in] name Debtor's new name.
  /// @param[in] debt Debtor's debt value.
  Debtor (str_c name, int debt);

  /// @brief Getter for name field.
  /// @return Const reference to name field.
  inline str_c &name() const { return name_; };
  /// @brief Setter for name field.
  /// @param name [in] new name.
  void name(str_c &name) { name_ = name; };

  /// @brief Getter for debt field.
  /// @return Copy of value of debt.
  inline int debt() const { return debt_; }

  /// @brief Setter for debt field.
  /// @param debt [in] New debt value.
  void debt(int debt) { debt_ = debt; };

  /** 
   * @brief Getter for operations, attached to this person from Operations table
   * @return Reference to the vector with all operations
   * This field is used to create connection between Debtors and Operations tables.
   * @note
   * Make sure you use this while a transaction. 
   * Otherwise, out of any transaction all pointers will be expired.
  */
  inline const std::vector<std::weak_ptr<Operation> > &operations() { return operations_; }

  /** 
   * @brief Getter for id field.
   * @return Copy of id field.
  */
  inline unsigned long id() const { return id_; }

  /** 
   * @brief Setter for id field.
  */
  inline void id(unsigned long id) { id_ = id; }

private:

  friend class odb::access;

  #pragma db id auto
  unsigned long id_;

  #pragma db not_null type("VARCHAR(40)")
  std::string name_;
  
  #pragma db not_null
  int debt_;

  #pragma db not_null inverse(debtor_)
  std::vector<std::weak_ptr<Operation> > operations_;
};
///@}

#endif