#include <memory>
#include <iostream>
#include <fstream>

#include <jsoncpp/json/json.h>

#include "config.hpp"
#include "dbconnector.hpp"

#include "db_api.hpp"

#include "commandsIO.hpp"

/*#include <odb/database.hxx>
 #include <odb/transaction.hxx>

 #include <odb/pgsql/database.hxx>

 #include "./db_types/person.hxx"
 #include "./db_types/person-odb.hxx"

   using namespace odb::core;*/

int main (int argc, char *argv[])
{
  Json::Value config = readFromConfig();
  std::cout << "user=" + config["user"].asString() + " dbname=" + config["dbname"].asString() + " password=" + config["password"].asString();
  std::cout << "\nConfig readed.\n";
  DbConnector::initConnection(config);
  std::cout << "Database initialized.\n";

  /*std::cout << "\n----------\n\n";

  {
    try
    {
      api::addDebtor({ "Zlata", 6000 });
      std::cout << "Added debtor Zlata\n";
    }
    catch (const std::exception &e)
    {
      std::cout << "Zlata alreaty in table\n";
    }
    std::cout << "\nDebtors:\n";
    auto res = api::getDebtors();
    for (const auto &p : res)
    {
      std::cout << "Name: " << p.name() << '\n'
                << "Debt: " << p.debt() << "\n\n";
    }
  }

  std::cout << "\n----------\n\n";

  try
  {
    api::removeDebtor("Zlata");
    std::cout << "Debtor Zlata was removed successfully\n";
  } catch (...)
  { }

  {
    std::cout << "\nDebtors:\n";
    auto res = api::getDebtors();
    for (const auto &p : res)
    {
      std::cout << "Name: " << p.name() << '\n'
                << "Debt: " << p.debt() << "\n\n";
    }
  }

  std::cout << "\n----------\n";

  {
    std::cout << "\nGet Debtor by name Nikita:\n";
    auto p = *api::getDebtor("Nikita");
    std::cout << "Name: " << p.name() << '\n'
              << "Debt: " << p.debt() << "\n\n";
  }

  /*{
     std::cout << "\nGet Debtor by id 21:\n";
     auto p = getDebtor(21);
     std::cout << "Name: " << p.name() << '\n'
              << "Debt: " << p.debt() << "\n\n";
     }

  {
    std::cout << "\nAdd value to Nikita:\n";
    api::addValue("Nikita", 12);
    auto p = *api::getDebtor("Nikita");
    std::cout << "Name: " << p.name() << '\n'
              << "Debt: " << p.debt() << "\n\n";
  }*/
  processInput(std::cin);
  return 0;
}

  /*try
     {
     std::unique_ptr< database > db (new odb::pgsql::database ("user=postgres dbname=testdb password=admin"));

     unsigned long john_id, jane_id, joe_id;

     // Create a few persistent person objects.
     //
     typedef odb::query< person > query;
     typedef odb::result< person > result;
     {
      person john ("John", "Doe", 33);
      person jane ("Jane", "Doe", 32);
      person joe ("Joe", "Dirt", 30);

      transaction t (db->begin ());

      // Make objects persistent and save their ids for later use.
      //
      john_id = db->persist (john);
      jane_id = db->persist (jane);
      joe_id = db->persist (joe);

      //transaction t1 (db->begin ());

      std::unique_ptr< person > joe1 (db->load< person > (joe_id));
      joe1->first ("Fluffy");
      db->update (*joe1);

      t.commit ();
     }

     // Say hello to those over 30.
     //
     {
      transaction t (db->begin ());

      result r (db->query< person > (query::age >= 30));

      for (result::iterator i (r.begin ()); i != r.end (); ++i)
      {
        std::cout << "Hello, " << i->first () << "!" << std::endl;
      }

      t.commit ();
     }
     }
     catch (const odb::exception &e)
     {
     std::cerr << e.what() << '\n';
     return 1;
     }*/