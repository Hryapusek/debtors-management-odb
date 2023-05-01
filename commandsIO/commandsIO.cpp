#include "commandsIO.hpp"
#include "db_types/debtor.hpp"
#include "db_api.hpp"
#include "token_stream.hpp"
#include "tabulate.hpp"
#include "ioctl.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <iterator>
#include <boost/regex.hpp>
#include <odb/exceptions.hxx>

using str = std::string;

namespace
{
  void printErrorReadingDebtor(TokenStream &ts)
  {
    std::cerr << "Error occured while trying to read next debtor.\nSkipping the rest...\n";
  }

  void printDebtorNotFoundWarning(const str &name)
  {
    std::cerr << "WARNING: Debtor with name " << name << " was not found" << '\n';
  }

  std::ostream &operator<<(std::ostream &out, Debtor d)
  {
    out << "Debtor's name: " << d.name() << '\n';
    out << "Debt: " << d.debt() << '\n';
    return out;
  }

  void getAllDebtors(tabulate::Table &debtorsTable)
  {
    auto res = api::getDebtors();
    for (const auto &p : res)
    {
      debtorsTable.add_row({ std::to_string(p.id()), p.name(), std::to_string(p.debt()) });
    }
  }

  void getDebtorsFromStream(tabulate::Table &debtorsTable, TokenStream &ts)
  {
    while (ts)
    {
      DebtorIO dIO;
      ts >> dIO;
      if (ts.fail())
      {
        printErrorReadingDebtor(ts);
        break;
      }
      boost::optional< Debtor > res = helpersIO::getDebtor(dIO);
      if (res)
        debtorsTable.add_row({ std::to_string(res->id()), res->name(), std::to_string(res->debt()) });
      else
        printDebtorNotFoundWarning(dIO->name());
    }
  }

  void help()
  {
    std::cout << "\tNOTE:\n\tOperation - {id, \"name\", value, [\"description\"]}\n";
    std::cout << "\tDebtor - {id, \"name\", debt}\n";
    std::cout << "\tid or name can be ommited in some commands\n\n";
    std::cout << "showAll - lists all debtors\n";
    std::cout << "show Debtor Debtor ... - prints information about debtors\n";
    std::cout << "new Debtor Debtor ... - creates new debtor\n";
    std::cout << "addVal Operation Operation ... - adds values to Debtors, could be negative\n";
    std::cout << "rm Debtor Debtor ... - removes debtors from table\n";
    std::cout << "rollback Debtor Debtor ... - rollback last operation on users\n";
    std::cout << "hist Debtor Debtor ... - show history of last n operations. Debt is considered as n. Default n is 5\n";
  }

  void showAll(TokenStream &ts, std::ostream &out = std::cout)
  {
    tabulate::Table debtorsTable;
    debtorsTable.add_row({ "ID", "Debtor", "Value" });
    getAllDebtors(debtorsTable);
    out << debtorsTable << '\n';
  }

  void show(TokenStream &ts, std::ostream &out = std::cout)
  {
    tabulate::Table debtorsTable;
    debtorsTable.add_row({ "ID", "Debtor", "Value" });
    getDebtorsFromStream(debtorsTable, ts);
    out << debtorsTable << '\n';
  }

  void newDebtor(TokenStream &ts)
  {
    DebtorIO dIO;
    try
    {
      while (ts)
      {
        ts >> dIO;
        if (ts.fail() || !(dIO.readFields & dIO.NAME))
        {
          printErrorReadingDebtor(ts);
          break;
        }
        api::addDebtor(dIO.debtor);
        std::cout << "Debtor with name " << dIO->name() << " added successfully!\n";
      }
    }
    catch (const odb::database_exception &e)
    {
      std::cerr << "WANING: Debtor with name " << dIO->name() << " already exists!\n";
    }
  }

  void addValue(TokenStream &ts)
  { //value expected
    OperationIO opIO;
    while (ts)
    {
      ts >> opIO;
      if (ts.fail())
      {
        printErrorReadingDebtor(ts);
        break;
      }
      try
      {
        helpersIO::addValue(opIO);
        std::cout << "Value " << opIO.valueChange << " successfully added to "
                  << opIO.id << " " << opIO.name << '\n';
      }
      catch (const api::debtor_not_found &e)
      {
        printDebtorNotFoundWarning(opIO.name);
      }
    }
  }

  void removeDebtor(TokenStream &ts)
  {
    DebtorIO dIO;
    while (ts)
    {
      ts >> dIO;
      if (ts.fail())
      {
        printErrorReadingDebtor(ts);
        break;
      }
      try
      {
        helpersIO::removeDebtor(dIO);
        std::cout << dIO->id() << " " << dIO->name() << " removed successfully\n";
      }
      catch (const api::debtor_not_found &e)
      {
        printDebtorNotFoundWarning(dIO->name());
      }
    }
  }

  void rollback(TokenStream &ts)
  {
    DebtorIO dIO;
    while (ts)
    {
      ts >> dIO;
      if (ts.fail())
      {
        printErrorReadingDebtor(ts);
        break;
      }
      try
      {
        helpersIO::rollback(dIO);
        std::cout << "Successfully rollbacked " << dIO->id() << " " << dIO->name() << "\n";
      }
      catch (const api::debtor_not_found &e)
      {
        printDebtorNotFoundWarning(dIO->name());
      }
    }
  }

  void history(TokenStream &ts)
  {
    Debtor d;
    DebtorIO dIO{ d };
    tabulate::Table operationsTable;
    operationsTable.add_row({ "Debtor", "Value change", "Timestamp", "Description" });
    while (ts)
    {
      ts >> dIO;
      if (ts.fail())
      {
        printErrorReadingDebtor(ts);
        break;
      }
      int nOperations = dIO->debt() <= 0 ? 5 : dIO->debt();
      try
      {
        std::vector< Operation > operations = helpersIO::getOperations(dIO);
        for (auto it = operations.rbegin(); it != operations.rend() && nOperations > 0; ++it, --nOperations)
        {
          time_t t = it->getTime();
          operationsTable.add_row({ dIO.readFields & dIO.NAME ? dIO->name() : std::to_string(dIO->id()), std::to_string(it->valueChange()), ctime(std::addressof(t)), it->description() });
        }
      }
      catch (const api::debtor_not_found &e)
      {
        printDebtorNotFoundWarning(dIO->name());
        continue;
      }
    }
    std::cout << operationsTable << '\n';
  }
}

void processInput(std::istream &in)
{
  help();
  in.exceptions(std::cin.exceptions() | std::ios_base::badbit);
  TokenStream ts(in);
  while (in && !in.eof())
  {
    str cmd;
    std::cout << "\n-> ";
    ts >> CommandIO{ cmd };
    if (ts.fail() && ts.eof())
    {
      break;
    }
    std::cout << '\n';
    if (cmd == "show")
      show(ts);
    else if (cmd == "showAll")
      showAll(ts);
    else if (cmd == "new")
      newDebtor(ts);
    else if (cmd == "help")
      help();
    else if (cmd == "rm")
      removeDebtor(ts);
    else if (cmd == "addVal")
      addValue(ts);
    else if (cmd == "rollback")
      rollback(ts);
    else if (cmd == "hist")
      history(ts);
    if (ts.fail())
    {
      ts.clear();
      ts.skipRestLine();
    }
  }
}