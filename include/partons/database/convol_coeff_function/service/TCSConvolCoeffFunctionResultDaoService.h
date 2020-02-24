#ifndef TCS_CONVOL_COEFF_FUNCTION_RESULT_DAO_SERVICE_H
#define TCS_CONVOL_COEFF_FUNCTION_RESULT_DAO_SERVICE_H

/**
 * @file TCSConvolCoeffFunctionResultDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 05, 2015
 * @version 1.0
 */

#include "../../../beans/convol_coeff_function/TCS/TCSConvolCoeffFunctionResult.h"
#include "../../../beans/List.h"
#include "../dao/TCSConvolCoeffFunctionResultDao.h"
#include "ConvolCoeffFunctionResultDaoService.h"
#include "TCSConvolCoeffFunctionKinematicDaoService.h"

namespace PARTONS {

/**
 * @class TCSConvolCoeffFunctionResultDaoService
 *
 * @brief TCS Compton form factor (CFF) result Data Access Object (DAO) service.
 */
class TCSConvolCoeffFunctionResultDaoService: public ConvolCoeffFunctionResultDaoService {

public:

    /**
     * Default constructor.
     */
    TCSConvolCoeffFunctionResultDaoService();

    /**
     * Destructor.
     */
    virtual ~TCSConvolCoeffFunctionResultDaoService();

    /**
     * Insert into database a new TCSConvolCoeffFunctionResult object with transactions mechanisms.
     *
     * @param result Reference to TCSConvolCoeffFunctionResult object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insert(const TCSConvolCoeffFunctionResult &result);

    /**
     * Insert into database a list of TCSConvolCoeffFunctionResult objects with transactions mechanisms.
     *
     * @param resultList Reference to List of TCSConvolCoeffFunctionResult objects to be inserted.
     * @return Unique id related to the last entry inserted into the database.
     */
    int insert(const List<TCSConvolCoeffFunctionResult> &resultList);

    /**
     * Try to find all entries in the database containing CFF results that are associated with given unique computation id value.
     *
     * @param computationId Computation id of database entries to be found.
     * @return List of TCSConvolCoeffFunctionResult objects that matches with computation id.
     */
    List<TCSConvolCoeffFunctionResult> getResultListByComputationId(
            const int computationId) const;

private:

    /**
     * TCSConvolCoeffFunctionKinematicDaoService object to perform database queries.
     */
    TCSConvolCoeffFunctionKinematicDaoService m_tcsConvolCoeffFunctionKinematicDaoService;

    /**
     * ConvolCoeffFunctionResultDao object to perform database queries.
     */
    TCSConvolCoeffFunctionResultDao m_tcsConvolCoeffFunctionResultDao;
};

} /* namespace PARTONS */

#endif /* TCS_CONVOL_COEFF_FUNCTION_RESULT_DAO_SERVICE_H */
