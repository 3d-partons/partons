#include "../../../../../include/partons/database/observable/service/ObservableResultDaoService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlquery.h>
#include <utility>

#include "../../../../../include/partons/beans/observable/ObservableKinematic.h"
#include "../../../../../include/partons/database/Database.h"
#include "../../../../../include/partons/database/DatabaseManager.h"
#include "../../../../../include/partons/utils/math/ErrorBar.h"
#include "../../../../../include/partons/utils/type/PhysicalType.h"

namespace PARTONS {

ObservableResultDaoService::ObservableResultDaoService() :
        ResultDaoService("ObservableResultDaoService"), m_lastObservableKinematicId(
                -1), m_lastObservableResultId(-1), m_observableKinematicTableFile(
                ElemUtils::StringUtils::EMPTY), m_observableResultTableFile(
                ElemUtils::StringUtils::EMPTY) {
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    ElemUtils::Formatter formatter;
    formatter << "SELECT COUNT("
            << Database::COLUMN_NAME_OBSERVABLE_KINEMATIC_ID << ") FROM "
            << Database::TABLE_NAME_OBSERVABLE_KINEMATIC;

    if (query.exec(QString(formatter.str().c_str()))) {
        if (query.first()) {
            m_lastObservableKinematicId = query.value(0).toInt();
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }
    formatter.clear();
    query.clear();

    formatter << "SELECT COUNT(" << Database::COLUMN_NAME_OBSERVABLE_RESULT_ID
            << ") FROM " << Database::TABLE_NAME_OBSERVABLE_RESULT;

    if (query.exec(QString(formatter.str().c_str()))) {
        if (query.first()) {
            m_lastObservableResultId = query.value(0).toInt();
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }
    formatter.clear();
    query.clear();
}

ObservableResultDaoService::~ObservableResultDaoService() {
}

int ObservableResultDaoService::insert(
        const ObservableResult &observableResult) {

    List<ObservableResult> results;
    results.add(observableResult);

    return insert(results);
}

int ObservableResultDaoService::insert(
        const List<ObservableResult> &resultList) {
    // For multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {
        info(__func__, "Prepare data before inserting them into database ...");
        info(__func__,
                ElemUtils::Formatter() << resultList.size()
                        << " ObservableResult(s) will be inserted ...");

        for (unsigned int i = 0; i != resultList.size(); i++) {
            prepareCommonTablesFromResultInfo(resultList[i].getResultInfo());

            ObservableKinematic kinematic = resultList[i].getKinematic();

            int kinematicId =
                    m_observableKinematicDaoService.getKinematicIdByHashSum(
                            kinematic.getHashSum());

            if (kinematicId == -1) {
                m_lastObservableKinematicId++;
                kinematicId = m_lastObservableKinematicId;

                //TODO remove hardcoded 0 values
                m_observableKinematicTableFile += ElemUtils::Formatter()
                        << m_lastObservableKinematicId << "," << 0 << ","
                        << kinematic.getXB() << "," << kinematic.getT() << ","
                        << kinematic.getQ2() << "," << kinematic.getE() << ","
                        << kinematic.getPhi().getValue() << "," << 0 << ","
                        << kinematic.getHashSum() << '\n';
            }

            m_lastObservableResultId++;
            m_observableResultTableFile += ElemUtils::Formatter()
                    << m_lastObservableResultId << ","
                    << resultList[i].getObservableName() << ","
                    << resultList[i].getValue() << ","
                    << resultList[i].getStatError().getLowerBound() << ","
                    << resultList[i].getStatError().getUpperBound() << ","
                    << resultList[i].getSystError().getLowerBound() << ","
                    << resultList[i].getSystError().getUpperBound() << ","
                    << resultList[i].getScaleError().getLowerBound() << ","
                    << resultList[i].getScaleError().getUpperBound() << ","
                    << resultList[i].getComputationModuleName() << ","
                    << resultList[i].getObservableType() << "," << kinematicId
                    << "," << m_previousComputationId.second << '\n';
        }

        insertCommonDataIntoDatabaseTables();

        insertDataIntoDatabaseTables("observableKinematicTableFile.csv",
                m_observableKinematicTableFile, "observable_kinematic");
        insertDataIntoDatabaseTables("observableResultTableFile.csv",
                m_observableResultTableFile, "observable_result");

        // If there is no exception we can commit all query
        QSqlDatabase::database().commit();

        info(__func__, "Done !");

    } catch (const std::exception &e) {
        // Else return database in a stable state : n-1
        QSqlDatabase::database().rollback();

        throw ElemUtils::CustomException(getClassName(), __func__, e.what());
    }

    return getLastComputationId();
}

List<ObservableResult> ObservableResultDaoService::getObservableResultListByComputationId(
        const int computationId) const {
    return m_observableResultDao.getObservableResultListByComputationId(
            computationId);
}

List<ObservableResult> ObservableResultDaoService::getObservableResultListFromSQLQuery(
        const std::string& sqlQuery) const {
    return m_observableResultDao.getObservableResultListFromSQLQuery(sqlQuery);
}

} /* namespace PARTONS */
