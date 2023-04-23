#include <memory>
#include <iostream>
#include <fstream>

#include <jsoncpp/json/json.h>

#include "config.hpp"
#include "dbconnector.hpp"

#include "commandsIO.hpp"

int main (int argc, char *argv[])
{
  Json::Value config = readFromConfig();
  std::cout << "user=" + config["user"].asString() + " dbname=" + config["dbname"].asString() + " password=" + config["password"].asString();
  std::cout << "\nConfig readed.\n";
  DbConnector::initConnection(config);
  std::cout << "Database initialized.\n";
  processInput(std::cin);
  return 0;
}
