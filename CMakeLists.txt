cmake_minimum_required(VERSION 3.0)
set(TARGET DEBTORS_MANAGEMENT)
project(DEBTORS_MANAGEMENT)
add_executable(${TARGET} main.cpp)
set(CMAKE_CXX_STANDARD C++14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
#-lodb-pgsql -lodb -ljsoncpp -lboost_regex
find_library(ODB_PGSQL odb-pgsql REQUIRED)
find_library(ODB odb REQUIRED)
find_library(JSONCPP jsoncpp REQUIRED)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

find_package(Boost 1.45.0 COMPONENTS regex) 
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found!!!")
endif()

set(SOURCE_FILES_LIST 
    commandsIO/commandsIO.cpp 
    config/config.cpp 
    db_api/db_api.cpp 
    db_api/dbconnector.cpp 
    db_types/debtor.cpp 
    db_types/operation.cpp
    db_types/debtor-odb.cxx
    db_types/operation-odb.cxx
    ioctl/token_stream.cpp
    ioctl/private/ioctl.cpp
    ${CMAKE_BINARY_DIR}/db_types/debtor-odb.cxx
    ${CMAKE_BINARY_DIR}/db_types/operation-odb.cxx
    )
execute_process(COMMAND odb --std c++14 -d pgsql --generate-query --generate-schema ${CMAKE_SOURCE_DIR}/db_types/debtor.hpp ${CMAKE_SOURCE_DIR}/db_types/operation.hpp WORKING_DIRECTORY db_types OUTPUT_VARIABLE OUT ERROR_VARIABLE OUT)
message("${OUT}")
target_include_directories(${TARGET} PRIVATE ioctl db_types db_api config commandsIO ${CMAKE_BINARY_DIR}/db_types)
target_sources(${TARGET} PRIVATE ${SOURCE_FILES_LIST})
include_directories(${Boost_INCLUDE_DIRS}) 
target_link_libraries(${TARGET} ${Boost_LIBRARIES})
target_link_libraries(${TARGET} ${ODB_PGSQL} ${ODB} ${JSONCPP})