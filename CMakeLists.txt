cmake_minimum_required(VERSION 2.6)

project(PARTONS CXX)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
  
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

 # this command finds Qt4 libraries and sets all required variables
# note that it's Qt4, not QT4 or qt4
FIND_PACKAGE( Qt4 COMPONENTS QtCore QtSql QtGui REQUIRED )

 # add some useful macros and variables
# (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS (${QT_DEFINITIONS})

# Recherche de librairies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
FIND_PACKAGE( ROOT COMPONENTS Core Hist REQUIRED)
find_package(SFML COMPONENTS system REQUIRED)
find_package(CLN REQUIRED)

# NumA++
set(NUMA_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/../NumA++/include")
set(NUMA_LIB_DIR "${CMAKE_SOURCE_DIR}/../NumA++/lib")
set(NUMA_LIBRARIES NumA++)

# Inclusion des libraries
INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIRS} ${ROOT_INCLUDE_DIR} ${NUMA_INCLUDE_DIR} ${CLN_INCLUDE_DIR} ${SFML_INCLUDE_DIR})

# Link libraries
LINK_DIRECTORIES(${NUMA_LIB_DIR})

#Inclusion des en-têtes publics
include_directories(include)

file(

        GLOB_RECURSE

        source_files

        src/*
)


# and finally this will run moc:
#QT4_WRAP_CPP( source_files include )

#Génération de la liste des fichiers sources

#QT4_WRAP_CPP(LIB_HEADER_MOC ${header_files})

add_library(

        PARTONS

        SHARED

        ${source_files}
)

#Configuration de l'édition de liens
target_link_libraries(

        PARTONS

         ${QT_LIBRARIES}
 
         ${ROOT_LIBRARIES}
         
         ${NUMA_LIBRARIES}
         
         ${CLN_LIBRARIES}

         ${SFML_LIBRARIES}
)