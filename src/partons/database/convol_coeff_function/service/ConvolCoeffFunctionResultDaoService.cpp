#include "../../../../../include/partons/database/convol_coeff_function/service/ConvolCoeffFunctionResultDaoService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlquery.h>
#include <complex>
#include <map>
#include <utility>

#include "../../../../../include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../../../../../include/partons/beans/Computation.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../include/partons/beans/system/ResultInfo.h"
#include "../../../../../include/partons/database/DatabaseManager.h"

ConvolCoeffFunctionResultDaoService::ConvolCoeffFunctionResultDaoService() :
        ResultDaoService("ConvolCoeffFunctionResultDaoService"), m_lastCCFKinematicId(
                -1), m_lastCCFResultId(-1), m_lastCCFResultComplexId(-1), m_ccfKinematicTableFile(
                ElemUtils::StringUtils::EMPTY), m_ccfResultTableFile(
                ElemUtils::StringUtils::EMPTY) {
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    if (query.exec("SELECT COUNT(ccf_kinematic_id) FROM ccf_kinematic;")) {
        if (query.first()) {
            m_lastCCFKinematicId = query.value(0).toInt();
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }
    query.clear();

    if (query.exec("SELECT COUNT(ccf_result_id) FROM ccf_result;")) {
        if (query.first()) {
            m_lastCCFResultId = query.value(0).toInt();
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }
    query.clear();

    if (query.exec(
            "SELECT COUNT(ccf_result_complex_id) FROM ccf_result_complex;")) {
        if (query.first()) {
            m_lastCCFResultComplexId = query.value(0).toInt();
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }
    query.clear();
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
        throw ElemUtils::CustomException(getClassName(), __func__, e.what());

        // Else return database in a stable state : n-1
        QSqlDatabase::database().rollback();
    }

    return result;
}

int ConvolCoeffFunctionResultDaoService::insert(
        const List<DVCSConvolCoeffFunctionResult>& resultList) {
    info(__func__, "Prepare data before inserting them into database ...");
    info(__func__,
            ElemUtils::Formatter() << resultList.size()
                    << " ConvolCoeffFunctionResult(s) will be inserted ...");

    for (unsigned int i = 0; i != resultList.size(); i++) {

        prepareCommonTablesFromResultInfo(resultList[i].getResultInfo());

        DVCSConvolCoeffFunctionKinematic kinematic =
                resultList[i].getKinematic();

        int kinematicId =
                m_convolCoeffFunctionKinematicDaoService.getKinematicIdByHashSum(
                        kinematic.getHashSum());

        if (kinematicId == -1) {
            m_lastCCFKinematicId++;
            kinematicId = m_lastCCFKinematicId;

            m_ccfKinematicTableFile += ElemUtils::Formatter()
                    << m_lastCCFKinematicId << "," << kinematic.getXi() << ","
                    << kinematic.getT() << "," << kinematic.getQ2() << ","
                    << kinematic.getMuF2() << "," << kinematic.getMuR2() << ","
                    << kinematic.getHashSum() << '\n';
        }

        m_lastCCFResultId++;
        m_ccfResultTableFile += ElemUtils::Formatter() << m_lastCCFResultId
                << "," << resultList[i].getComputationModuleName() << ","
                << resultList[i].getChannel().getType() << "," << kinematicId
                << "," << m_previousComputationId.second << '\n';

        std::map<GPDType::Type, std::complex<double> > resultsByGPDType =
                resultList[i].getResultsByGpdType();

        for (std::map<GPDType::Type, std::complex<double> >::const_iterator it =
                resultsByGPDType.begin(); it != resultsByGPDType.end(); it++) {

            m_lastCCFResultComplexId++;
            m_ccfResultComplexTableFile += ElemUtils::Formatter()
                    << m_lastCCFResultComplexId << "," << (it->first) << ","
                    << (it->second).real() << "," << (it->second).imag() << ","
                    << m_lastCCFResultId << '\n';
        }
    }

    insertCommonDataIntoDatabaseTables();

    insertDataIntoDatabaseTables("ccfKinematicTableFile.csv",
            m_ccfKinematicTableFile, "ccf_kinematic");
    insertDataIntoDatabaseTables("ccfResultTableFile.csv", m_ccfResultTableFile,
            "ccf_result");
    insertDataIntoDatabaseTables("ccfResultComplexTableFile.csv",
            m_ccfResultComplexTableFile, "ccf_result_complex");

    info(__func__, "Done !");

    return getLastComputationId();
}

List<DVCSConvolCoeffFunctionResult> ConvolCoeffFunctionResultDaoService::getResultListByComputationId(
        const int computationId) const {
    return m_convolCoeffFunctionResultDao.getResultListByComputationId(
            computationId);
    ResultInfo resultInfo = m_resultInfoDaoService.getResultInfoByComputationId(
            computationId);

    List<DVCSConvolCoeffFunctionResult> results =
            m_convolCoeffFunctionResultDao.getResultListByComputationId(
                    computationId);

    for (unsigned int i = 0; i != results.size(); i++) {
        results[i].setResultInfo(resultInfo);
    }

    return results;
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
