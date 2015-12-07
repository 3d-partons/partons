#include "../../../../../include/partons/database/observable/service/ObservableResultDaoService.h"

#include <QtSql/qsqldatabase.h>
#include <stddef.h>
#include <exception>

#include "../../../../../include/partons/beans/List.h"
#include "../../../../../include/partons/utils/math/ErrorBar.h"
#include "../../../../../include/partons/utils/stringUtils/Formatter.h"

ObservableResultDaoService::ObservableResultDaoService() :
        BaseObject("ObservableResultDaoService") {
}

ObservableResultDaoService::~ObservableResultDaoService() {
}

int ObservableResultDaoService::insert(
        const ObservableResult &observableResult) const {

    int result = -1;

    // For multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {

        result = insertWithoutTransaction(observableResult);

        // If there is no exception we can commit all query
        QSqlDatabase::database().commit();

    } catch (std::exception &e) {
        error(__func__, e.what());

        // Else return database in a stable state : n-1
        QSqlDatabase::database().rollback();
    }

    return result;
}

int ObservableResultDaoService::insertWithoutTransaction(
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
            observableResult.getComputationDateTime());
    // If not, insert new entry in database and retrieve its id
    if (computationId == -1) {
        computationId = m_commonDaoService.insertComputation(
                observableResult.getComputationDateTime());
    }

    //Then store observableResult in database
    m_observableResultDao.insert(observableResult.getObservableName(),
            observableResult.getValue(),
            observableResult.getStatError().getLowerBound(),
            observableResult.getStatError().getUpperBound(),
            observableResult.getSystError().getLowerBound(),
            observableResult.getSystError().getUpperBound(),
            observableResult.getTotalError(),
            observableResult.getComputationModuleName(),
            observableResult.getObservableType(), kinematicId, computationId);

    return computationId;
}

int ObservableResultDaoService::insert(
        const ResultList<ObservableResult> &observableResultList) {
    int result = -1;

    info(__func__,
            Formatter() << "Inserting object size = "
                    << observableResultList.size());

    // For multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {
        // for each result
        for (size_t i = 0; i != observableResultList.size(); i++) {
            result = insertWithoutTransaction(observableResultList[i]);
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

ResultList<ObservableResult> ObservableResultDaoService::getObservableResultListByComputationId(
        const int computationId) const {
    return m_observableResultDao.getObservableResultListByComputationId(
            computationId);
}
