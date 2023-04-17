#include "debtor.hpp"

using str_c = const std::__cxx11::basic_string< char >;

Debtor::Debtor(str_c name, int debt) :
  name_(name),
  debt_(debt)
{ }
