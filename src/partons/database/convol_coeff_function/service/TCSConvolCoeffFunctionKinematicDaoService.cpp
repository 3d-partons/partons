#include "../../../../../include/partons/database/convol_coeff_function/service/TCSConvolCoeffFunctionKinematicDaoService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <QtSql/qsqldatabase.h>

#include "../../../../../include/partons/utils/type/PhysicalType.h"

namespace PARTONS {

TCSConvolCoeffFunctionKinematicDaoService::TCSConvolCoeffFunctionKinematicDaoService() :
        ConvolCoeffFunctionKinematicDaoService(
                "TCSConvolCoeffFunctionKinematicDaoService") {
}

TCSConvolCoeffFunctionKinematicDaoService::~TCSConvolCoeffFunctionKinematicDaoService() {
}

int TCSConvolCoeffFunctionKinematicDaoService::insert(
        const TCSConvolCoeffFunctionKinematic& ccfKinematic) const {

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

int TCSConvolCoeffFunctionKinematicDaoService::insert(
        const List<TCSConvolCoeffFunctionKinematic>& ccfKinematicList) const {

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

int TCSConvolCoeffFunctionKinematicDaoService::getIdByKinematicObject(
        const TCSConvolCoeffFunctionKinematic& ccfKinematic) const {
    return m_tcsConvolCoeffFunctionKinematicDao.select(ccfKinematic.getXi(),
            ccfKinematic.getT(), ccfKinematic.getQ2Prim(),
            ccfKinematic.getMuF2(), ccfKinematic.getMuR2());
}

TCSConvolCoeffFunctionKinematic TCSConvolCoeffFunctionKinematicDaoService::getKinematicById(
        const int id) const {
    return m_tcsConvolCoeffFunctionKinematicDao.getKinematicById(id);
}

int TCSConvolCoeffFunctionKinematicDaoService::insertWithoutTransaction(
        const TCSConvolCoeffFunctionKinematic& ccfKinematic) const {
    return m_tcsConvolCoeffFunctionKinematicDao.insert(ccfKinematic.getXi(),
            ccfKinematic.getT(), ccfKinematic.getQ2Prim(),
            ccfKinematic.getMuF2(), ccfKinematic.getMuR2(),
            ccfKinematic.getHashSum());
}

List<TCSConvolCoeffFunctionKinematic> TCSConvolCoeffFunctionKinematicDaoService::getKinematicListByComputationId(
        int computationId) const {
    return m_tcsConvolCoeffFunctionKinematicDao.getKinematicListByComputationId(
            computationId);
}

int TCSConvolCoeffFunctionKinematicDaoService::getKinematicIdByHashSum(
        const std::string& hashSum) const {
    return m_tcsConvolCoeffFunctionKinematicDao.getKinematicIdByHashSum(hashSum);
}

} /* namespace PARTONS */
