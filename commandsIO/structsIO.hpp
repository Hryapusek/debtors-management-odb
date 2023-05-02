#ifndef STRUCTSIO_HPP
#define STRUCTSIO_HPP
#include "../db_types/debtor.hpp"
#include <boost/optional.hpp>
/**
 * @defgroup commands Commands processing
 *    This group contains module that controls database 
 *      using commands and args from TokenStreamInterface.  
 */

/**
 * @ingroup commands
 * @brief This struct is used to get already persist person.
 * @note  Minimum \b one field expected to be initialized.
 *    
 *
 * @see @ref TokenStreamInterface [read](@ref TokenStreamInterface::read(DebtorIO&)) operation.
 */
struct DebtorIO
{
  boost::optional< int > id;
  boost::optional< std::string > name;
};

/**
 * @ingroup commands
 * @brief This struct is used to get a person to be crated in table.
 * @note  Field \b name expected to be initialized.
 * @note  Field \b debt can be uninitialized.
 *
 * @see @ref TokenStreamInterface [read](@ref TokenStreamInterface::read(NewDebtorIO&)) operation.
 */
struct NewDebtorIO
{
  boost::optional< std::string > name;
  boost::optional< int > debt;
};

/**
 * @ingroup commands
 * @brief This struct is used to get next command stream.
 * @note If EOF met - field command must be uninitialized. Failbit should not be set.
 *
 * @see @ref TokenStreamInterface [read](@ref TokenStreamInterface::read(CommandIO&)) operation.
 */
struct CommandIO
{
  boost::optional< std::string > command;
};

/**
 * @ingroup commands
 * @brief This struct is used to get DebtorIO and RepeatCount for /history command.
 * @note Field num can be uninitialized.
 * @see @ref TokenStreamInterface [read](@ref TokenStreamInterface::read(DebtorNumIO&)) operation.
 */
struct DebtorNumIO
{
  DebtorIO debtor;
  boost::optional< unsigned int > num;
};

/**
 * @ingroup commands
 * @brief This struct is used to get DebtorIO, valueChange and description
 *      for addVal command. Basicly it contains operation details. 
 *      Whos debt was changed. 
 * @note Field description can be uninitialized.
 * @see @ref TokenStreamInterface [read](@ref TokenStreamInterface::read(DebtorNumIO&)) operation.
 */
struct OperationIO
{
  DebtorIO debtor;
  int valueChange;
  std::string description;
};
#endif
