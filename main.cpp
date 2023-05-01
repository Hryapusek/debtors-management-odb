#include <memory>
#include <iostream>
#include <fstream>

#include <jsoncpp/json/json.h>

#include "config/config.hpp"
#include "db_api/dbconnector.hpp"

#include "commandsIO/commandsIO.hpp"
#include "ioctl/token_stream.hpp"

int main (int argc, char *argv[])
{
  Json::Value config = readFromConfig();
  std::cout << "user=" + config["user"].asString() + " dbname=" + config["dbname"].asString() + " password=" + config["password"].asString();
  std::cout << "\nConfig readed.\n";
  DbConnector::initConnection(config);
  std::cout << "Database initialized.\n";
  TokenStream ts(std::cin);
  processInput(ts, std::cout);
  return 0;
}
