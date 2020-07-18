#ifndef COLLINEAR_DISTRIBUTION_RESULT_DAO_SERVICE
#define COLLINEAR_DISTRIBUTION_RESULT_DAO_SERVICE

/**
 * @file CollinearDistributionResultDaoService.h
 * @author: Valerio BERTONE (CEA Saclay)
 * @date July 18, 2020
 * @version 1.0
 */

#include <string>

#include "../../../beans/parton_distribution/CollinearDistributionResult.h"
#include "../../../beans/List.h"
#include "../../common/service/ResultInfoDaoService.h"
#include "../../parton_distribution/service/PartonDistributionDaoService.h"
#include "../../ResultDaoService.h"
#include "../dao/CollinearDistributionResultDao.h"
#include "CollinearDistributionKinematicDaoService.h"

namespace PARTONS {

/**
 * @class CollinearDistributionResultDaoService
 *
 * @brief collinear distribution result Data Access Object (DAO) service.
 *
 * It deals with CollinearDistributionResult C++ object and related tables from the database.
 */
class CollinearDistributionResultDaoService: public ResultDaoService {

public:

    /**
     * Default constructor.
     */
    CollinearDistributionResultDaoService();

    /**
     * Destructor.
     */
    virtual ~CollinearDistributionResultDaoService();

    /**
     * Insert into database a new CollinearDistributionKinematic object with transactions mechanisms.
     *
     * @param colldistResult Reference to CollinearDistributionResult object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insert(const CollinearDistributionResult &colldistResult);

    /**
     * Insert into database a list of CollinearDistributionResult objects with transactions mechanisms.
     *
     * @param colldistResultList Reference to List of CollinearDistributionResult objects to be inserted.
     * @return Unique id related to the last entry inserted into the database.
     */
    int insert(const List<CollinearDistributionResult> &colldistResultList);

    /**
     * Try to find all entries in the database containing collinear distribution results that are associated with given unique computation id value.
     *
     * @param computationId Computation id of database entries to be found.
     * @return List of CollinearDistributionKinematic objects that matches with computation id.
     */
    List<CollinearDistributionResult> getCollinearDistributionResultListByComputationId(
            const int computationId) const;

private:

    int m_lastCollinearDistributionKinematicId; ///< Unique id of row containing collinear distribution kinematics to be inserted via temporary file mechanism.
    int m_lastCollinearDistributionResultId; ///< Unique id of row containing collinear distribution result to be inserted via temporary file mechanism.
    int m_lastPartonDistributionId; ///< Unique id of row containing parton distribution to be inserted via temporary file mechanism.
    int m_lastQuarkDistributionId; ///< Unique id of row containing quark distribution to be inserted via temporary file mechanism.
    int m_lastPartonDistributionQuarkDistributionId; ///< Unique id of row containing association between parton distribution and quark distribution to be inserted via temporary file mechanism.
    int m_lastCollinearDistributionResultPartonDistributionId; ///< Unique id of row containing association between collinear distribution result and parton distribution to be inserted via temporary file mechanism.

    std::string m_colldistKinematicDatabaseFile; ///< collinear distribution kinematics to be inserted via temporary file mechanism.
    std::string m_colldistResultDatabaseFile; ///< collinear distribution results to be inserted via temporary file mechanism.
    std::string m_quark_distribution_table; ///< Quark distributions to be inserted via temporary file mechanism.
    std::string m_parton_distribution_table; ///< Parton distributions to be inserted via temporary file mechanism.
    std::string m_collinear_distribution_result_parton_distribution_table; ///< Associations between parton distributions and quark distributions to be inserted via temporary file mechanism.
    std::string m_parton_distribution_quark_distribution_table; ///< Association between collinear distribution results and parton distributions to be inserted via temporary file mechanism.

    /**
     * CollinearDistributionResultDao object to perform database queries.
     */
    CollinearDistributionResultDao m_colldistResultDao;

    /**
     * CollinearDistributionKinematicDaoService object to perform database queries.
     */
    CollinearDistributionKinematicDaoService m_colldistKinematicDaoService;

    /**
     * PartonDistributionDaoService object to perform database queries.
     */
    PartonDistributionDaoService m_partonDistributionDaoService;

    /**
     * ResultInfoDaoService object to perform database queries.
     */
    ResultInfoDaoService m_resultInfoDaoService;
};

} /* namespace PARTONS */

#endif /* COLLINEAR_DISTRIBUTION_RESULT_DAO_SERVICE */
