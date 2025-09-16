cmake_minimum_required(VERSION 3.15)
project(RType VERSION 1.0 LANGUAGES CXX)

# -------------------------------------------------------------
# Standard C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX AND NOT APPLE)
    message(STATUS "Linux detected: make sure X11 libs are installed")
elseif(WIN32)
    message(STATUS "Windows detected: no system packages needed")
elseif(APPLE)
    message(STATUS "macOS detected: install SFML via Homebrew or conan")
endif()

# -------------------------------------------------------------
# Options pour compiler client et/ou serveur
option(BUILD_CLIENT "Build the client executable" ON)
option(BUILD_SERVER "Build the server executable" ON)

#  -------------------------------------------------------------
# Clang-Tidy
option(ENABLE_CLANG_TIDY "Enable clang-tidy analysis" OFF)

if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES clang-tidy)
    if(CLANG_TIDY_EXE)
        message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
        set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    else()
        message(WARNING "clang-tidy not found")
    endif()
endif()

# -------------------------------------------------------------
# Clang-Format
find_program(CLANG_FORMAT_EXE NAMES clang-format)
if(CLANG_FORMAT_EXE)
    file(GLOB_RECURSE ALL_CXX_SOURCE_FILES "src/*.cpp" "include/*.hpp")
    add_custom_target(
        clang_format
        COMMAND ${CLANG_FORMAT_EXE} -i ${ALL_CXX_SOURCE_FILES}
        COMMENT "Formatting all source files"
    )
endif()

# -------------------------------------------------------------
# Client
if(BUILD_CLIENT)
    file(GLOB_RECURSE CLIENT_SOURCES "src/rtype_client/*.cpp")
    if(NOT CLIENT_SOURCES)
        message(FATAL_ERROR "No client source files found in src/rtype_client/")
    endif()
    
    add_executable(r-type_client ${CLIENT_SOURCES})
    target_include_directories(r-type_client PRIVATE include)
    target_link_libraries(r-type_client PRIVATE)
    
    # Compiler flags for debug builds
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(r-type_client PRIVATE -g -Wall -Wextra)
    endif()
endif()

# -------------------------------------------------------------
# Serveur
if(BUILD_SERVER)
    file(GLOB_RECURSE SERVER_SOURCES "src/rtype_server/*.cpp")
    if(NOT SERVER_SOURCES)
        message(FATAL_ERROR "No server source files found in src/rtype_server/")
    endif()
    
    add_executable(r-type_server ${SERVER_SOURCES})
    target_include_directories(r-type_server PRIVATE include)
    target_link_libraries(r-type_server PRIVATE)
    
    # Compiler flags for debug builds
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(r-type_server PRIVATE -g -Wall -Wextra)
    endif()
endif()