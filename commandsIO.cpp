#include "commandsIO.hpp"
#include "db_types/debtor.hpp"
#include "db_api.hpp"
#include "token_stream.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <iterator>
#include <boost/regex.hpp>
#include <odb/exceptions.hxx>

using str = std::string;

namespace
{
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

  //Skips isspace(in.peek()) but new line
  void skipWs(std::istream &in)
  {
    while (!in.eof() && in.peek() != '\n' && isspace(in.peek()))
      in.ignore(1);
  }

  bool getCommandAndArgs(std::istream &in, str &command, std::vector< str > &args)
  {
    if (in.eof())
      return false;
    str tempCommand;
    in >> tempCommand;
    if (in.fail())
      return false;
    if (tempCommand[0] == '/')
      tempCommand = tempCommand.substr(1);
    auto tempArgs = std::vector< str >();
    {
      skipWs(in);
      str word;
      while (!in.eof() && in.peek() != '\n')
      {
        in >> word;
        skipWs(in);
        tempArgs.push_back(word);
      }
    }
    args = std::move(tempArgs);
    command = std::move(tempCommand);
    return true;
  }

  void showAll()
  {
    auto res = api::getDebtors();
    for (const auto &p : res)
    {
      std::cout << p;
      if (&p != &res.back())
        std::cout << '\n';
    }
  }

  void help()
  {
    std::cout << "show - lists all debtors\n";
    std::cout << "show :name: :name: ... - prints information about debtors\n";
    std::cout << "new :name: [:debtValue:] ... - creates new debtor\n";
    std::cout << "addVal :name: :valueChange: ... - adds values to Debtors, could be negative\n";
    std::cout << "rm :name: :name: ... - removes debtors from table\n";
    std::cout << "rollback :name: :name: ... - rollback last operation on users\n";
    std::cout << "hist :name: [:n:] :name: ... - show history of last n operations. Default 5\n";
    std::cout << "NOTE use first\\ last to pass multiword names\n";
  }

  void show(const std::vector< str > &args, std::ostream &out = std::cout)
  {
    if (args.size() == 0)
    {
      showAll();
      return;
    }
    TokenStream ts(args);
    while (!ts.eof())
    {
      std::string name = ts.readName();
      if (ts.fail())
      {
        std::cerr << "Name " << name << " " << *ts.base() << " is incorrect\n";
        break;
      }
      auto res = api::getDebtor(name);
      if (res)
        out << *res << '\n';
      else
        printDebtorNotFoundWarning(name);
    }
  }

  void newDebtor(const std::vector< str > &args)
  {
    Debtor d;
    TokenStream ts(args);
    try
    {
      while (ts)
      {
        d = ts.readDebtor();
        if (ts.fail())
        {
          std::cerr << "Unexpected token \"" << *ts.base() << "\"\n";
          break;
        }
        api::addDebtor(d);
        std::cout << "Debtor with name " << d.name() << " added successfully!\n";
      }
    }
    catch (const odb::database_exception &e)
    {
      std::cerr << "WANING: Debtor with name " << d.name() << " already exists!\n";
    }
  }

  void addValue(const std::vector< str > &args)
  {
    Debtor d;
    TokenStream ts(args);
    while (ts)
    {
      d = ts.readDebtor();
      if (ts.fail())
      {
        std::cerr << "Unexpected token \"" << *ts.base() << "\"\n";
        break;
      }
      try
      {
        api::addValue(d.name(), d.debt());
        std::cout << "Added " << d.debt() << " to " << d.name() << " successfully" << '\n';
      }
      catch (const std::logic_error &e)
      {
        printDebtorNotFoundWarning(d.name());
      }
    }
  }

  void removeDebtor(const std::vector< str > &args)
  {
    Debtor d;
    TokenStream ts(args);
    while (ts)
    {
      d = ts.readDebtor();
      if (ts.fail())
      {
        std::cerr << "Unexpected token \"" << *ts.base() << "\"\n";
        break;
      }
      try
      {
        api::removeDebtor(d.name());
        std::cout << d.name() << " removed successfully\n";
      }
      catch (const std::logic_error &e)
      {
        printDebtorNotFoundWarning(d.name());
      }
    }
  }

  void rollback(const std::vector< str > &args)
  {
    Debtor d;
    TokenStream ts(args);
    while (ts)
    {
      d = ts.readDebtor();
      if (ts.fail())
      {
        std::cerr << "Unexpected token \"" << *ts.base() << "\"\n";
        break;
      }
      try
      {
        api::rollback(d.name());
      }
      catch (const std::logic_error &e)
      {
        printDebtorNotFoundWarning(d.name());
      }
    }
  }

  void history(const std::vector< str > &args)
  {
    Debtor d{ "", 0 };
    TokenStream ts(args);
    while (ts)
    {
      d = ts.readDebtor();
      if (ts.fail())
      {
        std::cerr << "Unexpected token \"" << *ts.base() << "\"\n";
        break;
      }
      int nOperations = d.debt() <= 0 ? 5 : d.debt();
      try
      {
        const auto &operations = api::getOperations(d.name());
        for (auto it = operations.rbegin(); it != operations.rend() && nOperations > 0; ++it, --nOperations)
        {
          time_t t = it->getTime();
          std::cout << "Value change: " << std::setw(8) << it->valueChange() << ", Timestamp: " << std::setw(24) << ctime(std::addressof(t));
        }
      }
      catch (const std::logic_error &e)
      {
        printDebtorNotFoundWarning(d.name());
        continue;
      }
    }
  }
}


void processInput(std::istream &in)
{
  help();
  std::cin.exceptions(std::cin.exceptions() | std::ios_base::badbit);
  while (in && !in.eof())
  {
    str cmd;
    std::vector< str > args;
    std::cout << "\n-> ";
    while (getCommandAndArgs(in, cmd, args))
    {
      std::cout << '\n';
      if (cmd == "show")
        show(args);
      else if (cmd == "new")
        newDebtor(args);
      else if (cmd == "help")
        help();
      else if (cmd == "rm")
        removeDebtor(args);
      else if (cmd == "addVal")
        addValue(args);
      else if (cmd == "rollback")
        rollback(args);
      else if (cmd == "history")
        history(args);
      std::cout << "\n-> ";
    }
  }
}