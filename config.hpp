#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <string>
#include <map>
#include <jsoncpp/json/json.h>

using str = std::string;
Json::Value readFromConfig();

#endif