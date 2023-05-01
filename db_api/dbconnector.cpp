#include "dbconnector.hpp"
#include <odb/pgsql/database.hxx>
#include <string>

std::shared_ptr< odb::database > DbConnector::db;

void DbConnector::initConnection(Json::Value config)
{
  DbConnector::db = std::make_shared< odb::pgsql::database >("user=" + config["user"].asString() + " dbname=" + config["dbname"].asString() + " password=" + config["password"].asString());
}
