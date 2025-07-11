cmake_minimum_required(VERSION 3.18)

# ===============================
# Project Setup
# ===============================
project(Sidequest CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(SIDEQUEST_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/sidequest")
set(SIDEQUEST_SERVER_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/server")
set(SIDEQUEST_CLIENT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/client")
set(SIDEQUEST_DEPENDENCIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies")

# ===============================
# Setup CMake to discover sources
# ===============================
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(GetSourceFiles)
set(FILE_TYPES *.cpp *.h )

# ===============================
# Settings for all components
# ===============================
include_directories(${SIDEQUEST_INCLUDE_DIR})
include_directories(${SIDEQUEST_DEPENDENCIES_DIR})
link_directories(${SIDEQUEST_DEPENDENCIES_DIR})

# ===============================
# Component sidequest_lib
# ===============================
find_directories(LIBRARY_SOURCE_DIRS "src/sidequest" "${FILE_TYPES}")
generate_source_tree(LIBRARY_SOURCES "${LIBRARY_SOURCE_DIRS}" "${FILE_TYPES}")
add_library(sidequest_lib STATIC ${LIBRARY_SOURCES} )

# ===============================
# Component server_lib
# ===============================
find_directories(SERVER_SOURCE_DIRS "src/server" "${FILE_TYPES}")
generate_source_tree(SERVER_SOURCES "${SERVER_SOURCE_DIRS}" "${FILE_TYPES}")
list(REMOVE_ITEM SERVER_SOURCES "src/server/main.cpp")
add_library(server_lib ${SERVER_SOURCES} )

add_dependencies(server_lib sidequest_lib)

# ===============================
# Component server executable
# ===============================
add_executable(server src/server/main.cpp)

target_link_libraries(server sidequest_lib)  
target_link_libraries(server server_lib)  
target_link_libraries(server sqlite3)  

# set the application working directory
include_directories(${SIDEQUEST_SERVER_INCLUDE_DIR})
set_target_properties(server PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/application_root" )

# ===============================
# Component client_lib
# ===============================
find_directories(CLIENT_SOURCE_DIRS "src/client" "${FILE_TYPES}")
generate_source_tree(CLIENT_SOURCES "${CLIENT_SOURCE_DIRS}" "${FILE_TYPES}")
list(REMOVE_ITEM CLIENT_SOURCES "src/client/main.cpp")
add_library(client_lib ${CLIENT_SOURCES} )

add_dependencies(client_lib sidequest_lib)

# ===============================
# Component client executable
# ===============================
add_executable(client src/client/main.cpp)

target_link_libraries(client sidequest_lib)  
target_link_libraries(client client_lib)  
target_link_libraries(client sqlite3)  

# set the application working directory
include_directories(${SIDEQUEST_CLIENT_INCLUDE_DIR})
set_target_properties(client PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/application_root" )

# ===============================
# Setup Google Test
# ===============================

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

find_directories(TEST_SOURCE_DIRS "src/unit_tests" "${FILE_TYPES}")
generate_source_tree(TEST_SOURCES "${TEST_SOURCE_DIRS}" "${FILE_TYPES}")

add_executable(
  unit_tests
  ${TEST_SOURCES}
)
target_link_libraries(
  unit_tests
  GTest::gtest_main
  sidequest_lib
  server_lib
  client_lib
  sqlite3
)

add_dependencies(unit_tests sidequest_lib)
add_dependencies(unit_tests server_lib)
add_dependencies(unit_tests client_lib)

include(GoogleTest)
gtest_discover_tests(unit_tests)

# copy necessary dlls to target directory
if(MSVC)
	add_custom_command(
		TARGET server
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different 
		"${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sqlite3.dll"
		"$<TARGET_FILE_DIR:server>"
	)
	add_custom_command(
		TARGET unit_tests
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different 
		"${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sqlite3.dll"
		"$<TARGET_FILE_DIR:server>"
	)
endif(MSVC)


set_target_properties(unit_tests PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/application_root" )