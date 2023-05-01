#ifndef COMMANDSIO_HPP
#define COMMANDSIO_HPP
#include "token_stream_interface.hpp"
#include <iosfwd>

void processInput(TokenStreamInterface &ts, std::ostream &out);

#endif
