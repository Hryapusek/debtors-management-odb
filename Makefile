CXXFLAGS = --std=c++14
LDFLAGS = -lodb-pgsql -lodb -ljsoncpp -lboost_regex
OBJECT = main.o ioctl/token_stream.o ioctl/private/ioctl.o db_api/dbconnector.o config/config.o db_api/db_api.o commandsIO/commandsIO.o db_types/debtor-odb.o db_types/operation-odb.o db_types/debtor.o db_types/operation.o
TARGET = main
CC = g++

all: rec_make $(OBJECT)
	g++ -o $(TARGET) $(OBJECT) $(LDFLAGS)

rec_make:
	cd db_types && $(MAKE)

clean: $(wildcard *.o) $(wildcard db_types/*.o)
	rm -f $(wildcard *.o) $(wildcard db_types/*.o)
	rm -f $(wildcard db_types/*.gch)
	rm -f $(wildcard db_types/*-odb.?xx)
	rm -f $(wildcard db_types/*.sql)
	rm -f $(TARGET)