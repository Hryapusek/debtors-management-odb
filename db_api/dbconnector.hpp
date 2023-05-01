#ifndef DBCONNECTOR_HPP
#define DBCONNECTOR_HPP
#include <memory>
#include <odb/database.hxx>
#include <jsoncpp/json/json.h>

class DbConnector
{
public:
  static std::shared_ptr< odb::database > db;
  static void initConnection(Json::Value);
};

#endif