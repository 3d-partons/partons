#ifndef COLLINEAR_DISTRIBUTION_RESULT_DAO
#define COLLINEAR_DISTRIBUTION_RESULT_DAO

/**
 * @file CollinearDistributionResultDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 13, 2015
 * @version 1.0
 */

#include <QtSql/qsqlquery.h>
#include <string>

#include "../../../beans/parton_distribution/CollinearDistributionResult.h"
#include "../../../beans/List.h"
#include "../../parton_distribution/dao/PartonDistributionDao.h"
#include "CollinearDistributionKinematicDao.h"

namespace PARTONS {

/**
 * @class CollinearDistributionResultDao
 *
 * @brief collinear distribution result Data Access Object (DAO).
 *
 * This DAO is used to insert, select and delete collinear distribution results from the database.
 */
class CollinearDistributionResultDao: public BaseObject {
public:

    /**
     * Default constructor.
     */
    CollinearDistributionResultDao();

    /**
     * Destructor.
     */
    virtual ~CollinearDistributionResultDao();

    /**
     * Insert collinear distribution result into database.
     * @param computationModuleName Name of module used to evaluate this result.
     * @param colldistKinematicId Unique id of row in the database containing collinear distribution kinematics associated to this result.
     * @param computationId Unique id of row in the database containing computation information associated to this result.
     * @return Unique id of inserted row in the database.
     */
    int insertCollinearDistributionResult(const std::string &computationModuleName,
            int colldistKinematicId, int computationId) const;

    /**
     * Associate row in the database representing PartonDistribution to that representing CollinearDistributionResult.
     * @param colldistTypeId collinear distribution type, see CollinearDistributionType::Type.
     * @param colldistResultId Unique id of row in the database representing CollinearDistributionResult object.
     * @param partonDistributionId Unique id of row in the database representing PartonDistribution object.
     * @return Unique id of inserted row in the database.
     */
    int insertIntoCollinearDistributionResultPartonDistributionTable(const int colldistTypeId,
            const int colldistResultId, const int partonDistributionId) const;

    /**
     * Retrieve list of collinear distribution results from the database by given unique id of computation.
     * @param computationId Unique id of computation to be selected.
     * @return List of CollinearDistributionResult objects containing collinear distribution results retrieved from the database.
     */
    List<CollinearDistributionResult> getCollinearDistributionResultListByComputationId(
            const int computationId) const;

private:

    /**
     * Kinematic DAO.
     */
    CollinearDistributionKinematicDao m_colldistKinematicDao;

    /**
     * PartonDistributionDao object to perform database queries.
     */
    PartonDistributionDao m_partonDistributionDao;

    /**
     * Fill List of CollinearDistributionResult objects from given query.
     * @param colldistResultList List of CollinearDistributionResult objects to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillCollinearDistributionResultList(List<CollinearDistributionResult> &colldistResultList,
            QSqlQuery &query) const;
};

} /* namespace PARTONS */

#endif /* COLLINEAR_DISTRIBUTION_RESULT_DAO */
