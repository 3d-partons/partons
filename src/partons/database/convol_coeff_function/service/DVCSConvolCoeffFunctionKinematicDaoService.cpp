#include "../../../../../include/partons/database/convol_coeff_function/service/DVCSConvolCoeffFunctionKinematicDaoService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <QtSql/qsqldatabase.h>

#include "../../../../../include/partons/utils/type/PhysicalType.h"

namespace PARTONS {

DVCSConvolCoeffFunctionKinematicDaoService::DVCSConvolCoeffFunctionKinematicDaoService() :
        ConvolCoeffFunctionKinematicDaoService(
                "DVCSConvolCoeffFunctionKinematicDaoService") {
}

DVCSConvolCoeffFunctionKinematicDaoService::~DVCSConvolCoeffFunctionKinematicDaoService() {
}

int DVCSConvolCoeffFunctionKinematicDaoService::insert(
        const DVCSConvolCoeffFunctionKinematic& ccfKinematic) const {

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

int DVCSConvolCoeffFunctionKinematicDaoService::insert(
        const List<DVCSConvolCoeffFunctionKinematic>& ccfKinematicList) const {

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

int DVCSConvolCoeffFunctionKinematicDaoService::getIdByKinematicObject(
        const DVCSConvolCoeffFunctionKinematic& ccfKinematic) const {
    return m_dvcsConvolCoeffFunctionKinematicDao.select(ccfKinematic.getXi(),
            ccfKinematic.getT(), ccfKinematic.getQ2(), ccfKinematic.getMuF2(),
            ccfKinematic.getMuR2());
}

DVCSConvolCoeffFunctionKinematic DVCSConvolCoeffFunctionKinematicDaoService::getKinematicById(
        const int id) const {
    return m_dvcsConvolCoeffFunctionKinematicDao.getKinematicById(id);
}

int DVCSConvolCoeffFunctionKinematicDaoService::insertWithoutTransaction(
        const DVCSConvolCoeffFunctionKinematic& ccfKinematic) const {
    return m_dvcsConvolCoeffFunctionKinematicDao.insert(ccfKinematic.getXi(),
            ccfKinematic.getT(), ccfKinematic.getQ2(), ccfKinematic.getMuF2(),
            ccfKinematic.getMuR2());
}

List<DVCSConvolCoeffFunctionKinematic> DVCSConvolCoeffFunctionKinematicDaoService::getKinematicListByComputationId(
        int computationId) const {
    return m_dvcsConvolCoeffFunctionKinematicDao.getKinematicListByComputationId(
            computationId);
}

int DVCSConvolCoeffFunctionKinematicDaoService::getKinematicIdByHashSum(
        const std::string& hashSum) const {
    return m_dvcsConvolCoeffFunctionKinematicDao.getKinematicIdByHashSum(
            hashSum);
}

} /* namespace PARTONS */
