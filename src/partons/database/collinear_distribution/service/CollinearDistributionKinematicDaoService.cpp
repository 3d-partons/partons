#include "../../../../../include/partons/database/collinear_distribution/service/CollinearDistributionKinematicDaoService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtSql/qsqldatabase.h>

namespace PARTONS {

CollinearDistributionKinematicDaoService::CollinearDistributionKinematicDaoService() :
        BaseObject("CollinearDistributionKinematicDaoService") {
}

CollinearDistributionKinematicDaoService::~CollinearDistributionKinematicDaoService() {
}

int CollinearDistributionKinematicDaoService::insert(const CollinearDistributionKinematic &colldistKinematic) const {

    //result
    int colldistKinematicId = -1;

    //for multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {

        //insert
        colldistKinematicId = insertWithoutTransaction(colldistKinematic);

        //if there is no exception we can commit all query
        QSqlDatabase::database().commit();

    } catch (const std::exception &e) {

        //else return database in a stable state : n-1
        QSqlDatabase::database().rollback();

        throw ElemUtils::CustomException(getClassName(), __func__, e.what());
    }

    return colldistKinematicId;
}

int CollinearDistributionKinematicDaoService::insert(
        const List<CollinearDistributionKinematic>& colldistKinematicList) const {

    //result
    int colldistKinematicId = -1;

    //for multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {

        //insert
        for (unsigned int i = 0; i != colldistKinematicList.size(); i++) {
            colldistKinematicId = insertWithoutTransaction(colldistKinematicList.get(i));
        }

        //if there is no exception we can commit all query
        QSqlDatabase::database().commit();

    } catch (const std::exception &e) {

        //else return database in a stable state : n-1
        QSqlDatabase::database().rollback();

        throw ElemUtils::CustomException(getClassName(), __func__, e.what());
    }

    return colldistKinematicId;
}

int CollinearDistributionKinematicDaoService::getIdByKinematicObject(
        const CollinearDistributionKinematic &colldistKinematic) const {
    return m_CollinearDistributionKinematicDao.select(colldistKinematic.getX(), colldistKinematic.getMuF2(), colldistKinematic.getMuR2());
}

CollinearDistributionKinematic CollinearDistributionKinematicDaoService::getKinematicById(const int id) const {
    return m_CollinearDistributionKinematicDao.getKinematicById(id);
}

List<CollinearDistributionKinematic> CollinearDistributionKinematicDaoService::getKinematicListByComputationId(
        const int computationId) const {
    return m_CollinearDistributionKinematicDao.getKinematicListByComputationId(computationId);
}

int CollinearDistributionKinematicDaoService::insertWithoutTransaction(
        const CollinearDistributionKinematic& colldistKinematic) const {
    return m_CollinearDistributionKinematicDao.insert(colldistKinematic.getX(), colldistKinematic.getMuF2(), colldistKinematic.getMuR2(),
            colldistKinematic.getHashSum());
}

int CollinearDistributionKinematicDaoService::getKinematicIdByHashSum(
        const std::string& hashSum) const {
    return m_CollinearDistributionKinematicDao.getKinematicIdByHashSum(hashSum);
}

} /* namespace PARTONS */
