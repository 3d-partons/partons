#ifndef GPD_RESULT_DAO
#define GPD_RESULT_DAO

/**
 * @file GPDResultDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 13, 2015
 * @version 1.0
 */

#include <QtSql/qsqlquery.h>
#include <string>

#include "../../../beans/gpd/GPDResult.h"
#include "../../../beans/List.h"
#include "../../parton_distribution/dao/PartonDistributionDao.h"
#include "GPDKinematicDao.h"

namespace PARTONS {

/**
 * @class GPDResultDao
 *
 * @brief GPD result Data Access Object (DAO).
 *
 * This DAO is used to insert, select and delete GPD results from the database.
 */
class GPDResultDao: public BaseObject {
public:

    /**
     * Default constructor.
     */
    GPDResultDao();

    /**
     * Destructor.
     */
    virtual ~GPDResultDao();

    /**
     * Insert GPD result into database.
     * @param computationModuleName Name of module used to evaluate this result.
     * @param gpdKinematicId Unique id of row in the database containing GPD kinematics associated to this result.
     * @param computationId Unique id of row in the database containing computation information associated to this result.
     * @return Unique id of inserted row in the database.
     */
    int insertResult(const std::string &computationModuleName,
            int gpdKinematicId, int computationId) const;

    /**
     * Associate row in the database representing PartonDistribution to that representing GPDResult.
     * @param gpdTypeId GPD type, see GPDType::Type.
     * @param gpdResultId Unique id of row in the database representing GPDResult object.
     * @param partonDistributionId Unique id of row in the database representing PartonDistribution object.
     * @return Unique id of inserted row in the database.
     */
    int insertIntoGPDResultPartonDistributionTable(const int gpdTypeId,
            const int gpdResultId, const int partonDistributionId) const;

    /**
     * Retrieve list of GPD results from the database by given unique id of computation.
     * @param computationId Unique id of computation to be selected.
     * @return List of GPDResult objects containing GPD results retrieved from the database.
     */
    List<GPDResult> getGPDResultListByComputationId(
            const int computationId) const;

private:

    /**
     * Kinematic DAO.
     */
    GPDKinematicDao m_gpdKinematicDao;

    /**
     * PartonDistributionDao object to perform database queries.
     */
    PartonDistributionDao m_partonDistributionDao;

    /**
     * Fill List of GPDResult objects from given query.
     * @param gpdResultList List of GPDResult objects to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillGPDResultList(List<GPDResult> &gpdResultList,
            QSqlQuery &query) const;
};

} /* namespace PARTONS */

#endif /* GPD_RESULT_DAO */
