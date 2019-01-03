#include "../../../../../include/partons/database/gpd/service/GPDKinematicDaoService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtSql/qsqldatabase.h>

#include "../../../../../include/partons/utils/type/PhysicalType.h"

namespace PARTONS {

GPDKinematicDaoService::GPDKinematicDaoService() :
        BaseObject("GPDKinematicDaoService") {

}
GPDKinematicDaoService::~GPDKinematicDaoService() {
    // Nothing to do
}

int GPDKinematicDaoService::insert(const GPDKinematic &gpdKinematic) const {
    int gpdKinematicId = -1;

    // For multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {
        gpdKinematicId = insertWithoutTransaction(gpdKinematic);

        // If there is no exception we can commit all query
        QSqlDatabase::database().commit();

    } catch (const std::exception &e) {
        // Else return database in a stable state : n-1
        QSqlDatabase::database().rollback();

        throw ElemUtils::CustomException(getClassName(), __func__, e.what());
    }

    return gpdKinematicId;
}

int GPDKinematicDaoService::insert(
        const List<GPDKinematic>& gpdKinematicList) const {
    int gpdKinematicId = -1;

    // For multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {

        for (unsigned int i = 0; i != gpdKinematicList.size(); i++) {
            gpdKinematicId = insertWithoutTransaction(gpdKinematicList.get(i));
        }

        // If there is no exception we can commit all query
        QSqlDatabase::database().commit();

    } catch (const std::exception &e) {
        // Else return database in a stable state : n-1
        QSqlDatabase::database().rollback();

        throw ElemUtils::CustomException(getClassName(), __func__, e.what());
    }

    return gpdKinematicId;
}

int GPDKinematicDaoService::getIdByKinematicObject(
        const GPDKinematic &gpdKinematic) const {
    return m_GPDKinematicDao.select(gpdKinematic.getX().getValue(),
            gpdKinematic.getXi().getValue(), gpdKinematic.getT().getValue(),
            gpdKinematic.getMuF2().getValue(),
            gpdKinematic.getMuR2().getValue());
}

GPDKinematic GPDKinematicDaoService::getKinematicById(const int id) const {
    return m_GPDKinematicDao.getKinematicById(id);
}

List<GPDKinematic> GPDKinematicDaoService::getKinematicListByComputationId(
        const int computationId) const {
    debug(__func__, "Processing ...");
    info(__func__,
            ElemUtils::Formatter()
                    << "Searching in database for GPD kinematic(s) with computation id = "
                    << computationId << " ...");
    return m_GPDKinematicDao.getKinematicListByComputationId(computationId);
}

int GPDKinematicDaoService::insertWithoutTransaction(
        const GPDKinematic& gpdKinematic) const {
    return m_GPDKinematicDao.insert(gpdKinematic.getX().getValue(),
            gpdKinematic.getXi().getValue(), gpdKinematic.getT().getValue(),
            gpdKinematic.getMuF2().getValue(),
            gpdKinematic.getMuR2().getValue());
}

int GPDKinematicDaoService::getKinematicIdByHashSum(
        const std::string& hashSum) const {
    return m_GPDKinematicDao.getKinematicIdByHashSum(hashSum);
}

} /* namespace PARTONS */
