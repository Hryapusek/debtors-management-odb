#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <string>
#include <map>
#include <jsoncpp/json/json.h>

using str = std::string;
/**
 * @brief Reads values from file config.json.
 *    File should be in executing directory.
 * 
 * @return Json::Value that contains config fields.
 */
Json::Value readFromConfig();

#endif