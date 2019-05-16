#include "../../../../../include/partons/database/observable/service/TCSObservableKinematicDaoService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <QtSql/qsqldatabase.h>

namespace PARTONS {

TCSObservableKinematicDaoService::TCSObservableKinematicDaoService() :
        ObservableKinematicDaoService("TCSObservableKinematicDaoService") {
}

TCSObservableKinematicDaoService::~TCSObservableKinematicDaoService() {
}

int TCSObservableKinematicDaoService::insert(
        const TCSObservableKinematic &observableKinematic) const {

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

int TCSObservableKinematicDaoService::insert(
        const List<TCSObservableKinematic>& observableKinematicList) const {

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

int TCSObservableKinematicDaoService::getIdByKinematicObject(
        const TCSObservableKinematic& observableKinematic) const {
    return m_tcsObservableKinematicDao.select(observableKinematic.getXB(),
            observableKinematic.getT(), observableKinematic.getQ2Prim(),
            observableKinematic.getE(), observableKinematic.getPhi(),
            observableKinematic.getTheta());
}

List<TCSObservableKinematic> TCSObservableKinematicDaoService::getKinematicListByComputationId(
        int computationId) const {
    return m_tcsObservableKinematicDao.getKinematicListByComputationId(
            computationId);
}

TCSObservableKinematic TCSObservableKinematicDaoService::getKinematicById(
        const int kinematicId) const {
    return m_tcsObservableKinematicDao.getKinematicById(kinematicId);
}

int TCSObservableKinematicDaoService::insertWithoutTransaction(
        const TCSObservableKinematic& observableKinematic) const {
    return m_tcsObservableKinematicDao.insert(observableKinematic.getXB(),
            observableKinematic.getT(), observableKinematic.getQ2Prim(),
            observableKinematic.getE(), observableKinematic.getPhi(),
            observableKinematic.getTheta(), observableKinematic.getHashSum());
}

int TCSObservableKinematicDaoService::getKinematicIdByHashSum(
        const std::string& hashSum) const {
    return m_tcsObservableKinematicDao.getKinematicIdByHashSum(hashSum);
}

} /* namespace PARTONS */
