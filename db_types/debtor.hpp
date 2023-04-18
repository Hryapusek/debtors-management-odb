#ifndef DEBTOR_HPP
#define DEBTOR_HPP
#include <string>
#include <memory>
#include <vector>
#include <odb/core.hxx>
#include <odb/tr1/memory.hxx>

class Operation;
#include "operation.hpp"

#pragma db object table("Debtors") pointer(std::shared_ptr) session
class Debtor
{
  using str_c = const std::__cxx11::basic_string<char>;
public:
  Debtor () { }           // (3)
  Debtor (str_c, int);

  inline str_c &name() const { return name_; };
  void name(str_c &n) { name_ = n; };

  inline int debt() const { return debt_; }
  void debt(int d) { debt_ = d; };

  inline const std::vector<std::weak_ptr<Operation> > &operations() { return operations_; }

  inline unsigned long id() const { return id_; }

private:

  friend class odb::access; // (4)

  #pragma db id auto        // (5)
  unsigned long id_;        // (5)

  #pragma db not_null
  std::string name_;
  
  #pragma db not_null
  int debt_;

  #pragma db not_null inverse(debtor_)
  std::vector<std::weak_ptr<Operation> > operations_;
};


#endif