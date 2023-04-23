#include "operation.hpp"

Operation::Operation(std::shared_ptr< Debtor > d, int valueChange, std::string description) :
  debtor_(d),
  valueChange_(valueChange),
  time_(time(nullptr)),
  description_(description)
{}