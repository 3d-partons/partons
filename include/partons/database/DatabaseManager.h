#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

/**
 * @file DatabaseManager.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 20 April 2015
 * @version 1.0
 */

#include <QtSql/qsqldatabase.h>

#include "../BaseObject.h"

namespace PARTONS {

/**
 * @class DatabaseManager
 *
 * @brief Manager for database handling.
 *
 * It is responsible for database handling, including initialization, giving the access and closing.
 */
class DatabaseManager: BaseObject {
public:

    /**
     * Share a unique pointer of this class.
     */
    static DatabaseManager* getInstance();

    /**
     * Destructor.
     */
    virtual ~DatabaseManager();

    /**
     * Initialize manager - open connection to the database.
     */
    void init();

    /**
     * Close manager - close connection to the database.
     */
    void close();

    /**
     * Get reference to DatabaseManager::m_productionDatabase.
     * @return Requested reference.
     */
    const QSqlDatabase& getProductionDatabase();

private:

    /**
     * Private pointer of this class for a unique instance
     */
    static DatabaseManager* m_pInstance;

    /**
     * Default constructor
     */
    DatabaseManager();

    /**
     * The database represented by QSqlDatabase object.
     */
    QSqlDatabase m_productionDatabase;

    //TODO Test database not used - to be removed?
    /**
     * Test database.
     */
    QSqlDatabase m_testDatabase;
};

} /* namespace PARTONS */

#endif /* DATABASE_MANAGER_H */
