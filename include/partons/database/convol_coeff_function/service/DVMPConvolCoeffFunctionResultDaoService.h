#ifndef DVMP_CONVOL_COEFF_FUNCTION_RESULT_DAO_SERVICE_H
#define DVMP_CONVOL_COEFF_FUNCTION_RESULT_DAO_SERVICE_H

/**
 * @file DVMPConvolCoeffFunctionResultDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 05, 2015
 * @version 1.0
 */

#include "../../../beans/convol_coeff_function/DVMP/DVMPConvolCoeffFunctionResult.h"
#include "../../../beans/List.h"
#include "../dao/DVMPConvolCoeffFunctionResultDao.h"
#include "ConvolCoeffFunctionResultDaoService.h"
#include "DVMPConvolCoeffFunctionKinematicDaoService.h"

namespace PARTONS {

/**
 * @class DVMPConvolCoeffFunctionResultDaoService
 *
 * @brief DVMP Compton form factor (CFF) result Data Access Object (DAO) service.
 */
class DVMPConvolCoeffFunctionResultDaoService: public ConvolCoeffFunctionResultDaoService {

public:

    /**
     * Default constructor.
     */
    DVMPConvolCoeffFunctionResultDaoService();

    /**
     * Destructor.
     */
    virtual ~DVMPConvolCoeffFunctionResultDaoService();

    /**
     * Insert into database a new DVMPConvolCoeffFunctionResult object with transactions mechanisms.
     *
     * @param result Reference to DVMPConvolCoeffFunctionResult object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insert(const DVMPConvolCoeffFunctionResult &result);

    /**
     * Insert into database a list of DVMPConvolCoeffFunctionResult objects with transactions mechanisms.
     *
     * @param resultList Reference to List of DVMPConvolCoeffFunctionResult objects to be inserted.
     * @return Unique id related to the last entry inserted into the database.
     */
    int insert(const List<DVMPConvolCoeffFunctionResult> &resultList);

    /**
     * Try to find all entries in the database containing CFF results that are associated with given unique computation id value.
     *
     * @param computationId Computation id of database entries to be found.
     * @return List of DVMPConvolCoeffFunctionResult objects that matches with computation id.
     */
    List<DVMPConvolCoeffFunctionResult> getResultListByComputationId(
            const int computationId) const;

private:

    /**
     * DVMPConvolCoeffFunctionKinematicDaoService object to perform database queries.
     */
    DVMPConvolCoeffFunctionKinematicDaoService m_dvmpConvolCoeffFunctionKinematicDaoService;

    /**
     * ConvolCoeffFunctionResultDao object to perform database queries.
     */
    DVMPConvolCoeffFunctionResultDao m_dvmpConvolCoeffFunctionResultDao;
};

} /* namespace PARTONS */

#endif /* DVMP_CONVOL_COEFF_FUNCTION_RESULT_DAO_SERVICE_H */
