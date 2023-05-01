# debtors-managment-cpp-prog

## About program
If you have a lot of your friends who owe you some money, and you want to manipulate them easily and dont want to spend a lot of time on installation some hard-weight program - this one exactly for you. You can create debtors, add values to their debts, check perfomed operations and more cool things.<br /> <br /> 
It is console program that uses ORM c++ library ODB and PostgreSQL database.
This program helps to manipulate debtors in database tables. You can create people, change their debts, see history of changes, rollback operations if was mistaken.<br />
1. You can create people using `new`<br /> 
- `new {"Rabbit", 200}` Person with name "Rabbit" and debt 200 created.<br />
2. You can add value to the person using `addVal`<br />
- `addVal {{"Rabbit"}, 20, "Added 20"}` Now "Rabbit" has 220. Operation table contains operation with description "Added 20".<br />
- `addVal {{RabbitID}, -20}` Again "Rabbit" has 200. Operation table contains operation without description.<br />
3. You can rollback last perfomed operation on person using `rollback`<br />
- `addVal {{"Rabbit"}, 150, "Added 150"}` or `addVal {{RabbitID}, 150}` Now "Rabbit" has 350.<br />
- `rollback {"Rabbit"}` Again "Rabbit" has 200.<br />
4. You can check operations history of your person using `hist`<br />
- `hist {{"Rabbit"}, 5}` or `hist {{RabbitID}}` Prints table with last 5 perfomed operations. 5 is default value.<br /><br />
You can display all people using `showall` and some of them using `show {"Name1"} {"Name2"} {ID_3}`.

### Note
You can always type `help` to display all commands.<br />
You dont have to put '/' in the command beginning.<br />
Commands are not case sensetive.<br />
If you have any questions about this one - vk.com/ezhiklisiy.<br />

## Libraries
* boost
* [jsoncpp](https://github.com/open-source-parsers/jsoncpp/)
* [odb](https://www.codesynthesis.com/products/odb/)

## Compile and run

### For Linux only.

Firstly, you go to **db_types/** and use `make` in here. Two `.sql` files and some other will be generated. You have to create tables respectively to this files. Next you specify in config.json file your database information.<br /><br />
Secondly, go to root directory and `make` here. Binary file with name **main** will be generated if success.
Now you can start it.

## About code
Program splitted into 3 modules:
- Database api
- Commands handler
- Input Output operations

## Note
It will instantly break down with exception if connection to database can not be established.
