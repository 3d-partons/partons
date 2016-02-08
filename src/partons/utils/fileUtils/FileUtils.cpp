#include "../../../../include/partons/utils/fileUtils/FileUtils.h"

#include <sstream>

std::string FileUtils::EMPTY = "";

bool FileUtils::isReadable(const std::string & filePath) {
    std::ifstream file(filePath.c_str());
    return !file.fail();
}

void FileUtils::write(const std::string & filePath, const std::string & str) {
    std::ofstream file(filePath.c_str(), std::ios_base::app);

    if (file) {
        file << str;
    }
}

void FileUtils::writef(const std::string & filePath, const std::string & str) {
    std::ofstream file(filePath.c_str(), std::ios_base::app);

    if (file) {
        file << str << "\n";
    }
}

std::string FileUtils::read(const std::string & filePath) {
    std::string str = EMPTY;

    // le constructeur de ifstream permet d'ouvrir un fichier en lecture
    std::ifstream file(filePath.c_str());

    // ce test echoue si le fichier n'est pas ouvert
    if (file) {
        std::stringstream buffer;
        // copier l'integralite du fichier dans le buffer
        buffer << file.rdbuf();
        // nous n'avons plus besoin du fichier !
        file.close();
        // manipulations du buffer...
        str = buffer.str();
    }

    return str;
}

std::vector<std::string> FileUtils::readByLine(const std::string & filePath) {
    // variable contenant chaque ligne lue
    std::string line = EMPTY;
    // variable contenant l'ensemble des lignes lues
    std::vector<std::string> lines;

    std::ifstream file(filePath.c_str());

    // ce test echoue si le fichier n'est pas ouvert
    if (file) {
        // cette boucle s'arrete des qu'une erreur de lecture survient
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
    }

    return lines;
}

//TODO voir a cast le resultat en LONG
std::ifstream::pos_type FileUtils::getFileSize(const std::string & filetPath) {
    std::ifstream file(filetPath.c_str(), std::ios::binary | std::ios::ate);
    return file.tellg();
}

bool FileUtils::open(std::ofstream &fileOutputStream,
        const std::string & filePath) {
    fileOutputStream.open(filePath.c_str(),
            std::ofstream::out | std::ofstream::app);
    return fileOutputStream.is_open();
}

void FileUtils::write(std::ofstream& fileOutputStream, const std::string& str) {
    fileOutputStream << str;
    fileOutputStream.flush();
}

void FileUtils::close(std::ofstream& fileOutputStream) {
    fileOutputStream.close();
}
