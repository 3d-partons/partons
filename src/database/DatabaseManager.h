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

#include <Qt/qsqlquery.h>
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

    /**
     * http://stackoverflow.com/a/26500811
     *
     * query.size() is not supported with SQLite. But you can get the number of rows with a workaround.
     *
     * @param query
     * @return
     */
    static int getNumberOfRows(QSqlQuery &query);

    // ##### GETTERS & SETTERS #####

    const QSqlDatabase& getProductionDatabase() const;
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
