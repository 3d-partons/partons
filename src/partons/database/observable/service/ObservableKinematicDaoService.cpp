#include "../../../../../include/partons/database/observable/service/ObservableKinematicDaoService.h"

#include <QtSql/qsqldatabase.h>
#include <exception>

#include "../../../../../include/partons/utils/type/PhysicalType.h"

ObservableKinematicDaoService::ObservableKinematicDaoService() :
        BaseObject("ObservableKinematicDaoService") {
}

ObservableKinematicDaoService::~ObservableKinematicDaoService() {
}

int ObservableKinematicDaoService::insert(
        const ObservableKinematic &observableKinematic) const {
    int kinematicId = -1;

    // For multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {
        kinematicId = insertWithoutTransaction(observableKinematic);

        // If there is no exception we can commit all query
        QSqlDatabase::database().commit();

    } catch (std::exception &e) {
        error(__func__, e.what());

        // Else return database in a stable state : n-1
        QSqlDatabase::database().rollback();
    }

    return kinematicId;
}

int ObservableKinematicDaoService::insert(
        const List<ObservableKinematic>& observableKinematicList) const {
    int kinematicId = -1;

    // For multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {

        for (unsigned int i = 0; i != observableKinematicList.size(); i++) {
            kinematicId = insertWithoutTransaction(
                    observableKinematicList.get(i));
        }

        // If there is no exception we can commit all query
        QSqlDatabase::database().commit();

    } catch (std::exception &e) {
        error(__func__, e.what());

        // Else return database in a stable state : n-1
        QSqlDatabase::database().rollback();
    }

    return kinematicId;
}

int ObservableKinematicDaoService::getIdByKinematicObject(
        const ObservableKinematic& observableKinematic) const {
    return m_observableKinematicDao.select(observableKinematic.getXB(),
            observableKinematic.getT(), observableKinematic.getQ2(),
            observableKinematic.getPhi().getValue());
}

List<ObservableKinematic> ObservableKinematicDaoService::getKinematicListByComputationId(
        int computationId) const {
    return m_observableKinematicDao.getKinematicListByComputationId(
            computationId);
}

ObservableKinematic ObservableKinematicDaoService::getKinematicById(
        const int kinematicId) const {
    return m_observableKinematicDao.getKinematicById(kinematicId);
}

int ObservableKinematicDaoService::insertWithoutTransaction(
        const ObservableKinematic& observableKinematic) const {
    return m_observableKinematicDao.insert(observableKinematic.getXB(),
            observableKinematic.getT(), observableKinematic.getQ2(),
            observableKinematic.getPhi().getValue());
}
