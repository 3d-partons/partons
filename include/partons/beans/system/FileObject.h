/*
 * FileObject.h
 *
 *  Created on: May 17, 2016
 *      Author: debian
 */

#ifndef FILEOBJECT_H_
#define FILEOBJECT_H_

#include <string>

#include "../../BaseObject.h"

class CryptographicHashService;

class FileObject: public BaseObject {
public:
    FileObject();
    FileObject(const FileObject &other);
    virtual ~FileObject();

    const std::string& getFile() const;
    void setFile(const std::string& file);
    const std::string& getFilePath() const;
    void setFilePath(const std::string& filePath);
    const std::string& getHashSum() const;
    void setHashSum(const std::string& hashSum);

private:
    CryptographicHashService* m_pCryptographicHashService; ///<

    std::string m_filePath;
    std::string m_file;
    std::string m_hashSum;
};

#endif /* FILEOBJECT_H_ */
