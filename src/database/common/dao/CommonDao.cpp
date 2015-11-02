#include "CommonDao.h"

#include <Qt/qsqlquery.h>
#include <Qt/qvariant.h>
#include <QtCore/qstring.h>

#include "../../DatabaseManager.h"

int CommonDao::insertComputation(const std::string &computationDate) {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getDb());

    query.prepare(
            "INSERT INTO computation (computation_date) VALUES (:computation_date)");

    query.bindValue(":phi_list", QString(computationDate.c_str()));

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    }

    query.clear();

    return result;
}
