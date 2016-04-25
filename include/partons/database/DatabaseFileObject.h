#ifndef DATABASE_FILE_OBJECT_H
#define DATABASE_FILE_OBJECT_H

/**
 * @file DatabaseFileObject.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date April 12, 2015
 * @version 1.0
 */

#include <ctime>
#include <string>

#include "DatabaseObject.h"

class CryptographicHashService;

/**
 * @class DatabaseFileObject
 *
 * @brief
 */
class DatabaseFileObject: public DatabaseObject {
public:
    /**
     *
     * @param className
     */
    DatabaseFileObject(const std::string &className);

    /**
     * Destructor
     */
    virtual ~DatabaseFileObject();

    // ##### GETTERS & SETTERS #####

    const std::string& getFile() const;
    void setFile(const std::string& file);
    const std::string& getHashSum() const;
    void setHashSum(const std::string& hashSum);
    time_t getStoreDate() const;
    void setStoreDate(time_t storeDate);
    const std::string& getFilePath() const;
    void setFilePath(const std::string& filePath);

private:
    CryptographicHashService* m_pCryptographicHashService; ///<

    time_t m_storeDate; ///<
    mutable std::string m_file; ///<
    mutable std::string m_hashSum; ///<

    std::string m_filePath; ///<
    /**
     *
     */
    void setCryptographicHashService();
};

#endif /* DATABASE_FILE_OBJECT_H */
