#ifndef CONVOL_COEFF_FUNCTION_RESULT_DAO_SERVICE_H
#define CONVOL_COEFF_FUNCTION_RESULT_DAO_SERVICE_H

/**
 * @file ConvolCoeffFunctionResultDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 05, 2015
 * @version 1.0
 *
 * @class ConvolCoeffFunctionResultDaoService
 *
 * @brief Compton form factor (CFF) result Data Access Object (DAO) service.
 *
 * It deals with ConvolCoeffFunctionResult C++ object and related tables from the database.
 *
 * With this service you can insert, select or remove CFF results from the database. It ensures the integrity of the database by using transaction and rollback mechanisms - if something wrong happened, the database will stay always in a stable state. In addition, it improves querying speed by using transaction and commit mechanisms for a large amount of simultaneous queries.
 *
 * Analyze the following code for the example of usage:
 \code{.cpp}
 //evaluate exemplary CFF result to be inserted in database

 //retrieve CFF service
 ConvolCoeffFunctionService* pDVCSConvolCoeffFunctionService = Partons::getInstance()->getServiceObjectRegistry()->getConvolCoeffFunctionService();

 //load GPD module with the BaseModuleFactory
 GPDModule* pGPDModel = Partons::getInstance()->getModuleObjectFactory()->newGPDModule(MMS13Model::classId);

 //load CFF module with the BaseModuleFactory
 DVCSConvolCoeffFunctionModule* pDVCSCFFModule = Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(DVCSCFFModel::classId);

 //configure CFF module
 ElemUtils::Parameters parameters(PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE, PerturbativeQCDOrderType::LO);
 pDVCSCFFModule->configure(parameters);

 pDVCSCFFModule->setGPDModule(pGPDModel);

 //define CFF kinematics used in computation
 DVCSConvolCoeffFunctionKinematic kinematic = DVCSConvolCoeffFunctionKinematic(0.05, -0.1, 4., 4., 4.);

 //evaluate
 DVCSConvolCoeffFunctionResult cffResultInserted = pDVCSConvolCoeffFunctionService->computeForOneCCFModel(kinematic, pDVCSCFFModule);

 //get ConvolCoeffFunctionResultDaoService
 ConvolCoeffFunctionResultDaoService convolCoeffFunctionResultDaoService;

 //insert result into database
 int id = convolCoeffFunctionResultDaoService.insert(cffResultInserted);

 //retrieve result from database
 DVCSConvolCoeffFunctionResult cffResultExtracted;
 convolCoeffFunctionResultDaoService.select(id, cffResultExtracted);

 //compare
 Partons::getInstance()->getLoggerManager()->info("example", __func__,
 ElemUtils::Formatter() << "Inserted: " << cffResultInserted.toString());
 Partons::getInstance()->getLoggerManager()->info("example", __func__,
 ElemUtils::Formatter() << "Extracted: " << cffResultExtracted.toString());
 \endcode
 */

#include <include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h>
#include <include/partons/beans/List.h>
#include <include/partons/database/common/service/ComputationDaoService.h>
#include <include/partons/database/common/service/ResultInfoDaoService.h>
#include <include/partons/database/convol_coeff_function/dao/ConvolCoeffFunctionResultDao.h>
#include <include/partons/database/convol_coeff_function/service/ConvolCoeffFunctionKinematicDaoService.h>
#include <include/partons/database/ResultDaoService.h>
#include <string>

class ConvolCoeffFunctionResultDaoService: public ResultDaoService {
public:

    /**
     * Default constructor.
     */
    ConvolCoeffFunctionResultDaoService();

    /**
     * Destructor.
     */
    virtual ~ConvolCoeffFunctionResultDaoService();

    /**
     * Insert into database a new DVCSConvolCoeffFunctionResult object with transactions mechanisms.
     *
     * @param result Reference to DVCSConvolCoeffFunctionResult object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insert(const DVCSConvolCoeffFunctionResult &result);

    /**
     * Insert into database a list of DVCSConvolCoeffFunctionResult objects with transactions mechanisms.
     *
     * @param resultList Reference to List of DVCSConvolCoeffFunctionResult objects to be inserted.
     * @return Unique id related to the last entry inserted into the database.
     */
    int insert(const List<DVCSConvolCoeffFunctionResult> &resultList);

    /**
     * Try to find all entries in the database containing CFF results that are associated with given unique computation id value.
     *
     * @param computationId Computation id of database entries to be found.
     * @return List of DVCSConvolCoeffFunctionResult objects that matches with computation id.
     */
    List<DVCSConvolCoeffFunctionResult> getResultListByComputationId(
            const int computationId) const;

private:

    int m_lastCCFKinematicId; ///< Unique id of row containing CFF kinematics to be inserted via temporary file mechanism.
    int m_lastCCFResultId; ///< Unique id of row containing CFF result to be inserted via temporary file mechanism.
    int m_lastCCFResultComplexId; ///< Unique id of row containing complex values (result for given GPD type) to be inserted via temporary file mechanism.

    std::string m_ccfKinematicTableFile; ///< CFF kinematics to be inserted via temporary file mechanism.
    std::string m_ccfResultTableFile; ///< CFF results to be inserted via temporary file mechanism.
    std::string m_ccfResultComplexTableFile; ///< Complex values (results for GPD types) to be inserted via temporary file mechanism.

    /**
     * ConvolCoeffFunctionResultDao object to perform database queries.
     */
    ConvolCoeffFunctionResultDao m_convolCoeffFunctionResultDao;

    /**
     * ConvolCoeffFunctionKinematicDaoService object to perform database queries.
     */
    ConvolCoeffFunctionKinematicDaoService m_convolCoeffFunctionKinematicDaoService;

    /**
     * ComputationDaoService object to perform database queries.
     */
    ComputationDaoService m_computationDaoService;

    /**
     * ResultInfoDaoService object to perform database queries.
     */
    ResultInfoDaoService m_resultInfoDaoService;
};

#endif /* CONVOL_COEFF_FUNCTION_RESULT_DAO_SERVICE_H */
