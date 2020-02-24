#include "../../../../../include/partons/database/gpd/service/GPDKinematicDaoService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtSql/qsqldatabase.h>

namespace PARTONS {

GPDKinematicDaoService::GPDKinematicDaoService() :
        BaseObject("GPDKinematicDaoService") {
}

GPDKinematicDaoService::~GPDKinematicDaoService() {
}

int GPDKinematicDaoService::insert(const GPDKinematic &gpdKinematic) const {

    //result
    int gpdKinematicId = -1;

    //for multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {

        //insert
        gpdKinematicId = insertWithoutTransaction(gpdKinematic);

        //if there is no exception we can commit all query
        QSqlDatabase::database().commit();

    } catch (const std::exception &e) {

        //else return database in a stable state : n-1
        QSqlDatabase::database().rollback();

        throw ElemUtils::CustomException(getClassName(), __func__, e.what());
    }

    return gpdKinematicId;
}

int GPDKinematicDaoService::insert(
        const List<GPDKinematic>& gpdKinematicList) const {

    //result
    int gpdKinematicId = -1;

    //for multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {

        //insert
        for (unsigned int i = 0; i != gpdKinematicList.size(); i++) {
            gpdKinematicId = insertWithoutTransaction(gpdKinematicList.get(i));
        }

        //if there is no exception we can commit all query
        QSqlDatabase::database().commit();

    } catch (const std::exception &e) {

        //else return database in a stable state : n-1
        QSqlDatabase::database().rollback();

        throw ElemUtils::CustomException(getClassName(), __func__, e.what());
    }

    return gpdKinematicId;
}

int GPDKinematicDaoService::getIdByKinematicObject(
        const GPDKinematic &gpdKinematic) const {
    return m_GPDKinematicDao.select(gpdKinematic.getX(), gpdKinematic.getXi(),
            gpdKinematic.getT(), gpdKinematic.getMuF2(), gpdKinematic.getMuR2());
}

GPDKinematic GPDKinematicDaoService::getKinematicById(const int id) const {
    return m_GPDKinematicDao.getKinematicById(id);
}

List<GPDKinematic> GPDKinematicDaoService::getKinematicListByComputationId(
        const int computationId) const {
    return m_GPDKinematicDao.getKinematicListByComputationId(computationId);
}

int GPDKinematicDaoService::insertWithoutTransaction(
        const GPDKinematic& gpdKinematic) const {
    return m_GPDKinematicDao.insert(gpdKinematic.getX(), gpdKinematic.getXi(),
            gpdKinematic.getT(), gpdKinematic.getMuF2(), gpdKinematic.getMuR2(),
            gpdKinematic.getHashSum());
}

int GPDKinematicDaoService::getKinematicIdByHashSum(
        const std::string& hashSum) const {
    return m_GPDKinematicDao.getKinematicIdByHashSum(hashSum);
}

} /* namespace PARTONS */
