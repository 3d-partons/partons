cmake_minimum_required(VERSION 2.6)

#Configuration du projet

project(PARTONS CXX)

#  placez vos exécutables dans des dossiers portant le nom du type de compilation (Release, Debug...). Ce dernier est défini par la variable CMAKE_BUILD_TYPE (qui n'a d'effet seulement si le générateur choisi est basé sur Make)

set(LIBRARY_OUTPUT_PATH lib)

# Inclusion des libraries

 # this command finds Qt4 libraries and sets all required variables
# note that it's Qt4, not QT4 or qt4
FIND_PACKAGE( Qt4 REQUIRED )

 # add some useful macros and variables
# (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
INCLUDE( ${QT_USE_FILE} )

include_directories(${root_include_dir} ${numa_include_dir} ${cln_include_dir})

link_directories(${root_lib_dir} ${numa_lib_dir} ${cln_lib_dir})

#Inclusion des en-têtes publics
include_directories(include)

file(

        GLOB_RECURSE

        header_files

        include/*
)

file(

        GLOB_RECURSE

        source_files

        src/*
)


# and finally this will run moc:
QT4_WRAP_CPP( source_files include )

#Génération de la liste des fichiers sources

QT4_WRAP_CPP(LIB_HEADER_MOC ${header_files})

add_library(

        PARTONS

        SHARED

        ${source_files}
        
        ${LIB_HEADER_MOC}
)

#Configuration de l'édition de liens
target_link_libraries(

        PARTONS

         ${root_lib_name}
         
         ${numa_lib_name}
         
         ${cln_lib_name}
         
         ${QT_LIBRARIES}

)