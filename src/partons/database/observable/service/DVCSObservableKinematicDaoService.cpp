#include "../../../../../include/partons/database/observable/service/DVCSObservableKinematicDaoService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <QtSql/qsqldatabase.h>

namespace PARTONS {

DVCSObservableKinematicDaoService::DVCSObservableKinematicDaoService() :
        ObservableKinematicDaoService("DVCSObservableKinematicDaoService") {
}

DVCSObservableKinematicDaoService::~DVCSObservableKinematicDaoService() {
}

int DVCSObservableKinematicDaoService::insert(
        const DVCSObservableKinematic &observableKinematic) const {

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

int DVCSObservableKinematicDaoService::insert(
        const List<DVCSObservableKinematic>& observableKinematicList) const {

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

int DVCSObservableKinematicDaoService::getIdByKinematicObject(
        const DVCSObservableKinematic& observableKinematic) const {
    return m_dvcsObservableKinematicDao.select(observableKinematic.getXB(),
            observableKinematic.getT(), observableKinematic.getQ2(),
            observableKinematic.getE(), observableKinematic.getPhi());
}

List<DVCSObservableKinematic> DVCSObservableKinematicDaoService::getKinematicListByComputationId(
        int computationId) const {
    return m_dvcsObservableKinematicDao.getKinematicListByComputationId(
            computationId);
}

DVCSObservableKinematic DVCSObservableKinematicDaoService::getKinematicById(
        const int kinematicId) const {
    return m_dvcsObservableKinematicDao.getKinematicById(kinematicId);
}

int DVCSObservableKinematicDaoService::insertWithoutTransaction(
        const DVCSObservableKinematic& observableKinematic) const {
    return m_dvcsObservableKinematicDao.insert(observableKinematic.getXB(),
            observableKinematic.getT(), observableKinematic.getQ2(),
            observableKinematic.getE(), observableKinematic.getPhi());
}

int DVCSObservableKinematicDaoService::getKinematicIdByHashSum(
        const std::string& hashSum) const {
    return m_dvcsObservableKinematicDao.getKinematicIdByHashSum(hashSum);
}

} /* namespace PARTONS */
