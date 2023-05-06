#include "commandsIO.hpp"

#include <iostream>
#include <vector>
#include <map>

#include <boost/regex.hpp>
#include <odb/exceptions.hxx>

#include "../db_types/debtor.hpp"
#include "../db_types/operation.hpp"
#include "../db_api/db_api.hpp"
#include "tabulate.hpp"

using str = std::string;

namespace helpersIO
{
  //Name or id must be initialized or nothing will happen.
  boost::optional< Debtor > getDebtor(DebtorIO &dIO)
  {
    if (dIO.id)
      return api::getDebtor(*dIO.id);
    else if (dIO.name)
      return api::getDebtor(*dIO.name);
    else
      return boost::none;
  }

  //Name or id must be initialized or nothing will happen.
  void addValue(OperationIO &opIO)
  {
    if (opIO.debtor.name)
      api::addValue(*opIO.debtor.name, opIO.valueChange, opIO.description);
    else if (opIO.debtor.id)
      api::addValue(*opIO.debtor.id, opIO.valueChange, opIO.description);
  }

  //Name or id must be initialized or nothing will happen.
  void removeDebtor(DebtorIO &dIO)
  {
    if (dIO.name)
      api::removeDebtor(*dIO.name);
    else if (dIO.id)
      api::removeDebtor(*dIO.id);
  }

  //Name or id must be initialized or nothing will happen.
  void rollback(DebtorIO &dIO)
  {
    if (dIO.name)
      api::rollback(*dIO.name);
    else if (dIO.id)
      api::rollback(*dIO.id);
  }

  std::vector< Operation > getOperations(DebtorIO &dIO)
  {
    if (dIO.name)
      return api::getOperations(*dIO.name);
    else if (dIO.id)
      return api::getOperations(*dIO.id);
    else
      return std::vector< Operation >();
  }
}

namespace
{
  struct ToLower
  {
    unsigned char operator()(unsigned char c)
    {
      return std::tolower(c);
    }
  };

  std::string &to_lower(std::string &str)
  {
    std::transform(str.begin(), str.end(), str.begin(), ToLower{ });
    return str;
  }

  std::string to_string(DebtorIO dIO)
  {
    std::string res;
    if (dIO.id)
      res += std::to_string(*dIO.id);
    if (dIO.name)
      res += ' ' + *dIO.name;
    return res;
  }

  void printErrorReadingDebtor(std::ostream &out)
  {
    out << "Error occured while trying to read next debtor.\nSkipping the rest...\n";
  }

  void printDebtorNotFoundWarning(const str &name, std::ostream &out)
  {
    out << "WARNING: Debtor with name " << name << " was not found" << '\n';
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

  void getDebtorsFromStream(tabulate::Table &debtorsTable, TokenStreamInterface &ts, std::ostream &out)
  {
    while (!ts.eof() && !ts.eol())
    {
      DebtorIO dIO;
      ts.read(dIO);
      if (ts.fail())
      {
        printErrorReadingDebtor(out);
        break;
      }
      boost::optional< Debtor > res = helpersIO::getDebtor(dIO);
      if (res)
        debtorsTable.add_row({ std::to_string(res->id()), res->name(), std::to_string(res->debt()) });
      else
        printDebtorNotFoundWarning(to_string(dIO), out);
    }
  }

  void help(std::ostream &out)
  {
    out << "\n------------\n";
    out << "Debtors management by hryapusek.";
    out << "\n------------\n\n";
    out << "showAll - lists all debtors.\n";
    out << "show Debtor Debtor ... - prints information about debtors.\n";
    out << "new Debtor Debtor ... - creates new debtor.\n";
    out << "addVal Operation Operation ... - adds values to Debtors, could be negative.\n";
    out << "rm Debtor Debtor ... - removes debtors from table.\n";
    out << "rollback Debtor Debtor ... - rollback last operation on users.\n";
    out << "hist {Debtor, [n]} ... - show history of last n operations. Default n = 5\n";
    out << "\n------------\n";
    out << "\nExamples:\n";
    out << "/addVal {{21}, 100, \"Added 100 to user with id 21.\"}\n";
    out << "/show {21} {\"Lenya\"}\n";
    out << "/rollback {21} {\"Lenya\"}\n";
    out << "/hist {{21}} {{\"Lenya\"}, 15}\n";
    out << "\n------------\n";
    out << "\n\tNOTE:\n";
    out << "\tDebtor - {id, \"name\"}.\n";
    out << "\tOperation - {Debtor, value, [\"description\"]}.\n";
    out << "\tId or name can be ommited in some commands.\n\n";
  }

  void showAll(TokenStreamInterface &ts, std::ostream &out)
  {
    tabulate::Table debtorsTable;
    debtorsTable.add_row({ "ID", "Debtor", "Value" });
    getAllDebtors(debtorsTable);
    out << debtorsTable << '\n';
  }

  void show(TokenStreamInterface &ts, std::ostream &out)
  {
    tabulate::Table debtorsTable;
    debtorsTable.add_row({ "ID", "Debtor", "Value" });
    getDebtorsFromStream(debtorsTable, ts, out);
    out << debtorsTable << '\n';
  }

  void newDebtor(TokenStreamInterface &ts, std::ostream &out)
  {
    NewDebtorIO dIO;
    try
    {
      while (!ts.eof() && !ts.eol())
      {
        ts.read(dIO);
        if (ts.fail())
        {
          printErrorReadingDebtor(out);
          break;
        }
        api::addDebtor({ *dIO.name, dIO.debt ? *dIO.debt : 0 });
        out << "Debtor with name " << *dIO.name << " added successfully!\n";
      }
    }
    catch (const odb::object_already_persistent &e)
    {
      std::cerr << "WANING: Debtor with name " << *dIO.name << " already exists!\n";
    }
  }

  void addValue(TokenStreamInterface &ts, std::ostream &out)
  { //value expected
    OperationIO opIO;
    while (!ts.eof() && !ts.eol())
    {
      ts.read(opIO);
      if (ts.fail())
      {
        printErrorReadingDebtor(out);
        break;
      }
      try
      {
        helpersIO::addValue(opIO);
        out << "Value " << opIO.valueChange << " successfully added to ";
        out << to_string(opIO.debtor) << '\n';
      }
      catch (const api::debtor_not_found &e)
      {
        printDebtorNotFoundWarning(to_string(opIO.debtor), out);
      }
    }
  }

  void removeDebtor(TokenStreamInterface &ts, std::ostream &out)
  {
    DebtorIO dIO;
    while (!ts.eof() && !ts.eol())
    {
      ts.read(dIO);
      if (ts.fail())
      {
        printErrorReadingDebtor(out);
        break;
      }
      try
      {
        helpersIO::removeDebtor(dIO);
        out << to_string(dIO) << " removed successfully\n";
      }
      catch (const api::debtor_not_found &e)
      {
        printDebtorNotFoundWarning(to_string(dIO), out);
      }
    }
  }

  void rollback(TokenStreamInterface &ts, std::ostream &out)
  {
    DebtorIO dIO;
    while (!ts.eof() && !ts.eol())
    {
      ts.read(dIO);
      if (ts.fail())
      {
        printErrorReadingDebtor(out);
        break;
      }
      try
      {
        helpersIO::rollback(dIO);
        out << "Successfully rollbacked " << to_string(dIO) << "\n";
      }
      catch (const api::debtor_not_found &e)
      {
        printDebtorNotFoundWarning(to_string(dIO), out);
      }
    }
  }

  void history(TokenStreamInterface &ts, std::ostream &out)
  {
    DebtorNumIO dIO;
    tabulate::Table operationsTable;
    operationsTable.add_row({ "Debtor", "Value change", "Timestamp", "Description" });
    while (!ts.eof() && !ts.eol())
    {
      ts.read(dIO);
      if (ts.fail())
      {
        printErrorReadingDebtor(out);
        break;
      }
      try
      {
        std::vector< Operation > operations = helpersIO::getOperations(dIO.debtor);
        unsigned int nOperations = dIO.num ? *dIO.num : 5;
        for (auto it = operations.rbegin(); it != operations.rend() && nOperations > 0; ++it, --nOperations)
        {
          time_t t = it->getTime();
          operationsTable.add_row({ to_string(dIO.debtor), std::to_string(it->valueChange()), ctime(std::addressof(t)), it->description() });
        }
      }
      catch (const api::debtor_not_found &e)
      {
        printDebtorNotFoundWarning(to_string(dIO.debtor), out);
        continue;
      }
    }
    out << operationsTable << '\n';
  }
}

void processInput(TokenStreamInterface &ts, std::ostream &out)
{
  std::map< std::string, std::function< void() > > commandFunc {
    { "show", std::bind(show, std::ref(ts), std::ref(out)) },
    { "showall", std::bind(showAll, std::ref(ts), std::ref(out)) },
    { "new", std::bind(newDebtor, std::ref(ts), std::ref(out)) },
    { "help", std::bind(help, std::ref(out)) },
    { "rm", std::bind(removeDebtor, std::ref(ts), std::ref(out)) },
    { "addval", std::bind(addValue, std::ref(ts), std::ref(out)) },
    { "hist", std::bind(history, std::ref(ts), std::ref(out)) },
    { "rollback", std::bind(rollback, std::ref(ts), std::ref(out)) },
  };
  help(out);
  while (!ts.eof())
  {
    CommandIO cmdIO;
    out << "\n-> ";
    ts.read(cmdIO);
    if (!cmdIO.command)
      break;
    out << '\n';
    try
    {
      commandFunc.at(to_lower(*cmdIO.command))();
    }
    catch (const std::out_of_range &e)
    {
      out << "Unrecognized command. Skipping line.\n";
      ts.clear();
      ts.skipCurrentCommand();
    }
  }
}