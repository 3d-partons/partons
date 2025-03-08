#include "../../../../../include/partons/database/convol_coeff_function/service/DDVCSConvolCoeffFunctionKinematicDaoService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <QtSql/qsqldatabase.h>

#include "../../../../../include/partons/utils/type/PhysicalType.h"

namespace PARTONS {

DDVCSConvolCoeffFunctionKinematicDaoService::DDVCSConvolCoeffFunctionKinematicDaoService() :
        ConvolCoeffFunctionKinematicDaoService(
                "DDVCSConvolCoeffFunctionKinematicDaoService") {
}

DDVCSConvolCoeffFunctionKinematicDaoService::~DDVCSConvolCoeffFunctionKinematicDaoService() {
}

int DDVCSConvolCoeffFunctionKinematicDaoService::insert(
        const DDVCSConvolCoeffFunctionKinematic& ccfKinematic) const {

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

int DDVCSConvolCoeffFunctionKinematicDaoService::insert(
        const List<DDVCSConvolCoeffFunctionKinematic>& ccfKinematicList) const {

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

int DDVCSConvolCoeffFunctionKinematicDaoService::getIdByKinematicObject(
        const DDVCSConvolCoeffFunctionKinematic& ccfKinematic) const {
    return m_ddvcsConvolCoeffFunctionKinematicDao.select(ccfKinematic.getXi(),
            ccfKinematic.getT(), ccfKinematic.getQ2(), ccfKinematic.getQ2Prim(), ccfKinematic.getMuF2(),
            ccfKinematic.getMuR2());
}

DDVCSConvolCoeffFunctionKinematic DDVCSConvolCoeffFunctionKinematicDaoService::getKinematicById(
        const int id) const {
    return m_ddvcsConvolCoeffFunctionKinematicDao.getKinematicById(id);
}

int DDVCSConvolCoeffFunctionKinematicDaoService::insertWithoutTransaction(
        const DDVCSConvolCoeffFunctionKinematic& ccfKinematic) const {
    return m_ddvcsConvolCoeffFunctionKinematicDao.insert(ccfKinematic.getXi(),
            ccfKinematic.getT(), ccfKinematic.getQ2(), ccfKinematic.getQ2Prim(), ccfKinematic.getMuF2(),
            ccfKinematic.getMuR2(), ccfKinematic.getHashSum());
}

List<DDVCSConvolCoeffFunctionKinematic> DDVCSConvolCoeffFunctionKinematicDaoService::getKinematicListByComputationId(
        int computationId) const {
    return m_ddvcsConvolCoeffFunctionKinematicDao.getKinematicListByComputationId(
            computationId);
}

int DDVCSConvolCoeffFunctionKinematicDaoService::getKinematicIdByHashSum(
        const std::string& hashSum) const {
    return m_ddvcsConvolCoeffFunctionKinematicDao.getKinematicIdByHashSum(
            hashSum);
}

} /* namespace PARTONS */
