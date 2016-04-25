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
 * @brief Its role is to deal with DVCSConvolCoeffFunctionResult C++ object and related tables from the database.
 * You can insert, select or remove results from database by many kind of stuff.
 * Therefore database services ensure the integrity of the database by using transaction and rollback mechanisms if something wrong append : the database will stay always in a stable state.
 * And they improve querying speed by using transaction and commit mechanisms for a large amount of simultaneous queries.
 */

#include "../../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../../../beans/ResultList.h"
#include "../../common/service/ComputationDaoService.h"
#include "../dao/ConvolCoeffFunctionResultDao.h"
#include "ConvolCoeffFunctionKinematicDaoService.h"

class ConvolCoeffFunctionResultDaoService: public BaseObject {
public:
    /**
     * Default constructor
     */
    ConvolCoeffFunctionResultDaoService();

    /**
     * Default destructor
     */
    virtual ~ConvolCoeffFunctionResultDaoService();

    /**
     * Insert into database a new DVCSConvolCoeffFunctionResult object with transactions mechanisms.
     *
     * @param result
     * @return unique id related to the new entry inserted into the database
     */
    int insert(const DVCSConvolCoeffFunctionResult &result);

    /**
     * Insert into database a list of DVCSConvolCoeffFunctionResult objects with transactions mechanisms.
     *
     * @param resultList
     * @return unique id related to the last entry inserted into the database
     */
    int insert(const ResultList<DVCSConvolCoeffFunctionResult> &resultList);

    /**
     * Return a list of DVCSConvolCoeffFunctionResult objects from the database identified by a specific computation identifier.
     *
     * @param computationId
     * @return list of DVCSConvolCoeffFunctionResult objects.
     */
    ResultList<DVCSConvolCoeffFunctionResult> getResultListByComputationId(
            const int computationId) const;

private:
    ConvolCoeffFunctionResultDao m_convolCoeffFunctionResultDao; ///< reference to the right DAO object to perform database queries

    ConvolCoeffFunctionKinematicDaoService m_convolCoeffFunctionKinematicDaoService; ///< reference to be able to store kinematic object related to the result.
    ComputationDaoService m_computationDaoService; ///< reference to be able to generate computationId and store ComputationConfiguration object and EnvironmentConfiguration object related to the result.

    /**
     * Insert into the database a new DVCSConvolCoeffFunctionResult object without using transactions mechanisms.
     * Helpful when dealing with a ResultList<DVCSConvolCoeffFunctionResult> object, because transactions are already performed earlier.
     *
     * @param result
     * @return unique id related to the new entry inserted into the database
     */
    int insertWithoutTransaction(
            const DVCSConvolCoeffFunctionResult &result) const;
};

#endif /* CONVOL_COEFF_FUNCTION_RESULT_DAO_SERVICE_H */
