#include "../../../../../include/partons/database/convol_coeff_function/service/DVCSConvolCoeffFunctionResultDaoService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqlquery.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>
#include <complex>
#include <map>
#include <string>
#include <utility>

#include "../../../../../include/partons/beans/convol_coeff_function/ConvolCoeffFunctionResult.h"
#include "../../../../../include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/Result.h"
#include "../../../../../include/partons/database/DatabaseManager.h"
#include "../../../../../include/partons/utils/type/PhysicalType.h"

namespace PARTONS {

DVCSConvolCoeffFunctionResultDaoService::DVCSConvolCoeffFunctionResultDaoService() :
        ConvolCoeffFunctionResultDaoService(
                "DVCSConvolCoeffFunctionResultDaoService") {

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //get last id of CCF kinematics
    if (query.exec("SELECT COUNT(ccf_kinematic_id) FROM dvcs_ccf_kinematic;")) {
        if (query.first()) {
            m_lastCCFKinematicProcessId = query.value(0).toInt();
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    //get last id of CCF result complex
    if (query.exec(
            "SELECT COUNT(ccf_result_complex_id) FROM dvcs_ccf_result;")) {
        if (query.first()) {
            m_lastCCFResultProcessId = query.value(0).toInt();
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();
}

DVCSConvolCoeffFunctionResultDaoService::~DVCSConvolCoeffFunctionResultDaoService() {
}

int DVCSConvolCoeffFunctionResultDaoService::insert(
        const DVCSConvolCoeffFunctionResult &dvcsConvolCoeffFunctionResult) {

    //proceed via List
    List<DVCSConvolCoeffFunctionResult> results;
    results.add(dvcsConvolCoeffFunctionResult);

    return insert(results);
}

int DVCSConvolCoeffFunctionResultDaoService::insert(
        const List<DVCSConvolCoeffFunctionResult>& resultList) {

    //for multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {

        //info
        info(__func__, "Prepare data before inserting them into database ...");
        info(__func__,
                ElemUtils::Formatter() << resultList.size()
                        << " ConvolCoeffFunctionResult(s) will be inserted ...");

        //loop over results
        for (unsigned int i = 0; i != resultList.size(); i++) {

            //prepare for result info
            prepareCommonTablesFromResultInfo(resultList[i].getResultInfo());

            //get kinematics
            DVCSConvolCoeffFunctionKinematic kinematic =
                    resultList[i].getKinematic();

            //check if kinematics already in DB
            int kinematicId =
                    m_dvcsConvolCoeffFunctionKinematicDaoService.getKinematicIdByHashSum(
                            kinematic.getHashSum());

            //if not, store it
            if (kinematicId == -1) {

                //increment counter and set last
                m_lastCCFKinematicProcessId++;
                kinematicId = m_lastCCFKinematicProcessId;

                //prepare query
                m_ccfKinematicTableFile += ElemUtils::Formatter()
                        << m_lastCCFKinematicProcessId << ","
                        << kinematic.getXi().getValue() << ","
                        << kinematic.getT().getValue() << ","
                        << kinematic.getQ2().getValue() << ","
                        << kinematic.getMuF2().getValue() << ","
                        << kinematic.getMuR2().getValue() << ","
                        << kinematic.getHashSum() << '\n';
            }

            //increment counter
            m_lastCCFResultId++;

            //prepare query
            m_ccfResultTableFile += ElemUtils::Formatter() << m_lastCCFResultId
                    << "," << resultList[i].getComputationModuleName() << ","
                    << resultList[i].getChannelType() << "," << kinematicId
                    << "," << m_previousComputationId.second << '\n';

            //get all results by GPDType
            std::map<GPDType::Type, std::complex<double> > resultsByGPDType =
                    resultList[i].getResultsByGpdType();

            //loop over GPDType to store objects into database
            for (std::map<GPDType::Type, std::complex<double> >::const_iterator it =
                    resultsByGPDType.begin(); it != resultsByGPDType.end();
                    it++) {

                //increment counter
                m_lastCCFResultProcessId++;

                //prepare query
                m_ccfResultComplexTableFile += ElemUtils::Formatter()
                        << m_lastCCFResultProcessId << "," << (it->first) << ","
                        << (it->second).real() << "," << (it->second).imag()
                        << "," << m_lastCCFResultId << '\n';
            }
        }

        //insert
        insertCommonDataIntoDatabaseTables();

        insertDataIntoDatabaseTables("ccfKinematicTableFile.csv",
                m_ccfKinematicTableFile, "ccf_kinematic");
        insertDataIntoDatabaseTables("ccfResultTableFile.csv",
                m_ccfResultTableFile, "ccf_result");
        insertDataIntoDatabaseTables("ccfResultComplexTableFile.csv",
                m_ccfResultComplexTableFile, "ccf_result_complex");

        //if there is no exception we can commit all query
        QSqlDatabase::database().commit();

        //info
        info(__func__, "Done !");

    } catch (const std::exception &e) {

        //else return database in a stable state : n-1
        QSqlDatabase::database().rollback();

        throw ElemUtils::CustomException(getClassName(), __func__, e.what());
    }

    //and throw exception
    return getLastComputationId();
}

List<DVCSConvolCoeffFunctionResult> DVCSConvolCoeffFunctionResultDaoService::getResultListByComputationId(
        const int computationId) const {

    //result info
    ResultInfo resultInfo = m_resultInfoDaoService.getResultInfoByComputationId(
            computationId);

    //result
    List<DVCSConvolCoeffFunctionResult> results =
            m_convolCoeffFunctionResultDao.getResultListByComputationId(
                    computationId);

    //set result info
    for (unsigned int i = 0; i != results.size(); i++) {
        results[i].setResultInfo(resultInfo);
    }

    return results;
}

} /* namespace PARTONS */
