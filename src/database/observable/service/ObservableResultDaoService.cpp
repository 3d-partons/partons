#include "ObservableResultDaoService.h"

#include <QtSql/qsqldatabase.h>
#include <stddef.h>
#include <exception>

#include "../../../beans/observable/ObservableResult.h"
#include "../../../beans/observable/ObservableResultList.h"
#include "../../../utils/math/ErrorBar.h"
#include "../dao/ObservableResultDao.h"

ObservableResultDaoService::ObservableResultDaoService() :
        BaseObject("ObservableResultDaoService") {
}

ObservableResultDaoService::~ObservableResultDaoService() {
}

int ObservableResultDaoService::insert(
        const ObservableResult& observableResult) const {

    // Check if this kinematic already exists
    int kinematicId = m_observableKinematicDaoService.getKinematicId(
            observableResult.getKinematic());
    // If not, insert new entry in database and retrieve its id
    if (kinematicId == -1) {
        kinematicId = m_observableKinematicDaoService.insert(
                observableResult.getKinematic());
    }

    // Check if this computation date already exists and retrieve Id
    int computationId = m_commonDaoService.getComputationId(
            observableResult.getDateTime());
    // If not, insert new entry in database and retrieve its id
    if (computationId == -1) {
        computationId = m_commonDaoService.insertComputation(
                observableResult.getDateTime());
    }

    ObservableResultDao::insert(observableResult.getObservbleName(),
            observableResult.getValue(), observableResult.getPhi(),
            observableResult.getStatError().getLowerBound(),
            observableResult.getStatError().getUpperBound(),
            observableResult.getSystError().getLowerBound(),
            observableResult.getSystError().getUpperBound(),
            observableResult.getTotalError(), kinematicId, computationId);

    return computationId;
}

int ObservableResultDaoService::insert(
        const ObservableResultList &observableResultList) {
    int result = -1;

    // For multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {
        // for each result
        for (size_t i = 0; i != observableResultList.size(); i++) {
            result = insert(observableResultList[i]);
        }

        // If there is no exception we can commit all query
        QSqlDatabase::database().commit();

    } catch (std::exception &e) {

        error(__func__, e.what());

        // Else return database in a stable state : n-1
        QSqlDatabase::database().rollback();
    }

    return result;
}
