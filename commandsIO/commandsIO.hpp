#ifndef COMMANDSIO_HPP
#define COMMANDSIO_HPP
#include "token_stream_interface.hpp"
#include <iosfwd>

/**
 * @ingroup commands
 * @brief This function works with given TokenStream and processes
 *    given through it commands and args.
 * 
 * @details This function abstracts performing operations in database.
 * 
 * @param[in] ts @ref TokenStreamInterface object used to extract commands and args from.
 * @param[in] out std::ostream object that will get replies from commands.
 */
void processInput(TokenStreamInterface &ts, std::ostream &out);

#endif
