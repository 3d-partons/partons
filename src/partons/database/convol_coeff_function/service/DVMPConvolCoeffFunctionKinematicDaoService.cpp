#include "../../../../../include/partons/database/convol_coeff_function/service/DVMPConvolCoeffFunctionKinematicDaoService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <QtSql/qsqldatabase.h>

#include "../../../../../include/partons/utils/type/PhysicalType.h"

namespace PARTONS {

DVMPConvolCoeffFunctionKinematicDaoService::DVMPConvolCoeffFunctionKinematicDaoService() :
        ConvolCoeffFunctionKinematicDaoService(
                "DVMPConvolCoeffFunctionKinematicDaoService") {
}

DVMPConvolCoeffFunctionKinematicDaoService::~DVMPConvolCoeffFunctionKinematicDaoService() {
}

int DVMPConvolCoeffFunctionKinematicDaoService::insert(
        const DVMPConvolCoeffFunctionKinematic& ccfKinematic) const {

    //result
    int kinematicId = -1;

    //for multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {

        //test
        kinematicId = insertWithoutTransaction(ccfKinematic);

        //if there is no exception we can commit all query
        QSqlDatabase::database().commit();

    } catch (const std::exception &e) {

        //else return database in a stable state : n-1
        QSqlDatabase::database().rollback();

        throw ElemUtils::CustomException(getClassName(), __func__, e.what());
    }

    return kinematicId;
}

int DVMPConvolCoeffFunctionKinematicDaoService::insert(
        const List<DVMPConvolCoeffFunctionKinematic>& ccfKinematicList) const {

    //result
    int kinematicId = -1;

    //for multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {

        //test
        for (unsigned int i = 0; i != ccfKinematicList.size(); i++) {
            kinematicId = insertWithoutTransaction(ccfKinematicList.get(i));
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

int DVMPConvolCoeffFunctionKinematicDaoService::getIdByKinematicObject(
        const DVMPConvolCoeffFunctionKinematic& ccfKinematic) const {
    return m_dvmpConvolCoeffFunctionKinematicDao.select(ccfKinematic.getXi(),
            ccfKinematic.getT(), ccfKinematic.getQ2(), ccfKinematic.getMuF2(),
            ccfKinematic.getMuR2(), ccfKinematic.getMesonType(),
            ccfKinematic.getMesonPolarization());
}

DVMPConvolCoeffFunctionKinematic DVMPConvolCoeffFunctionKinematicDaoService::getKinematicById(
        const int id) const {
    return m_dvmpConvolCoeffFunctionKinematicDao.getKinematicById(id);
}

int DVMPConvolCoeffFunctionKinematicDaoService::insertWithoutTransaction(
        const DVMPConvolCoeffFunctionKinematic& ccfKinematic) const {
    return m_dvmpConvolCoeffFunctionKinematicDao.insert(ccfKinematic.getXi(),
            ccfKinematic.getT(), ccfKinematic.getQ2(), ccfKinematic.getMuF2(),
            ccfKinematic.getMuR2(), ccfKinematic.getMesonType(),
            ccfKinematic.getMesonPolarization(), ccfKinematic.getHashSum());
}

List<DVMPConvolCoeffFunctionKinematic> DVMPConvolCoeffFunctionKinematicDaoService::getKinematicListByComputationId(
        int computationId) const {
    return m_dvmpConvolCoeffFunctionKinematicDao.getKinematicListByComputationId(
            computationId);
}

int DVMPConvolCoeffFunctionKinematicDaoService::getKinematicIdByHashSum(
        const std::string& hashSum) const {
    return m_dvmpConvolCoeffFunctionKinematicDao.getKinematicIdByHashSum(
            hashSum);
}

} /* namespace PARTONS */
