#ifndef GPD_RESULT_DAO_SERVICE
#define GPD_RESULT_DAO_SERVICE

/**
 * @file GPDResultDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 13, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/gpd/GPDResult.h"
#include "../../../beans/List.h"
#include "../../common/service/ResultInfoDaoService.h"
#include "../../parton_distribution/service/PartonDistributionDaoService.h"
#include "../../ResultDaoService.h"
#include "../dao/GPDResultDao.h"
#include "GPDKinematicDaoService.h"

namespace PARTONS {

/**
 * @class GPDResultDaoService
 *
 * @brief GPD result Data Access Object (DAO) service.
 *
 * It deals with GPDResult C++ object and related tables from the database.
 *
 * With this service you can insert, select or remove GPD results from the database. It ensures the integrity of the database by using transaction and rollback mechanisms - if something wrong happened, the database will stay always in a stable state. In addition, it improves querying speed by using transaction and commit mechanisms for a large amount of simultaneous queries.
 *
 * Analyze the following code for the example of usage:
 \code{.cpp}
 //evaluate exemplary GPD result to be inserted in database

 //retrieve GPD service
 GPDService* pGPDService = Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

 //load GPD module with the BaseModuleFactory
 GPDModule* pGPDModel = Partons::getInstance()->getModuleObjectFactory()->newGPDModule(MMS13Model::classId);

 //define GPD kinematics used in computation
 GPDKinematic gpdKinematic(-0.1, 0.05, 0., 2., 2.);

 //define list of GPD types to be computed
 List<GPDType> gpdTypeList;
 gpdTypeList.add(GPDType::H);

 //evaluate
 GPDResult gpdResultInserted = pGPDService->computeGPDModel(gpdKinematic, pGPDModel, gpdTypeList);

 //get GPDResultDaoService
 GPDResultDaoService gpdResultDaoService;

 //insert result into database
 int id = gpdResultDaoService.insert(gpdResultInserted);

 //retrieve result from database
 GPDResult gpdResultExtracted;
 gpdResultDaoService.select(id, gpdResultExtracted);

 //compare
 Partons::getInstance()->getLoggerManager()->info("example", __func__,
 ElemUtils::Formatter() << "Inserted: " << gpdResultInserted.toString());
 Partons::getInstance()->getLoggerManager()->info("example", __func__,
 ElemUtils::Formatter() << "Extracted: " << gpdResultExtracted.toString());
 \endcode
 */
class GPDResultDaoService: public ResultDaoService {

public:

    /**
     * Default constructor.
     */
    GPDResultDaoService();

    /**
     * Destructor.
     */
    virtual ~GPDResultDaoService();

    /**
     * Insert into database a new GPDKinematic object with transactions mechanisms.
     *
     * @param gpdResult Reference to GPDResult object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insert(const GPDResult &gpdResult);

    /**
     * Insert into database a list of GPDResult objects with transactions mechanisms.
     *
     * @param gpdResultList Reference to List of GPDResult objects to be inserted.
     * @return Unique id related to the last entry inserted into the database.
     */
    int insert(const List<GPDResult> &gpdResultList);

    /**
     * Try to find all entries in the database containing GPD results that are associated with given unique computation id value.
     *
     * @param computationId Computation id of database entries to be found.
     * @return List of GPDKinematic objects that matches with computation id.
     */
    List<GPDResult> getGPDResultListByComputationId(
            const int computationId) const;

private:

    int m_lastGPDKinematicId; ///< Unique id of row containing GPD kinematics to be inserted via temporary file mechanism.
    int m_lastGPDResultId; ///< Unique id of row containing GPD result to be inserted via temporary file mechanism.
    int m_lastPartonDistributionId; ///< Unique id of row containing parton distribution to be inserted via temporary file mechanism.
    int m_lastQuarkDistributionId; ///< Unique id of row containing quark distribution to be inserted via temporary file mechanism.
    int m_lastPartonDistributionQuarkDistributionId; ///< Unique id of row containing association between parton distribution and quark distribution to be inserted via temporary file mechanism.
    int m_lastGPDResultPartonDistributionId; ///< Unique id of row containing association between gpd result and parton distribution to be inserted via temporary file mechanism.

    std::string m_gpdKinematicDatabaseFile; ///< GPD kinematics to be inserted via temporary file mechanism.
    std::string m_gpdResultDatabaseFile; ///< GPD results to be inserted via temporary file mechanism.
    std::string m_quark_distribution_table; ///< Quark distributions to be inserted via temporary file mechanism.
    std::string m_parton_distribution_table; ///< Parton distributions to be inserted via temporary file mechanism.
    std::string m_gpd_result_parton_distribution_table; ///< Associations between parton distributions and quark distributions to be inserted via temporary file mechanism.
    std::string m_parton_distribution_quark_distribution_table; ///< Association between gpd results and parton distributions to be inserted via temporary file mechanism.

    /**
     * GPDResultDao object to perform database queries.
     */
    GPDResultDao m_gpdResultDao;

    /**
     * GPDKinematicDaoService object to perform database queries.
     */
    GPDKinematicDaoService m_gpdKinematicDaoService;

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

#endif /* GPD_RESULT_DAO_SERVICE */
