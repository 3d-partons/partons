#include "FileUtils.h"

#include <fstream>
#include <sstream>

std::string FileUtils::EMPTY = "";

bool FileUtils::is_readable(const std::string filePath) {
    std::ifstream file(filePath.c_str());
    return !file.fail();
}

bool FileUtils::is_readable(const char* filePath) {
    std::ifstream file(filePath);
    return !file.fail();
}

void FileUtils::write(std::string _fichierPath, std::string _chaine) {
    std::ofstream fichier(_fichierPath.c_str(), std::ios_base::app);

    if (fichier) {
        fichier << _chaine;
    }
}

void FileUtils::writef(std::string _fichierPath, std::string _chaine) {
    std::ofstream fichier(_fichierPath.c_str(), std::ios_base::app);

    if (fichier) {
        fichier << _chaine << "\n";
    }
}

std::string FileUtils::read(std::string _fichierPath) {
    std::string chaine = EMPTY;

    // le constructeur de ifstream permet d'ouvrir un fichier en lecture
    std::ifstream fichier(_fichierPath.c_str());

    // ce test echoue si le fichier n'est pas ouvert
    if (fichier) {
        std::stringstream buffer;
        // copier l'integralite du fichier dans le buffer
        buffer << fichier.rdbuf();
        // nous n'avons plus besoin du fichier !
        fichier.close();
        // manipulations du buffer...
        chaine = buffer.str();
    }

    return chaine;
}

std::vector<std::string> FileUtils::readByLine(std::string _fichierPath) {
    // variable contenant chaque ligne lue
    std::string ligne = EMPTY;
    // variable contenant l'ensemble des lignes lues
    std::vector<std::string> lignes;

    std::ifstream fichier(_fichierPath.c_str());

    // ce test echoue si le fichier n'est pas ouvert
    if (fichier) {
        // cette boucle s'arrete des qu'une erreur de lecture survient
        while (std::getline(fichier, ligne)) {
            lignes.push_back(ligne);
        }
    }

    return lignes;
}

//int FileUtils::get_size(std::string _fichierPath) {
//    int size = -1;
//
//    std::ifstream fichier(_fichierPath.c_str());
//
//    // ce test echoue si le fichier n'est pas ouvert
//    if (fichier) {
//        // sauvegarder la position courante
//        long pos = fichier.tellg();
//        // se placer en fin de fichier
//        fichier.seekg(0, std::ios_base::end);
//        // recuperer la nouvelle position = la taille du fichier
//        size = fichier.tellg();
//        // restaurer la position initiale du fichier
//        fichier.seekg(pos, std::ios_base::beg);
//    }
//
//    return size;
//}

//TODO voir a cast le resultat en LONG
std::ifstream::pos_type FileUtils::fileSsize(const char* filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    return file.tellg();
}
