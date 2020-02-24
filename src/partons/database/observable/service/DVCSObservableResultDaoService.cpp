#include "../../../../../include/partons/database/observable/service/DVCSObservableResultDaoService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqlquery.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>

#include "../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../include/partons/beans/Result.h"
#include "../../../../../include/partons/database/DatabaseManager.h"
#include "../../../../../include/partons/utils/math/ErrorBar.h"
#include "../../../../../include/partons/utils/type/PhysicalType.h"

namespace PARTONS {

DVCSObservableResultDaoService::DVCSObservableResultDaoService() :
        ObservableResultDaoService("DVCSObservableResultDaoService") {

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //get last id of observable kinematics
    if (query.exec(
            "SELECT COUNT(dvcs_observable_kinematic_id) FROM dvcs_observable_kinematic")) {
        if (query.first()) {
            m_lastObservableKinematicId = query.value(0).toInt();
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    //get last if of observale result
    if (query.exec(
            "SELECT COUNT(dvcs_observable_result_id) FROM dvcs_observable_result")) {
        if (query.first()) {
            m_lastObservableResultId = query.value(0).toInt();
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();
}

DVCSObservableResultDaoService::~DVCSObservableResultDaoService() {
}

int DVCSObservableResultDaoService::insert(
        const DVCSObservableResult &observableResult) {

    //proceed via List
    List<DVCSObservableResult> results;
    results.add(observableResult);

    return insert(results);
}

int DVCSObservableResultDaoService::insert(
        const List<DVCSObservableResult> &resultList) {

    //for multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {

        //info
        info(__func__, "Prepare data before inserting them into database ...");
        info(__func__,
                ElemUtils::Formatter() << resultList.size()
                        << " ObservableResult(s) will be inserted ...");

        //loop over results
        for (unsigned int i = 0; i != resultList.size(); i++) {

            //prepare for result info
            prepareCommonTablesFromResultInfo(resultList[i].getResultInfo());

            //get kinematics
            DVCSObservableKinematic kinematic = resultList[i].getKinematic();

            //check if kinematics already in DB
            int kinematicId =
                    m_observableKinematicDaoService.getKinematicIdByHashSum(
                            kinematic.getHashSum());

            //if not, store it
            if (kinematicId == -1) {

                //increment counter and set last
                m_lastObservableKinematicId++;
                kinematicId = m_lastObservableKinematicId;

                //prepare query
                m_observableKinematicTableFile += ElemUtils::Formatter()
                        << m_lastObservableKinematicId << ","
                        << kinematic.getXB().getValue() << ","
                        << kinematic.getXB().getUnit() << ","
                        << kinematic.getT().getValue() << ","
                        << kinematic.getT().getUnit() << ","
                        << kinematic.getQ2().getValue() << ","
                        << kinematic.getQ2().getUnit() << ","
                        << kinematic.getE().getValue() << ","
                        << kinematic.getE().getUnit() << ","
                        << kinematic.getPhi().getValue() << ","
                        << kinematic.getPhi().getUnit() << "," << 0 << ","
                        << kinematic.getHashSum() << '\n';
            }

            //increment counter
            m_lastObservableResultId++;

            m_observableResultTableFile += ElemUtils::Formatter()
                    << m_lastObservableResultId << ","
                    << resultList[i].getComputationModuleName() << ","
                    << resultList[i].getValue().getValue() << ","
                    << resultList[i].getErrStat().getLowerBound() << ","
                    << resultList[i].getErrStat().getUpperBound() << ","
                    << resultList[i].getErrSys().getLowerBound() << ","
                    << resultList[i].getErrSys().getUpperBound() << ","
                    << resultList[i].getErrScale().getLowerBound() << ","
                    << resultList[i].getErrScale().getUpperBound() << ","
                    << resultList[i].getValue().getUnit() << "," << kinematicId
                    << "," << m_previousComputationId.second << '\n';
        }

        //insert
        insertCommonDataIntoDatabaseTables();

        insertDataIntoDatabaseTables("observableKinematicTableFile.csv",
                m_observableKinematicTableFile, "dvcs_observable_kinematic");
        insertDataIntoDatabaseTables("observableResultTableFile.csv",
                m_observableResultTableFile, "dvcs_observable_result");

        //if there is no exception we can commit all query
        QSqlDatabase::database().commit();

        //info
        info(__func__, "Done !");

    } catch (const std::exception &e) {

        //else return database in a stable state : n-1
        QSqlDatabase::database().rollback();

        //and throw exception
        throw ElemUtils::CustomException(getClassName(), __func__, e.what());
    }

    return getLastComputationId();
}

List<DVCSObservableResult> DVCSObservableResultDaoService::getObservableResultListByComputationId(
        const int computationId) const {
    return m_observableResultDao.getObservableResultListByComputationId(
            computationId);
}

List<DVCSObservableResult> DVCSObservableResultDaoService::getObservableResultListFromSQLQuery(
        const std::string& sqlQuery) const {
    return m_observableResultDao.getObservableResultListFromSQLQuery(sqlQuery);
}

} /* namespace PARTONS */
