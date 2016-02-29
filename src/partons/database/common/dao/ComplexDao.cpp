#include "../../../../../include/partons/database/common/dao/ComplexDao.h"

#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>
#include <QtCore/qvariant.h>
#include <QtCore/qstring.h>
#include <string>

#include "../../../../../include/partons/database/DatabaseManager.h"
#include "../../../../../include/partons/utils/stringUtils/Formatter.h"

ComplexDao::ComplexDao() :
        BaseObject("ComplexDao") {
}

ComplexDao::~ComplexDao() {
}

int ComplexDao::insert(const double realPart, const double imgPart) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO complex (real_part, img_part) VALUES (:realPart, :imgPart)");

    query.bindValue(":realPart", realPart);
    query.bindValue(":imgPart", imgPart);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        error(__func__,
                Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return result;
}

int ComplexDao::select(const double realPart, const double imgPart) const {
    int result = -1;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT id FROM complex WHERE real_part = :realPart AND img_part = :imgPart");

    query.bindValue(":realPart", realPart);
    query.bindValue(":imgPart", imgPart);

    if (query.exec()) {
        if (query.first()) {
            result = query.value(0).toInt();
        }
    } else {
        error(__func__,
                Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return result;
}

std::complex<double> ComplexDao::getComplexById(const int id) const {
    std::complex<double> result;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare("SELECT * FROM complex WHERE id = :id");

    query.bindValue(":id", id);

    if (query.exec()) {
        if (query.first()) {
            result = getComplexFromQuery(query);
        }
    } else {
        error(__func__,
                Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return result;
}

std::complex<double> ComplexDao::getComplexFromQuery(QSqlQuery& query) const {

    int field_real_part = query.record().indexOf("real_part");
    int field_img_part = query.record().indexOf("img_part");

    double realPart = query.value(field_real_part).toDouble();
    double imgPart = query.value(field_img_part).toDouble();

    return std::complex<double>(realPart, imgPart);

}
