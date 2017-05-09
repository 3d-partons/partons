#ifndef OBSERVABLE_RESULT_DAO_SERVICE_H
#define OBSERVABLE_RESULT_DAO_SERVICE_H

/**
 * @file ObservableResultDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 02, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/List.h"
#include "../../../beans/observable/ObservableResult.h"
#include "../../common/service/ComputationDaoService.h"
#include "../../common/service/ResultInfoDaoService.h"
#include "../../ResultDaoService.h"
#include "../dao/ObservableResultDao.h"
#include "ObservableKinematicDaoService.h"

/**
 * @class ObservableResultDaoService
 *
 * @brief Observable result Data Access Object (DAO) service.
 *
 * It deals with ObservableResult C++ object and related tables from the database.
 *
 * With this service you can insert, select or remove observable results from the database. It ensures the integrity of the database by using transaction and rollback mechanisms - if something wrong happened, the database will stay always in a stable state. In addition, it improves querying speed by using transaction and commit mechanisms for a large amount of simultaneous queries.
 *
 * Analyze the following code for the example of usage:
 \code{.cpp}
 //evaluate exemplary observable result to be inserted in database

 //retrieve observable service
 ObservableService* pObservableService = Partons::getInstance()->getServiceObjectRegistry()->getObservableService();

 //load GPD module with the BaseModuleFactory
 GPDModule* pGPDModel = Partons::getInstance()->getModuleObjectFactory()->newGPDModule(MMS13Model::classId);

 //load CFF module with the BaseModuleFactory
 DVCSConvolCoeffFunctionModule* pDVCSCFFModule = Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(DVCSCFFModel::classId);

 //configure CFF module
 ElemUtils::Parameters parameters(PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE, PerturbativeQCDOrderType::LO);
 pDVCSCFFModule->configure(parameters);

 pDVCSCFFModule->setGPDModule(pGPDModel);

 //load process module with BaseModuleFactory
 DVCSModule* pDVCSModule = Partons::getInstance()->getModuleObjectFactory()->newProcessModule(GV2008Model::classId);

 //configure process module
 pDVCSModule->setConvolCoeffFunctionModule(pDVCSCFFModel);

 //load observable module with BaseModuleFactory
 Observable* pObservable = Partons::getInstance()->getModuleObjectFactory()->newObservable(CrossSectionObservable::classId);

 //onfigure observable module
 pObservable->setProcessModule(pDVCSModule);

 //define observable kinematics used in computation
 ObservableKinematic observableKinematic(0.1, -0.1, 2., 6., 10.);

 //evaluate
 ObservableResult observableResultInserted = pObservableService->computeObservable(observableKinematic, pObservable);

 //get ObservableResultDaoService
 ObservableResultDaoService observableResultDaoService;

 //insert result into database
 int id = observableResultDaoService.insert(observableResultInserted);

 //retrieve result from database
 ObservableResult observableResultExtracted;
 observableResultDaoService.select(id, observableResultExtracted);

 //compare
 Partons::getInstance()->getLoggerManager()->info("example", __func__,
 ElemUtils::Formatter() << "Inserted: " << observableResultInserted.toString());
 Partons::getInstance()->getLoggerManager()->info("example", __func__,
 ElemUtils::Formatter() << "Extracted: " << observableResultExtracted.toString());
 \endcode
 */
class ObservableResultDaoService: public ResultDaoService {
public:

    /**
     * Default constructor.
     */
    ObservableResultDaoService();

    /**
     * Destructor.
     */
    virtual ~ObservableResultDaoService();

    /**
     * Insert into database a new ObservableResult object with transactions mechanisms.
     *
     * @param observableResult Reference to ObservableResult object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insert(const ObservableResult &observableResult);

    /**
     * Insert into database a list of ObservableResult objects with transactions mechanisms.
     *
     * @param observableResultList Reference to List of ObservableResult objects to be inserted.
     * @return Unique id related to the last entry inserted into the database.
     */
    int insert(const List<ObservableResult> &resultList);

    /**
     * Try to find all entries in the database containing observable results that are associated with given unique computation id value.
     *
     * @param computationId  Computation id of database entries to be found.
     * @return List of ObservableResult objects that matches with computation id.
     */
    List<ObservableResult> getObservableResultListByComputationId(
            const int computationId) const;

    /**
     * Retrieve list of observable results from the database by user-defined SQL query.
     * @param sqlQuery User-defined SQL query.
     * @return  List of ObservableResult objects containing observable results retrieved from the database.
     */
    List<ObservableResult> getObservableResultListFromSQLQuery(
            const std::string &sqlQuery) const;

private:

    /**
     * ObservableResultDao object to perform database queries.
     */
    ObservableResultDao m_observableResultDao;

    /**
     * ObservableKinematicDaoService object to perform database queries.
     */
    ObservableKinematicDaoService m_observableKinematicDaoService;

    /**
     * ComputationDaoService object to perform database queries.
     */
    ComputationDaoService m_computationDaoService;

    /**
     * ResultInfoDaoService object to perform database queries.
     */
    ResultInfoDaoService m_resultInfoDaoService;

    int m_lastObservableKinematicId; ///< Unique id of row containing observable kinematics to be inserted via temporary file mechanism.
    int m_lastObservableResultId; ///< Unique id of row containing observable result to be inserted via temporary file mechanism.

    std::string m_observableKinematicTableFile; ///< Observable kinematics to be inserted via temporary file mechanism.
    std::string m_observableResultTableFile; ///< Observable results to be inserted via temporary file mechanism.
};

#endif /* OBSERVABLE_RESULT_DAO_SERVICE_H */
