#include "../../../../../include/partons/database/convol_coeff_function/service/ConvolCoeffFunctionResultDaoService.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtSql/qsqldatabase.h>
#include <stddef.h>
#include <complex>
#include <exception>
#include <map>
#include <utility>

#include "../../../../../include/partons/beans/Computation.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
//#include "../../../../../include/partons/beans/List.h"
#include "../../../../../include/partons/beans/system/ResultInfo.h"

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

int ConvolCoeffFunctionResultDaoService::insert(
        const List<DVCSConvolCoeffFunctionResult>& resultList) {

    info(__func__,
            ElemUtils::Formatter() << "Inserting object size = "
                    << resultList.size());

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

List<DVCSConvolCoeffFunctionResult> ConvolCoeffFunctionResultDaoService::getResultListByComputationId(
        const int computationId) const {
    return m_convolCoeffFunctionResultDao.getResultListByComputationId(
            computationId);
}

int ConvolCoeffFunctionResultDaoService::insertWithoutTransaction(
        const DVCSConvolCoeffFunctionResult& result) const {

    // Check if this kinematic already exists
    int kinematicId =
            m_convolCoeffFunctionKinematicDaoService.getIdByKinematicObject(
                    result.getKinematic());
    // If not, insert new entry into database and retrieve its id
    if (kinematicId == -1) {
        kinematicId = m_convolCoeffFunctionKinematicDaoService.insert(
                result.getKinematic());
    }

    // Check if this computation date already exists
    int computationId = m_computationDaoService.getComputationIdByDateTime(
            result.getResultInfo().getComputation().getDateTime());
    // If not, insert new entry into database and retrieve its id
    if (computationId == -1) {
        computationId = m_computationDaoService.insertWithoutTransaction(
                result.getResultInfo().getComputation());
    }

    // Insert new ccf_result entry into database and retrieve its id
    int resultId = m_convolCoeffFunctionResultDao.insert(
            result.getComputationModuleName(), kinematicId, computationId);

    // Get all CCF results indexed by GPDType
    std::map<GPDType::Type, std::complex<double> > resultsByGPDType =
            result.getResultsByGpdType();

    // Then loop over GPDType to store real part and imaginary part from CFF result into database.
    for (std::map<GPDType::Type, std::complex<double> >::const_iterator it =
            resultsByGPDType.begin(); it != resultsByGPDType.end(); it++) {
        m_convolCoeffFunctionResultDao.insertIntoCCFResultComplex(
                (it->second).real(), (it->second).imag(), (it->first),
                resultId);
    }

    return computationId;
}
