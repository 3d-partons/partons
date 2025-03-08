#include "../../../../../include/partons/database/observable/service/DDVCSObservableKinematicDaoService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <QtSql/qsqldatabase.h>

namespace PARTONS {

DDVCSObservableKinematicDaoService::DDVCSObservableKinematicDaoService() :
        ObservableKinematicDaoService("DDVCSObservableKinematicDaoService") {
}

DDVCSObservableKinematicDaoService::~DDVCSObservableKinematicDaoService() {
}

int DDVCSObservableKinematicDaoService::insert(
        const DDVCSObservableKinematic &observableKinematic) const {

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

int DDVCSObservableKinematicDaoService::insert(
        const List<DDVCSObservableKinematic>& observableKinematicList) const {

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

int DDVCSObservableKinematicDaoService::getIdByKinematicObject(
        const DDVCSObservableKinematic& observableKinematic) const {
    return m_ddvcsObservableKinematicDao.select(observableKinematic.getXB(),
            observableKinematic.getT(), observableKinematic.getQ2(), observableKinematic.getQ2Prim(),
            observableKinematic.getE(), observableKinematic.getPhi(), observableKinematic.getPhiL(), observableKinematic.getThetaL());
}

List<DDVCSObservableKinematic> DDVCSObservableKinematicDaoService::getKinematicListByComputationId(
        int computationId) const {
    return m_ddvcsObservableKinematicDao.getKinematicListByComputationId(
            computationId);
}

DDVCSObservableKinematic DDVCSObservableKinematicDaoService::getKinematicById(
        const int kinematicId) const {
    return m_ddvcsObservableKinematicDao.getKinematicById(kinematicId);
}

int DDVCSObservableKinematicDaoService::insertWithoutTransaction(
        const DDVCSObservableKinematic& observableKinematic) const {
    return m_ddvcsObservableKinematicDao.insert(observableKinematic.getXB(),
            observableKinematic.getT(), observableKinematic.getQ2(), observableKinematic.getQ2Prim(),
            observableKinematic.getE(), observableKinematic.getPhi(), observableKinematic.getPhiL(), observableKinematic.getThetaL(),
            observableKinematic.getHashSum());
}

int DDVCSObservableKinematicDaoService::getKinematicIdByHashSum(
        const std::string& hashSum) const {
    return m_ddvcsObservableKinematicDao.getKinematicIdByHashSum(hashSum);
}

} /* namespace PARTONS */
