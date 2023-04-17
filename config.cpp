#include "config.hpp"
#include <fstream>
#include <iostream>
#include <jsoncpp/json/json.h>

Json::Value readFromConfig()
{
  Json::Value root;
  std::ifstream ifs("config.json");
  if (!ifs.is_open())
    throw std::logic_error("Can't open the file config.json");
  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;
  if (!Json::parseFromStream(builder, ifs, &root, &errs))
  {
    std::cerr << "WARNING: Error occured while reading json: " << errs << std::endl;
  }
  return root;
}
