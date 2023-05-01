#ifndef STRUCTSIO_HPP
#define STRUCTSIO_HPP
#include "../db_types/debtor.hpp"
#include <boost/optional.hpp>

/**
 * @ingroup ioctl
 * @brief
 *
 * @see
 */
struct DebtorIO
{
  boost::optional< int > id;
  boost::optional< std::string > name;
};

struct NewDebtorIO
{
  boost::optional< std::string > name;
  boost::optional< int > debt;
};

struct CommandIO
{
  boost::optional< std::string > command;
};

struct DebtorNumIO
{
  DebtorIO debtor;
  boost::optional< unsigned int > num;
};

/**
 * @ingroup ioctl
 *
 * @brief
 *
 * @details
 */
struct OperationIO
{
  DebtorIO debtor;
  /// @brief Value that debt was changed.
  int valueChange;
  /// @brief Additional info about operation.
  std::string description;
};
#endif
