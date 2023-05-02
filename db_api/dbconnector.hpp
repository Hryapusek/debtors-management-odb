#ifndef DBCONNECTOR_HPP
#define DBCONNECTOR_HPP
#include <memory>
#include <odb/database.hxx>
#include <jsoncpp/json/json.h>

/**
 * @brief Class establishe connection with database.
 *    Using Json::Value to get connection parametrs such as host, user...
 * 
 */
class DbConnector
{
public:
  /// @brief Contains database connection object. 
  ///     You better not try to use it before [initConnection()](@ref initConnection(Json::Value)) call
  static std::shared_ptr< odb::database > db;
  /**
   * @brief Must be called to establish connection with database. This method 
   *    initializing \b std::shared_ptr< \b odb::database \b > \b db 
   *    field that is needed to interract with database.
   * 
   */
  static void initConnection(Json::Value);
};

#endif