#include "../../../include/partons/database/DatabaseManager.h"

#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <QtCore/qstring.h>
#include <QtSql/qsqlerror.h>
#include <string>

// Global static pointer used to ensure a single instance of the class.
DatabaseManager* DatabaseManager::m_pInstance = 0;

const QSqlDatabase& DatabaseManager::getProductionDatabase() const {
    return m_productionDatabase;
}

const QSqlDatabase& DatabaseManager::getTestDatabase() const {
    return m_testDatabase;
}

DatabaseManager::DatabaseManager() :
        BaseObject("DatabaseManager") {

    ElemUtils::PropertiesManager* pPropertiesManager =
            ElemUtils::PropertiesManager::getInstance();

    //TODO replace by static const variable
    std::string sqlDatabaseType = pPropertiesManager->getString(
            "database.production.type");

    if (ElemUtils::StringUtils::equalsIgnoreCase(sqlDatabaseType, "MYSQL")) {
        m_productionDatabase = QSqlDatabase::addDatabase("QMYSQL");

    } else {
        m_productionDatabase = QSqlDatabase::addDatabase("QSQLITE");
    }

    m_productionDatabase.setDatabaseName(
            QString(
                    pPropertiesManager->getString("database.production.dbname").c_str()));
    m_productionDatabase.setHostName(
            QString(
                    pPropertiesManager->getString("database.production.url").c_str()));
    m_productionDatabase.setUserName(
            QString(
                    pPropertiesManager->getString("database.production.user").c_str()));
    m_productionDatabase.setPassword(
            QString(
                    pPropertiesManager->getString("database.production.passwd").c_str()));

    if (!m_productionDatabase.open()) {
        error(__func__,
                ElemUtils::Formatter() << "Can't connect to database : "
                        << m_productionDatabase.lastError().text().toStdString());
    } else {
        info(__func__, "Database connection OK");
    }
}

DatabaseManager::~DatabaseManager() {
    if (m_pInstance != 0) {
        delete m_pInstance;
        m_pInstance = 0;
    }
}

DatabaseManager* DatabaseManager::getInstance() {
    // Only allow one instance of class to be generated.
    if (!m_pInstance) {
        m_pInstance = new DatabaseManager();
    }

    return m_pInstance;
}

void DatabaseManager::close() {
    QString connection;
    connection = m_productionDatabase.connectionName();
    m_productionDatabase.close();
    // QSqlDatabasePrivate::removeDatabase: connection 'qt_sql_default_connection' is still in use, all queries will cease to work.
    // After you closed it, m_db still holds a reference to the database you configured in connect().
    // You can reset m_db by assigning a default constructed QSqlDatabase
    // See : http://stackoverflow.com/questions/9519736/warning-remove-database
    m_productionDatabase = QSqlDatabase();
    m_productionDatabase.removeDatabase(connection);
}

int DatabaseManager::getNumberOfRows(QSqlQuery& query) {
    int numberOfRows = 0;

    if (query.last()) {
        numberOfRows = query.at() + 1;
        query.first();
        query.previous();
    }

    return numberOfRows;
}
