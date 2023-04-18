#ifndef OPERATION_HPP
#define OPERATION_HPP
#include <string>
#include <memory>
#include <odb/core.hxx>
#include <odb/tr1/memory.hxx>

class Debtor;
#include "debtor.hpp"

#pragma db object table("Operations") pointer(std::shared_ptr) session
class Operation
{
  using str_c = const std::__cxx11::basic_string<char>;
public:
  Operation () { }           // (3)
  Operation (std::shared_ptr< Debtor > d, int valueChange, long long int, std::string description); 

  inline unsigned long id() const { return id_; }

  std::shared_ptr<Debtor> debtor() const { return debtor_; };

  long long int time() const { return time_; }

  inline int valueChange() const { return valueChange_; }

private:

  friend class odb::access; // (4)

  #pragma db id auto        // (5)
  unsigned long id_;        // (5)

  #pragma db not_null column("DebtorID")
  std::shared_ptr<Debtor> debtor_;

  #pragma db not_null column("ValueChange")
  int valueChange_;

  #pragma db auto not_null type("TIMESTAMP") column("Time") default("now()")
  long long int time_;

  #pragma db column("Description")
  std::string description_;
};


#endif