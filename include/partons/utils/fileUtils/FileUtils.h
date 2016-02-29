#ifndef FILE_UTILS_H
#define FILE_UTILS_H

/*!
 * @file FileUtils.h
 * @brief Handle file system
 * @author Bryan BERTHOU
 * @version 2.0
 * @date 28/04/2009
 * @date Updated 18/09/2014
 *
 * @class FileUtils
 */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//TODO commentaire et documentation
//TODO improve exception, error, function name, ...

class FileUtils {
    static std::string EMPTY;

public:

    static bool isReadable(const std::string & filePath);

    static bool open(std::ofstream &fileOutputStream,
            const std::string & filePath);
    static void write(std::ofstream & fileOutputStream,
            const std::string & str);
    static void close(std::ofstream & fileOutputStream);

    static void write(const std::string & filePath, const std::string & str);
    static void writef(const std::string & filePath, const std::string & str);
    static std::string read(const std::string & filePath);
    static std::vector<std::string> readByLine(const std::string & filePath);
    static std::ifstream::pos_type getFileSize(const std::string & filetPath);
};

#endif /* FILE_UTILS_H */
