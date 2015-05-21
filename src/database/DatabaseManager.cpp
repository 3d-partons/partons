#include "DatabaseManager.h"

#include <Qt/qsqlerror.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlrecord.h>
#include <QtSql/qsqltablemodel.h>
#include <iostream>
#include <stdexcept>
#include <string>

#include "../utils/PropertiesManager.h"
#include "../utils/stringUtils/StringUtils.h"

// Global static pointer used to ensure a single instance of the class.
DatabaseManager* DatabaseManager::m_pInstance = 0;

DatabaseManager::DatabaseManager() {
    //TODO replace by static const variable
    std::string sqlDatabaseType = PropertiesManager::getInstance()->getString(
            "database.type");

    if (StringUtils::equalsIgnoreCase(sqlDatabaseType, "SQLITE")) {
        m_db = QSqlDatabase::addDatabase("QSQLITE");

    } else {
        m_db = QSqlDatabase::addDatabase("QMYSQL");
    }

    m_db.setDatabaseName(
            QString(
                    PropertiesManager::getInstance()->getString(
                            "database.dbname").c_str()));
    m_db.setHostName(
            QString(
                    PropertiesManager::getInstance()->getString("database.url").c_str()));
    m_db.setUserName(
            QString(
                    PropertiesManager::getInstance()->getString("database.user").c_str()));
    m_db.setPassword(
            QString(
                    PropertiesManager::getInstance()->getString(
                            "database.passwd").c_str()));

    if (!m_db.open()) {
        throw std::runtime_error(
                "[DatabaseManager::DatabaseManager] Can't connect to database : "
                        + m_db.lastError().text().toStdString());
    } else {
        std::cerr << "Database connexion OK" << std::endl;

        QSqlTableModel model;
        model.setTable("tbl1");
        model.select();

        for (int i = 0; i < model.rowCount(); ++i) {
            QSqlRecord record = model.record(i);

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
    m_db.close();
    m_db.removeDatabase(m_db.connectionName());
}

const QSqlDatabase& DatabaseManager::getDb() const {
    return m_db;
}

void DatabaseManager::setDb(const QSqlDatabase& db) {
    m_db = db;
}
