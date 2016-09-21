#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

/**
 * @file DatabaseManager.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 20 April 2015
 * @version 1.0
 *
 * @class DatabaseManager
 * @brief
 */

#include <QtSql/qsqldatabase.h>

#include "../BaseObject.h"

class DatabaseManager: BaseObject {
public:
    /**
     * Share a unique pointer of this class
     */
    static DatabaseManager* getInstance();

    /**
     * Default destructor
     */
    virtual ~DatabaseManager();

    void close();

    // ##### GETTERS & SETTERS #####

    const QSqlDatabase& getProductionDatabase();
    const QSqlDatabase& getTestDatabase() const;

private:

    /**
     * Private pointer of this class for a unique instance
     */
    static DatabaseManager* m_pInstance;

    /**
     * Default constructor
     */
    DatabaseManager();

    QSqlDatabase m_productionDatabase;
    QSqlDatabase m_testDatabase;
};

#endif /* DATABASE_MANAGER_H */
