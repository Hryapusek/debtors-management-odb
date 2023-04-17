all: debtor-odb.o debtor.o operation-odb.o

operation-odb.o: operation-odb.cxx
	g++ -c operation-odb.cxx

debtor-odb.o: debtor-odb.cxx
	g++ -c debtor-odb.cxx

debtor.o: debtor.cpp
	g++ -c debtor.cpp

operation.o: operation.cpp
	g++ -c operation.cpp

debtor-odb.cxx operation-odb.cxx: debtor.hpp operation.hpp
	odb --std c++14 -d pgsql --generate-query --generate-schema $^