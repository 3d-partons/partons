#include "../../../../../include/partons/database/observable/service/DVMPObservableKinematicDaoService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <QtSql/qsqldatabase.h>

namespace PARTONS {

DVMPObservableKinematicDaoService::DVMPObservableKinematicDaoService() :
        ObservableKinematicDaoService("DVMPObservableKinematicDaoService") {
}

DVMPObservableKinematicDaoService::~DVMPObservableKinematicDaoService() {
}

int DVMPObservableKinematicDaoService::insert(
        const DVMPObservableKinematic &observableKinematic) const {

    //result
    int kinematicId = -1;

    //for multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {

        //test
        kinematicId = insertWithoutTransaction(observableKinematic);

        //if there is no exception we can commit all query
        QSqlDatabase::database().commit();

    } catch (const std::exception &e) {

        //else return database in a stable state : n-1
        QSqlDatabase::database().rollback();

        throw ElemUtils::CustomException(getClassName(), __func__, e.what());
    }

    return kinematicId;
}

int DVMPObservableKinematicDaoService::insert(
        const List<DVMPObservableKinematic>& observableKinematicList) const {

    //result
    int kinematicId = -1;

    //for multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {

        //test
        for (unsigned int i = 0; i != observableKinematicList.size(); i++) {
            kinematicId = insertWithoutTransaction(
                    observableKinematicList.get(i));
        }

        //if there is no exception we can commit all query
        QSqlDatabase::database().commit();

    } catch (const std::exception &e) {

        //else return database in a stable state : n-1
        QSqlDatabase::database().rollback();

        throw ElemUtils::CustomException(getClassName(), __func__, e.what());
    }

    return kinematicId;
}

int DVMPObservableKinematicDaoService::getIdByKinematicObject(
        const DVMPObservableKinematic& observableKinematic) const {
    return m_dvmpObservableKinematicDao.select(observableKinematic.getXB(),
            observableKinematic.getT(), observableKinematic.getQ2(),
            observableKinematic.getE(), observableKinematic.getPhi(),
            observableKinematic.getMesonType());
}

List<DVMPObservableKinematic> DVMPObservableKinematicDaoService::getKinematicListByComputationId(
        int computationId) const {
    return m_dvmpObservableKinematicDao.getKinematicListByComputationId(
            computationId);
}

DVMPObservableKinematic DVMPObservableKinematicDaoService::getKinematicById(
        const int kinematicId) const {
    return m_dvmpObservableKinematicDao.getKinematicById(kinematicId);
}

int DVMPObservableKinematicDaoService::insertWithoutTransaction(
        const DVMPObservableKinematic& observableKinematic) const {
    return m_dvmpObservableKinematicDao.insert(observableKinematic.getXB(),
            observableKinematic.getT(), observableKinematic.getQ2(),
            observableKinematic.getE(), observableKinematic.getPhi(),
            observableKinematic.getMesonType(),
            observableKinematic.getHashSum());
}

int DVMPObservableKinematicDaoService::getKinematicIdByHashSum(
        const std::string& hashSum) const {
    return m_dvmpObservableKinematicDao.getKinematicIdByHashSum(hashSum);
}

} /* namespace PARTONS */
