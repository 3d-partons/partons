#ifndef DVCS_CONVOL_COEFF_FUNCTION_RESULT_DAO_SERVICE_H
#define DVCS_CONVOL_COEFF_FUNCTION_RESULT_DAO_SERVICE_H

/**
 * @file DVCSConvolCoeffFunctionResultDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 05, 2015
 * @version 1.0
 */

#include "../../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../../../beans/List.h"
#include "../dao/DVCSConvolCoeffFunctionResultDao.h"
#include "ConvolCoeffFunctionResultDaoService.h"
#include "DVCSConvolCoeffFunctionKinematicDaoService.h"

namespace PARTONS {

/**
 * @class DVCSConvolCoeffFunctionResultDaoService
 *
 * @brief DVCS Compton form factor (CFF) result Data Access Object (DAO) service.
 */
class DVCSConvolCoeffFunctionResultDaoService: public ConvolCoeffFunctionResultDaoService {

public:

    /**
     * Default constructor.
     */
    DVCSConvolCoeffFunctionResultDaoService();

    /**
     * Destructor.
     */
    virtual ~DVCSConvolCoeffFunctionResultDaoService();

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

    /**
     * DVCSConvolCoeffFunctionKinematicDaoService object to perform database queries.
     */
    DVCSConvolCoeffFunctionKinematicDaoService m_dvcsConvolCoeffFunctionKinematicDaoService;

    /**
     * ConvolCoeffFunctionResultDao object to perform database queries.
     */
    DVCSConvolCoeffFunctionResultDao m_convolCoeffFunctionResultDao;
};

} /* namespace PARTONS */

#endif /* DVCS_CONVOL_COEFF_FUNCTION_RESULT_DAO_SERVICE_H */
