#ifndef DDVCS_CONVOL_COEFF_FUNCTION_RESULT_DAO_SERVICE_H
#define DDVCS_CONVOL_COEFF_FUNCTION_RESULT_DAO_SERVICE_H

/**
 * @file DDVCSConvolCoeffFunctionResultDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 05, 2015
 * @version 1.0
 */

#include "../../../beans/convol_coeff_function/DDVCS/DDVCSConvolCoeffFunctionResult.h"
#include "../../../beans/List.h"
#include "../dao/DDVCSConvolCoeffFunctionResultDao.h"
#include "ConvolCoeffFunctionResultDaoService.h"
#include "DDVCSConvolCoeffFunctionKinematicDaoService.h"

namespace PARTONS {

/**
 * @class DDVCSConvolCoeffFunctionResultDaoService
 *
 * @brief DDVCS Compton form factor (CFF) result Data Access Object (DAO) service.
 */
class DDVCSConvolCoeffFunctionResultDaoService: public ConvolCoeffFunctionResultDaoService {

public:

    /**
     * Default constructor.
     */
    DDVCSConvolCoeffFunctionResultDaoService();

    /**
     * Destructor.
     */
    virtual ~DDVCSConvolCoeffFunctionResultDaoService();

    /**
     * Insert into database a new DDVCSConvolCoeffFunctionResult object with transactions mechanisms.
     *
     * @param result Reference to DDVCSConvolCoeffFunctionResult object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insert(const DDVCSConvolCoeffFunctionResult &result);

    /**
     * Insert into database a list of DDVCSConvolCoeffFunctionResult objects with transactions mechanisms.
     *
     * @param resultList Reference to List of DDVCSConvolCoeffFunctionResult objects to be inserted.
     * @return Unique id related to the last entry inserted into the database.
     */
    int insert(const List<DDVCSConvolCoeffFunctionResult> &resultList);

    /**
     * Try to find all entries in the database containing CFF results that are associated with given unique computation id value.
     *
     * @param computationId Computation id of database entries to be found.
     * @return List of DDVCSConvolCoeffFunctionResult objects that matches with computation id.
     */
    List<DDVCSConvolCoeffFunctionResult> getResultListByComputationId(
            const int computationId) const;

private:

    /**
     * DDVCSConvolCoeffFunctionKinematicDaoService object to perform database queries.
     */
    DDVCSConvolCoeffFunctionKinematicDaoService m_ddvcsConvolCoeffFunctionKinematicDaoService;

    /**
     * ConvolCoeffFunctionResultDao object to perform database queries.
     */
    DDVCSConvolCoeffFunctionResultDao m_ddvcsConvolCoeffFunctionResultDao;
};

} /* namespace PARTONS */

#endif /* DDVCS_CONVOL_COEFF_FUNCTION_RESULT_DAO_SERVICE_H */
