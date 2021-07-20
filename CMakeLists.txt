# BASIC DEFINITIONS ========================================================================

# define minimum version of cmake
cmake_minimum_required(VERSION 3.2)

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

# configure
configure_file(${CMAKE_SOURCE_DIR}/cmake/Version/PartonsVersion.h_in ${CMAKE_SOURCE_DIR}/include/partons/PartonsVersion.h)

# AUTOMOC ==================================================================================

# tell cmake to run Qt moc when necessary
set(CMAKE_AUTOMOC ON)
  
# as moc files are generated in the binary dir, tell cmake to always look for includes there
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# FIND LIBRARIES ===========================================================================

# find libraries: additional modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# find libraries: Qt
find_package(Qt5 COMPONENTS Core Sql XmlPatterns QUIET)

if(${Qt5_FOUND})

   find_package(Qt5Widgets)
   message("-- Found Qt5: " ${Qt5Widgets_VERSION})

   set(QT_LIBRARY_CORE "Qt5::Core")
   set(QT_LIBRARY_SQL "Qt5::Sql")
   set(QT_LIBRARY_XML "Qt5::XmlPatterns")
   
   set(QT_VERSION ${Qt5Widgets_VERSION})
   
else()

   find_package(Qt4 COMPONENTS QtCore QtSql QtXmlPatterns)

   if(${Qt4_FOUND})

      set(QT_LIBRARY_CORE "Qt4::QtCore")
      set(QT_LIBRARY_SQL "Qt4::QtSql")
      set(QT_LIBRARY_XML "Qt4::QtXmlPatterns")
      
      set(QT_VERSION ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH})

   else()
      message(FATAL_ERROR "Neither Qt5 nor Qt4 found in this system")
   endif()
endif()

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

# directories containing headers 
include_directories(${QT_INCLUDE_DIRS} ${SFML_INCLUDE_DIR} ${CLN_INCLUDE_DIR} ${ElementaryUtils_INCLUDE_DIR} ${NumA++_INCLUDE_DIR} ${Apfel++_INCLUDE_DIR} ${LHAPDF_INCLUDE_DIR})
include_directories(include)

# ENVIRONMENT VARIABLES ========================================================================

execute_process(COMMAND bash -c "cd ../../elementary-utils/; git branch | grep '\*' | cut -d ' ' -f2" OUTPUT_VARIABLE ELEMUTILS_BRANCH)
execute_process(COMMAND bash -c "cd ../../elementary-utils/; git rev-parse HEAD;"             OUTPUT_VARIABLE ELEMUTILS_REVISION)
string(REGEX REPLACE "\n$" "" ELEMUTILS_BRANCH "${ELEMUTILS_BRANCH}")
string(REGEX REPLACE "\n$" "" ELEMUTILS_REVISION "${ELEMUTILS_REVISION}")

execute_process(COMMAND bash -c "cd ../../numa/; git branch | grep '\*' | cut -d ' ' -f2" OUTPUT_VARIABLE NUMA_BRANCH)
execute_process(COMMAND bash -c "cd ../../numa/; git rev-parse HEAD;"             OUTPUT_VARIABLE NUMA_REVISION)
string(REGEX REPLACE "\n$" "" NUMA_BRANCH "${NUMA_BRANCH}")
string(REGEX REPLACE "\n$" "" NUMA_REVISION "${NUMA_REVISION}")

execute_process(COMMAND bash -c "git branch | grep '\*' | cut -d ' ' -f2" OUTPUT_VARIABLE PARTONS_BRANCH)
execute_process(COMMAND bash -c "git rev-parse HEAD"             OUTPUT_VARIABLE PARTONS_REVISION)
string(REGEX REPLACE "\n$" "" PARTONS_BRANCH "${PARTONS_BRANCH}")
string(REGEX REPLACE "\n$" "" PARTONS_REVISION "${PARTONS_REVISION}")

configure_file("${CMAKE_SOURCE_DIR}/data/config/environment_configuration.dat.in" "${CMAKE_SOURCE_DIR}/data/config/environment_configuration.dat")

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

		${QT_LIBRARY_CORE}
		
		${QT_LIBRARY_SQL}
		
		${QT_LIBRARY_XML}
)

# install
install(TARGETS PARTONS 
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib/static)
install(DIRECTORY include/ DESTINATION include)
install(DIRECTORY data DESTINATION share)
