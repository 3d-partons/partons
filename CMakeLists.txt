# BASIC DEFINITIONS ========================================================================

# define minimum version of cmake
cmake_minimum_required(VERSION 2.6)

# define project name and its language
project(PARTONS CXX)

# define c++ standard and issue all the warning demanded by this standard
add_definitions(-std=c++98 -pedantic)

# VERSION ==================================================================================

# read file
file(READ ${CMAKE_SOURCE_DIR}/data/version.txt VERSION_STR)

# strip spaces
string(STRIP ${VERSION_STR} VERSION_STR)

# get major and minor versions
string(REGEX REPLACE "\\.[0-9]*" "" PARTONS_VERSION_MAJOR ${VERSION_STR})
string(REGEX REPLACE "[0-9]*\\." "" PARTONS_VERSION_MINOR ${VERSION_STR})

# configure
configure_file(${CMAKE_SOURCE_DIR}/cmake/Version/PartonsVersion.h_in ${CMAKE_SOURCE_DIR}/include/partons/PartonsVersion.h)

# AUTOMOC ==================================================================================

# tell cmake to run Qt moc when necessary
set(CMAKE_AUTOMOC ON)
  
# as moc files are generated in the binary dir, tell cmake to always look for includes there
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# FIND LIBRARIES ===========================================================================

# find libraries: Qt4
find_package(Qt4 REQUIRED QtCore QtSql QtXmlPatterns)

# add some useful macros and variables
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# find libraries: additional modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# find libraries: SFML
find_package(SFML COMPONENTS system REQUIRED)

# find libraries: CLN
find_package(CLN REQUIRED)

# find libraries: ElementaryUtils
find_package(ElementaryUtils REQUIRED)

# find libraries: NumA++
find_package(NumA++ REQUIRED)

# find libraries: GSL
find_package(GSL REQUIRED)

# directories containing headers 
include_directories(${QT_INCLUDE_DIRS} ${SFML_INCLUDE_DIR} ${CLN_INCLUDE_DIR} ${ELEMENTARY_UTILS_INCLUDE_DIR} ${NUMA_INCLUDE_DIR})
include_directories(include)

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

		${QT_LIBRARIES}
          
        ${SFML_LIBRARIES}
          
        ${CLN_LIBRARIES}
          
        ${ELEMENTARY_UTILS_LIBRARIES}
          
		${NUMA_LIBRARIES}
		
		${GSL_LIBRARIES}
)

# install
install(TARGETS PARTONS 
        RUNTIME DESTINATION bin/PARTONS
        LIBRARY DESTINATION lib/PARTONS
		ARCHIVE DESTINATION lib/static/PARTONS)
install(DIRECTORY include/ DESTINATION include/PARTONS)
install(DIRECTORY data/ DESTINATION share/PARTONS/)
