#include "ConvolCoeffFunctionResultDaoService.h"

#include <QtSql/qsqldatabase.h>
#include <stddef.h>
#include <complex>
#include <exception>
#include <map>
#include <utility>

#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/List.h"
#include "../../../utils/stringUtils/Formatter.h"

ConvolCoeffFunctionResultDaoService::ConvolCoeffFunctionResultDaoService() :
        BaseObject("ConvolCoeffFunctionResultDaoService") {
}

ConvolCoeffFunctionResultDaoService::~ConvolCoeffFunctionResultDaoService() {
}

int ConvolCoeffFunctionResultDaoService::insert(
        const DVCSConvolCoeffFunctionResult &dvcsConvolCoeffFunctionResult) {

    int result = -1;

    // For multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {

        result = insertWithoutTransaction(dvcsConvolCoeffFunctionResult);

        // If there is no exception we can commit all query
        QSqlDatabase::database().commit();

    } catch (std::exception &e) {
        error(__func__, e.what());

        // Else return database in a stable state : n-1
        QSqlDatabase::database().rollback();
    }

    return result;
}

int ConvolCoeffFunctionResultDaoService::insertWithoutTransaction(
        const DVCSConvolCoeffFunctionResult& result) const {

    // Check if this kinematic already exists
    int kinematicId = m_convolCoeffFunctionKinematicDaoService.getKinematicId(
            result.getKinematic());
    // If not, insert new entry into database and retrieve its id
    if (kinematicId == -1) {
        kinematicId = m_convolCoeffFunctionKinematicDaoService.insert(
                result.getKinematic());
    }

    // Check if this computation date already exists and retrieve Id
    int computationId = m_commonDaoService.getComputationId(
            result.getComputationDateTime());
    // If not, insert new entry into database and retrieve its id
    if (computationId == -1) {
        computationId = m_commonDaoService.insertComputation(
                result.getComputationDateTime());
    }

    int resultId = m_convolCoeffFunctionResultDao.insert(
            result.getComputationModuleName(), kinematicId, computationId);

    std::map<GPDType::Type, std::complex<double> > resultsByGPDType =
            result.getResultsByGpdType();
    std::map<GPDType::Type, std::complex<double> >::const_iterator it;

    for (it = resultsByGPDType.begin(); it != resultsByGPDType.end(); it++) {

        // Check if complex already exists
        int complexId = m_complexDaoService.getComplexId((it->second));
        if (complexId == -1) {
            // If not, insert new entry into database and retrieve its id
            complexId = m_complexDaoService.insert((it->second));
        }

        m_convolCoeffFunctionResultDao.insertIntoCCFResultComplex((it->first),
                resultId, complexId);
    }

    return computationId;
}

int ConvolCoeffFunctionResultDaoService::insert(
        const ResultList<DVCSConvolCoeffFunctionResult>& resultList) {

    info(__func__,
            Formatter() << "Inserting object size = " << resultList.size());

    int result = -1;

    // For multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {
        // for each result
        for (size_t i = 0; i != resultList.size(); i++) {
            result = insertWithoutTransaction(resultList[i]);
        }

        // If there is no exception we can commit all query
        QSqlDatabase::database().commit();

    } catch (std::exception &e) {
        // Else return database in a stable state : n-1
        QSqlDatabase::database().rollback();
    }

    return result;
}

ResultList<DVCSConvolCoeffFunctionResult> ConvolCoeffFunctionResultDaoService::getResultListByComputationId(
        const int computationId) const {
    return m_convolCoeffFunctionResultDao.getResultListByComputationId(
            computationId);
}
