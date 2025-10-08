cmake_minimum_required(VERSION 3.15)
project(RType VERSION 1.0 LANGUAGES CXX)

# -------------------------------------------------------------
# Standard C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# IMPORTANT : Définir CMAKE_BUILD_TYPE avant tout
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if(UNIX AND NOT APPLE)
    message(STATUS "Linux detected: make sure X11 libs are installed")
    find_library(UDEV_LIB udev)
    if(NOT UDEV_LIB)
        message(FATAL_ERROR
        "libudev non trouvée ! \n"
        "Installez-le d'abord sur votre système : \n"
        " Ubuntu/Debian: sudo apt-get install libudev-dev\n"
        " Fedora: sudo dnf install systemd-devel\n"
        " Arch: sudo pacman -S systemd"
        )
    endif()
endif()

#----------------------------------------------------------------
if (NOT EXISTS "$ENV{HOME}/.conan2/profiles/default")
    message(STATUS "Détection du profil Conan...")
    execute_process(
        COMMAND conan profile detect --force
        RESULT_VARIABLE result 
    )
    if(result)
        message(FATAL_ERROR "Impossible de détecter le profil Conan !")
    endif()
endif()

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.lock")
    message(STATUS "Installation des dépendances conan...")
    execute_process(
        COMMAND conan install ${CMAKE_SOURCE_DIR} --output-folder=${CMAKE_BINARY_DIR} --build=missing -s build_type=${CMAKE_BUILD_TYPE}
        RESULT_VARIABLE result
    )
    if(result)
        message(FATAL_ERROR "Conan install a échoué !")
    endif()
    file(TOUCH "${CMAKE_BINARY_DIR}/conan.lock")
endif()

# Inclure le toolchain Conan UNE SEULE FOIS
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

# Chercher les packages Conan
find_package(SFML REQUIRED)
find_package(asio REQUIRED)
find_package(GTest REQUIRED)

#----------------------------------------------------------------
# Génération de la documentation avec Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
else()
    message(STATUS "Doxygen not found, documentation will not be generated")
endif()

# -------------------------------------------------------------
# Options pour compiler client et/ou serveur
option(BUILD_CLIENT "Build the client executable" ON)
option(BUILD_SERVER "Build the server executable" ON)
option(BUILD_TESTS "Build the test executable" ON)

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
    file(GLOB_RECURSE ALL_CXX_SOURCE_FILES "rtype_client/src/*.cpp" "rtype_client/include/*.hpp" "rtype_server/src/*.cpp" "rtype_server/include/*.hpp")
    add_custom_target(
        clang_format
        COMMAND ${CLANG_FORMAT_EXE} -i ${ALL_CXX_SOURCE_FILES}
        COMMENT "Formatting all source files"
    )
endif()

# -------------------------------------------------------------
# Client
if(BUILD_CLIENT)
    file(GLOB_RECURSE CLIENT_SOURCES "rtype_client/src/*.cpp" "mains/client/main.cpp" "utility_classes/src/*.cpp" "engine/src/*.cpp" )
    if(NOT CLIENT_SOURCES)
        message(FATAL_ERROR "No client source files found in rtype_client/src/")
    endif()
    
    add_executable(r-type_client ${CLIENT_SOURCES})
    target_include_directories(r-type_client PRIVATE 
        ${CMAKE_SOURCE_DIR}/rtype_client/include 
        "utility_classes/include" 
        "engine/include/" 
        ${CMAKE_SOURCE_DIR}/asio-1.36.0/include
    )
    
    target_link_libraries(r-type_client PRIVATE 
        sfml::sfml
        asio::asio
        config++
    )
    
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(r-type_client PRIVATE -g -Wall)
    endif()
endif()

# -------------------------------------------------------------
# Serveur
if(BUILD_SERVER)
    file(GLOB_RECURSE SERVER_SOURCES "rtype_server/src/*.cpp" "mains/server/main.cpp" "utility_classes/src/*.cpp" "engine/src/*.cpp" )
    if(NOT SERVER_SOURCES)
        message(FATAL_ERROR "No server source files found in rtype_server/src/")
    endif()
    
    add_executable(r-type_server ${SERVER_SOURCES})
    target_include_directories(r-type_server PRIVATE 
        ${CMAKE_SOURCE_DIR}/rtype_server/include 
        "utility_classes/include" 
        "engine/include/" 
        ${CMAKE_SOURCE_DIR}/asio-1.36.0/include
    )
    
    target_link_libraries(r-type_server PRIVATE 
        sfml::sfml
        asio::asio
        config++
    )
    
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(r-type_server PRIVATE -g -Wall)
    endif()
endif()

# -------------------------------------------------------------
# Tests
if(BUILD_TESTS)
    file(GLOB_RECURSE TESTS_SOURCES "rtype_server/tests/*.cpp"  "rtype_client/tests/*.cpp" "utility_classes/tests/*.cpp" "engine/tests/*.cpp" "rtype_server/src/*.cpp"  "rtype_client/src/*.cpp" "utility_classes/src/*.cpp" "engine/src/*.cpp" )
    if(NOT TESTS_SOURCES)
        message(FATAL_ERROR "No server source files found")
    endif()

    add_executable(r-type_tests ${TESTS_SOURCES})
    target_include_directories(r-type_tests PRIVATE 
        ${CMAKE_SOURCE_DIR}/rtype_server/include  
        ${CMAKE_SOURCE_DIR}/rtype_client/include 
        "utility_classes/include" 
        "engine/include" 
        ${CMAKE_SOURCE_DIR}/asio-1.36.0/include
    )
    
    target_link_libraries(r-type_tests PRIVATE 
        GTest::gtest 
        GTest::gtest_main
        sfml::sfml
        asio::asio
        config++
    )

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(r-type_tests PRIVATE -g -Wall)
    endif()
endif()