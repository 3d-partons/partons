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

namespace PARTONS {

class CryptographicHashService;

/**
 * @class FileObject
 *
 * @brief Class representing single text file.
 *
 * This class represents a single text file. It contains path to the file, its content and a hash sum of this content.
 */
class FileObject: public BaseObject {

public:

    /**
     * Default constructor.
     */
    FileObject();

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    FileObject(const FileObject &other);

    /**
     * Destructor.
     */
    virtual ~FileObject();

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get path to the file.
     */
    const std::string& getFilePath() const;

    /**
     * Set path to the file.
     */
    void setFilePath(const std::string& filePath);

    /**
     * Get content of the file.
     */
    const std::string& getFile() const;

    /**
     * Set content of the file.
     */
    void setFile(const std::string& file);

    /**
     * Get hash sum of file's content.
     */
    const std::string& getHashSum() const;

    /**
     * Set hash sum of file's content.
     */
    void setHashSum(const std::string& hashSum);

private:

    /**
     * Pointer to CryptographicHashService.
     */
    CryptographicHashService* m_pCryptographicHashService;

    /**
     * Path to the file.
     */
    std::string m_filePath;

    /**
     * Content of the file.
     */
    std::string m_file;

    /**
     * Hash sum of file's content.
     */
    std::string m_hashSum;
};

} /* namespace PARTONS */

#endif /* FILEOBJECT_H_ */
