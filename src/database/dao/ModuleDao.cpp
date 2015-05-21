#include "ModuleDao.h"

#include <Qt/qsqlquery.h>
#include <Qt/qvariant.h>
#include <QtCore/qstring.h>

#include "../DatabaseManager.h"

int ModuleDao::getModuleIdByClassName(const std::string &moduleClassName) {
    int result = -1;

    QSqlQuery query(DatabaseManager::getInstance()->getDb());

    query.prepare("SELECT id FROM module WHERE class_name = :class_name");

    query.bindValue(":class_name", QString(moduleClassName.c_str()));

    if (query.exec()) {
        if (query.first()) {
            result = query.value(0).toInt();
        }
    }

    return result;
}
