#include "../../../include/partons/database/DatabaseManager.h"

#include <Qt/qsqlerror.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlrecord.h>
#include <QtSql/qsqltablemodel.h>
#include <iostream>
#include <string>

#include "../../../include/partons/utils/PropertiesManager.h"
#include "../../../include/partons/utils/stringUtils/Formatter.h"
#include "../../../include/partons/utils/stringUtils/StringUtils.h"

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

    //TODO replace by static const variable
    std::string sqlDatabaseType = PropertiesManager::getInstance()->getString(
            "database.production.type");

    if (StringUtils::equalsIgnoreCase(sqlDatabaseType, "MYSQL")) {
        m_productionDatabase = QSqlDatabase::addDatabase("QMYSQL");

    } else {
        m_productionDatabase = QSqlDatabase::addDatabase("QSQLITE");
    }

    m_productionDatabase.setDatabaseName(
            QString(
                    PropertiesManager::getInstance()->getString(
                            "database.production.dbname").c_str()));
    m_productionDatabase.setHostName(
            QString(
                    PropertiesManager::getInstance()->getString(
                            "database.production.url").c_str()));
    m_productionDatabase.setUserName(
            QString(
                    PropertiesManager::getInstance()->getString(
                            "database.production.user").c_str()));
    m_productionDatabase.setPassword(
            QString(
                    PropertiesManager::getInstance()->getString(
                            "database.production.passwd").c_str()));

    if (!m_productionDatabase.open()) {
        error(__func__,
                Formatter() << "Can't connect to database : "
                        << m_productionDatabase.lastError().text().toStdString());
    } else {
        info(__func__, "Database connection OK");

        QSqlTableModel model;
        model.setTable("tbl1");
        model.select();

        for (int i = 0; i < model.rowCount(); ++i) {
            QSqlRecord record = model.record(i);

            //TODO convert cout to logger
            std::cout << "col1 = "
                    << (record.value(0).toString()).toUtf8().constData()
                    << " - col2 = " << record.value(1).toInt() << std::endl;
        }
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
