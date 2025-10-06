# BASIC DEFINITIONS ========================================================================

# define minimum version of cmake
cmake_minimum_required(VERSION 3.5)

# define project name and its language
project(PARTONS CXX)

# define c++ standard and issue all the warning demanded by this standard
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
   add_definitions(-std=c++11 -pedantic -Wno-vla-extension)
else()
   add_definitions(-std=c++11 -pedantic -Wno-vla-extension -fext-numeric-literals)
endif()
set(CMAKE_CXX_STANDARD 11)

if (NOT DEFINED CMAKE_MACOSX_RPATH)
   set(CMAKE_MACOSX_RPATH 0)
endif()

# VERSION ==================================================================================

# read file
file(READ ${CMAKE_SOURCE_DIR}/data/version.txt VERSION_STR)

# strip spaces
string(STRIP ${VERSION_STR} VERSION_STR)

# get major, minor and patch versions
string(REGEX MATCH "^[0-9]*\\." TMP_STR ${VERSION_STR})
string(REGEX REPLACE "\\." "" PARTONS_VERSION_MAJOR ${TMP_STR})

string(REGEX MATCH "\\.[0-9]*\\." TMP_STR ${VERSION_STR})
string(REGEX REPLACE "\\." "" PARTONS_VERSION_MINOR ${TMP_STR})

string(REGEX MATCH "\\.[0-9]*$" TMP_STR ${VERSION_STR})
string(REGEX REPLACE "\\." "" PARTONS_VERSION_PATCH ${TMP_STR})

message("-- PARTONS version is: " ${PARTONS_VERSION_MAJOR} "." ${PARTONS_VERSION_MINOR} "." ${PARTONS_VERSION_PATCH})

execute_process(COMMAND bash -c "cd ${CMAKE_SOURCE_DIR}; git branch | grep '\*' | cut -d ' ' -f2 2> /dev/null" OUTPUT_VARIABLE PARTONS_GIT_BRANCH)
execute_process(COMMAND bash -c "cd ${CMAKE_SOURCE_DIR}; git rev-parse HEAD 2> /dev/null" OUTPUT_VARIABLE PARTONS_GIT_REVISION)
string(REGEX REPLACE "\n$" "" PARTONS_GIT_BRANCH "${PARTONS_GIT_BRANCH}")
string(REGEX REPLACE "\n$" "" PARTONS_GIT_REVISION "${PARTONS_GIT_REVISION}")

message("-- Git branch (revision): " ${PARTONS_GIT_BRANCH} " (" ${PARTONS_GIT_REVISION} ")")

# configure
configure_file(${CMAKE_SOURCE_DIR}/cmake/Version/PartonsVersion.h_in ${CMAKE_SOURCE_DIR}/include/partons/PartonsVersion.h)

# FIND LIBRARIES ===========================================================================

# find libraries: additional modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# find libraries: ElementaryUtils
find_package(ElementaryUtils REQUIRED)

# find libraries: NumA++
find_package(NumA++ REQUIRED)

# find libraries: SFML
find_package(SFML COMPONENTS system REQUIRED)

# find libraries: CLN
find_package(CLN REQUIRED)

# find libraries: GSL
find_package(GSL REQUIRED)

# find libraries: Apfel++
find_package(Apfel++ REQUIRED)

# find libraries: LHAPDF
find_package(LHAPDF REQUIRED)

# find libxml2
find_package(LibXml2)

# directories containing headers 
include_directories(${QT_INCLUDE_DIRS} ${ElementaryUtils_INCLUDE_DIR} ${NumA++_INCLUDE_DIR} ${SFML_INCLUDE_DIR} ${CLN_INCLUDE_DIR} ${GSL_INCLUDE_DIRS} ${Apfel++_INCLUDE_DIR} ${LHAPDF_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIRS})

# FINALIZE ==================================================================================

# generate list of source files
file(

        GLOB_RECURSE

        source_files

        src/*
)

# define target library
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_library(

        PARTONS

        SHARED

        ${source_files}
)

# define libraries to be linked
target_link_libraries(

	PARTONS
          
	${SFML_LIBRARIES}
          
	${CLN_LIBRARIES}
          
	${ElementaryUtils_LIBRARIES}
          
	${NumA++_LIBRARIES}

	${GSL_LIBRARIES}

	${Apfel++_LIBRARIES}

	${LHAPDF_LIBRARIES}

	${LIBXML2_LIBRARIES}
)

# install
install(TARGETS PARTONS 
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib/static)
install(DIRECTORY include/ DESTINATION include)
install(DIRECTORY data DESTINATION share)
