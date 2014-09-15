#ifndef FILE_UTILS_H
#define FILE_UTILS_H

/*!
 * \file FileUtils.h
 * \brief Manipulations et traitements sur les fichiers
 * \author Bryan BERTHOU
 * \version 0.1
 * \date Creation : 28/04/2009
 * \date Derniere maj : 08/09/2014
 */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//TODO uniformiser le fichier, corriger les parametres des fonctions (const, &, ...)

class FileUtils {
    static std::string EMPTY;

public:

    static bool is_readable(const std::string filePath);
    static bool is_readable(const char* filePath);
    static void write(std::string _fichierPath, std::string _chaine);
    static void writef(std::string _fichierPath, std::string _chaine);
    static std::string read(std::string _fichierPath);
    static std::vector<std::string> readByLine(std::string _fichierPath);
    //static int get_size( std::string _fichierPath );
    static std::ifstream::pos_type fileSsize(const char* filename);
};

#endif /* FILE_UTILS_H */
