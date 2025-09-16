cmake_minimum_required(VERSION 3.15)
project(RType VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy")


option(BUILD_CLIENT "Build the client executable" ON)
option(BUILD_SERVER "Build the server executable" ON)

# Activer clang-tidy pour tous les fichiers
find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-checks=*,-clang-analyzer-alpha*")
endif()

# Activer clang-format 
find_program(CLANG_FORMAT_EXE NAMES clang-format)
if(CLANG_FORMAT_EXE)
    file(GLOB_RECURSE ALL_CXX_SOURCE_FILES "src/*.cpp" "include/*.hpp")
    add_custom_target(
        clang_format
        COMMAND ${CLANG_FORMAT_EXE} -i ${ALL_CXX_SOURCE_FILES}
        COMMENT "Formatting all source files"
    )
endif()

if(BUILD_CLIENT)
    file(GLOB_RECURSE CLIENT_SOURCES "src/r-type_client/*.cpp")
    add_executable(r-type_client ${CLIENT_SOURCES})
    target_include_directories(r-type_client PRIVATE include)
endif()

if(BUILD_SERVER)
    file(GLOB_RECURSE SERVER_SOURCES "src/r-type_server/*.cpp")
    add_executable(r-type_server ${SERVER_SOURCES})
    target_include_directories(r-type_server PRIVATE include)
endif()
