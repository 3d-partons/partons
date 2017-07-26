#include "../../../../../include/partons/database/convol_coeff_function/service/ConvolCoeffFunctionKinematicDaoService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <QtSql/qsqldatabase.h>

namespace PARTONS {


ConvolCoeffFunctionKinematicDaoService::ConvolCoeffFunctionKinematicDaoService() :
        BaseObject("ConvolCoeffFunctionKinematicDaoService") {
}

ConvolCoeffFunctionKinematicDaoService::~ConvolCoeffFunctionKinematicDaoService() {
}

int ConvolCoeffFunctionKinematicDaoService::insert(
        const DVCSConvolCoeffFunctionKinematic& ccfKinematic) const {
    int kinematicId = -1;

    // For multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {
        kinematicId = insertWithoutTransaction(ccfKinematic);

        // If there is no exception we can commit all query
        QSqlDatabase::database().commit();

    } catch (const std::exception &e) {
        // Else return database in a stable state : n-1
        QSqlDatabase::database().rollback();

        throw ElemUtils::CustomException(getClassName(), __func__, e.what());
    }

    return kinematicId;
}

int ConvolCoeffFunctionKinematicDaoService::insert(
        const List<DVCSConvolCoeffFunctionKinematic>& ccfKinematicList) const {
    int kinematicId = -1;

    // For multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {

        for (unsigned int i = 0; i != ccfKinematicList.size(); i++) {
            kinematicId = insertWithoutTransaction(ccfKinematicList.get(i));
        }

        // If there is no exception we can commit all query
        QSqlDatabase::database().commit();

    } catch (const std::exception &e) {
        // Else return database in a stable state : n-1
        QSqlDatabase::database().rollback();

        throw ElemUtils::CustomException(getClassName(), __func__, e.what());
    }

    return kinematicId;
}

int ConvolCoeffFunctionKinematicDaoService::getIdByKinematicObject(
        const DVCSConvolCoeffFunctionKinematic& ccfKinematic) const {
    return m_convolCoeffFunctionKinematicDao.select(ccfKinematic.getXi(),
            ccfKinematic.getT(), ccfKinematic.getQ2(), ccfKinematic.getMuF2(),
            ccfKinematic.getMuR2());
}

DVCSConvolCoeffFunctionKinematic ConvolCoeffFunctionKinematicDaoService::getKinematicById(
        const int id) const {
    return m_convolCoeffFunctionKinematicDao.getKinematicById(id);
}

int ConvolCoeffFunctionKinematicDaoService::insertWithoutTransaction(
        const DVCSConvolCoeffFunctionKinematic& ccfKinematic) const {
    return m_convolCoeffFunctionKinematicDao.insert(ccfKinematic.getXi(),
            ccfKinematic.getT(), ccfKinematic.getQ2(), ccfKinematic.getMuF2(),
            ccfKinematic.getMuR2());
}

List<DVCSConvolCoeffFunctionKinematic> ConvolCoeffFunctionKinematicDaoService::getKinematicListByComputationId(
        int computationId) const {
    return m_convolCoeffFunctionKinematicDao.getKinematicListByComputationId(
            computationId);
}

int ConvolCoeffFunctionKinematicDaoService::getKinematicIdByHashSum(
        const std::string& hashSum) const {
    return m_convolCoeffFunctionKinematicDao.getKinematicIdByHashSum(hashSum);
}

} /* namespace PARTONS */
