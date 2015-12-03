cmake_minimum_required(VERSION 2.6)

                

#Configuration du projet

project(PARTONS CXX)

#  placez vos exécutables dans des dossiers portant le nom du type de compilation (Release, Debug...). Ce dernier est défini par la variable CMAKE_BUILD_TYPE (qui n'a d'effet seulement si le générateur choisi est basé sur Make)

set(LIBRARY_OUTPUT_PATH lib/${CMAKE_BUILD_TYPE})

# Inclusion de librarie ROOT

include_directories(${root_include_dir} ${numa_include_dir} ${cln_include_dir} ${qt4_include_dir})

link_directories(${root_lib_dir} ${numa_lib_dir} ${cln_lib_dir} ${qt4_lib_dir})

#Inclusion des en-têtes publics

include_directories(include)

#Génération de la liste des fichiers sources

file(

        GLOB_RECURSE

        source_files

        src/*
)

add_library(

        PARTONS

        SHARED

        ${source_files}
)

#Configuration de l'édition de liens
target_link_libraries(

        PARTONS

         ${root_lib_name}
         
         ${numa_lib_name}
         
         ${cln_lib_name}
         
         ${qt4_lib_name}

)