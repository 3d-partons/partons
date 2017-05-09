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

#include "../BaseObject.h"

class CryptographicHashService;

/**
 * @class DatabaseFileObject
 *
 * @brief Base class for those representing files in database.
 *
 * It is the base class for those representing files that can be stored in the database. It provides a set of useful tools for those objects.
 */
class DatabaseFileObject: public BaseObject {
public:

    /**
     * Constructor.
     * @param className Name of class.
     */
    DatabaseFileObject(const std::string &className);

    /**
     * Constructor.
     * @param className Name of class.
     * @param indexId Unique id used when object is created from the database, see BaseObject::m_indexId.
     * @param storeDate Time of insertion into the database.
     * @param filePath Path to file.
     * @param hashSum Hash sum of file content.
     * @param file String containing file content.
     */
    DatabaseFileObject(const std::string &className, const int indexId,
            const time_t storeDate, const std::string& filePath,
            const std::string& hashSum, const std::string& file);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DatabaseFileObject(const DatabaseFileObject &other);

    /**
     * Destructor.
     */
    virtual ~DatabaseFileObject();

    virtual std::string toString() const;

    /**
     * Get reference to DatabaseFileObject::m_file.
     * @return Requested reference.
     */
    const std::string& getFile() const;

    /**
     * Set DatabaseFileObject::m_file.
     * @param file Object to be set.
     */
    void setFile(const std::string& file);

    /**
     * Get reference to DatabaseFileObject::m_hashSum.
     * @return Requested reference.
     */
    const std::string& getHashSum() const;

    /**
     * Set DatabaseFileObject::m_hashSum.
     * @param hashSum Object to be set.
     */
    void setHashSum(const std::string& hashSum);

    /**
     * Get DatabaseFileObject::m_storeDate.
     * @return Requested value.
     */
    time_t getStoreDate() const;

    /**
     * Set DatabaseFileObject::m_storeDate.
     * @param storeDate Value to be set.
     */
    void setStoreDate(time_t storeDate);

    /**
     * Get reference to DatabaseFileObject::m_filePath.
     * @return Requested reference.
     */
    const std::string& getFilePath() const;

    /**
     * Set DatabaseFileObject::m_filePath.
     * @param filePath Object to be set.
     */
    void setFilePath(const std::string& filePath);

protected:

    /**
     * Fill file from database.
     * @return String containing file.
     */
    virtual std::string fillFile() const = 0;

private:

    CryptographicHashService* m_pCryptographicHashService; ///< Pointer to CryptographicHashService.

    time_t m_storeDate; ///<  Time of insertion into the database.
    mutable std::string m_file; ///< String containing file content.
    mutable std::string m_hashSum; ///< Hash sum of file content.
    std::string m_filePath; ///< Path to file.

    /**
     * Set m_pCryptographicHashService pointer.
     */
    void setCryptographicHashService();
};

#endif /* DATABASE_FILE_OBJECT_H */
