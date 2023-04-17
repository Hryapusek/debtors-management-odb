#include "commandsIO.hpp"
#include "db_types/debtor.hpp"
#include "db_api.hpp"
#include <iostream>
#include <vector>
#include <iterator>
#include <boost/regex.hpp>
#include <odb/exceptions.hxx>

using str = std::string;

namespace
{
  bool isNum(const std::string &num)
  {
    try
    {
      std::stoi(num);
      return true;
    }
    catch (const std::exception &e)
    {
      return false;
    }
  }

  bool checkIfNum(const std::string &num, const std::string &intro = "")
  {
    if (!isNum(num))
    {
      std::cerr << intro << ": Invalid number\n";
      std::cerr << "Number: " << num << '\n';
      return false;
    }
    return true;
  }

  bool isName(const std::string &name)
  {
    boost::regex ex("[a-zA-Zа-яА-Я]{2,20}");
    return boost::regex_match(name, ex);
  }

  bool checkIfName(const std::string &name, const std::string &intro = "")
  {
    if (!isName(name))
    {
      std::cerr << intro << ": Invalid name of a debtor\n";
      std::cerr << "Name: " << name << '\n';
      return false;
    }
    return true;
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
    std::string tempCommand;
    in >> tempCommand;
    if (in.fail())
      return false;
    if (tempCommand[0] == '/')
      tempCommand = tempCommand.substr(1);
    auto tempArgs = std::vector< str >();
    {
      skipWs(in);
      std::string word;
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
    std::cout << "new :name: [:surname:] [:debtValue:] - creates new debtor\n";
    std::cout << "add :name: :valueChange: - adds value to Debtor's, could be negative\n";
    std::cout << "rm :name: :name: ... - removes debtors from table\n";
    std::cout << "NOTE: use first\\ last to remove double word names\n";
  }

  void show(const std::vector< str > &args, std::ostream &out = std::cout)
  {
    if (args.size() == 0)
    {
      showAll();
      return;
    }
    for (auto it = args.begin(); it != args.end(); ++it)
    {
      auto res = api::getDebtor(*it);
      if (res)
        out << *res << '\n';
      else
        std::cerr << "Debtor with name " << *it << " was not found" << '\n';
    }
  }

  void newDebtor(const std::vector< str > &args)
  {
    //size == 1 -> debt(0) & name(args[0])
    //size == 2 -> debt(args[1] or 0) & name(args[0] || args[0] + " " + args[1])
    //size == 3 -> debt(args[3]) & name(args[0] + " " + args[1])
    if (args.size() > 3 || args.size() == 0)
    {
      std::cerr << "Invalid number of arguments to create a debtor\n";
      std::cerr << "Args: ";
      std::copy(args.begin(), args.end(), std::ostream_iterator< str >(std::cerr, " "));
      std::cerr << '\n';
      return;
    }
    Debtor d;
    if (!checkIfName(args[0]))
      return;
    switch (args.size())
    {
    case 1:
    {
      d.name(args[0]);
      d.debt(0);
      break;
    }

    case 2:
    {
      if (isName(args[1]))
      {
        d.name(args[0] + " " + args[1]);
        d.debt(0);
      }
      else
      {
        d.name(args[0]);
        if (!checkIfNum(args[1], "Invalid debtor's value"))
          return;
        d.debt(std::stoi(args[1]));
      }
      break;
    }

    case 3:
    {
      if (!checkIfName(args[1]))
        return;
      d.name(args[0] + " " + args[1]);
      if (!checkIfNum(args[2], "Invalid debtor's value"))
        return;
      d.debt(std::stoi(args[2]));
      break;
    }
    }
    try
    {
      api::addDebtor(d);
    }
    catch (const odb::database_exception &e)
    {
      std::cerr << "Debtor with name " << d.name() << " already exists!\n";
    }
  }

  void addValue(const std::vector< str > &args)
  {
    if (args.size() != 2)
    {
      std::cerr << "Invalid arguments count for addValue command\n";
      return;
    }
    checkIfName(args[0], "addValue - invalid name");
    checkIfNum(args[1], "addValue - invalid value");
    try
    {
      api::addValue(args[0], std::stoi(args[1]));
      std::cout << "Added " << args[1] << " to " << args[0] << '\n';
    }
    catch (const odb::object_not_persistent &e)
    {
      std::cerr << "Debtor with name " << args[0] << " was not found\n";
    }
  }

  void removeDebtor(const std::vector< str > &args)
  {
    std::string doubleWordName;
    bool addPrevWord = false;
    for (auto it = args.begin(); it != args.end(); ++it)
    {
      std::reference_wrapper< const std::string > res = *it;
      if (addPrevWord)
      {
        doubleWordName += " " + *it;
        res = doubleWordName;
        addPrevWord = false;
      }
      else if (it->back() == '\\')
      {
        doubleWordName = it->substr(0, it->size() - 1);
        addPrevWord = true;
        continue;
      }
      try
      {
        api::removeDebtor(res.get());
        std::cout << res.get() << " removed successfully\n";
      }
      catch (const std::logic_error &e)
      {
        std::cerr << "Debtor with name " << res.get() << " was not found" << '\n';
      }
    }
  }
}


void processInput(std::istream &in)
{
  while (in && !in.eof())
  {
    str cmd;
    std::vector< str > args;
    std::cout << "\n-> ";
    while (getCommandAndArgs(in, cmd, args))
    {
      /*std::cout << "Command: " << cmd << '\n';
         std::cout << "Args: ";
         std::copy(args.begin(), args.end(), std::ostream_iterator< str >(std::cout, " "));
       */
      std::cout << '\n';
      if (cmd == "show")
        show(args);
      else if (cmd == "new")
        newDebtor(args);
      else if (cmd == "help")
        help();
      else if (cmd == "rm")
        removeDebtor(args);
      else if (cmd == "add")
        addValue(args);
      std::cout << "\n-> ";
    }
  }
}