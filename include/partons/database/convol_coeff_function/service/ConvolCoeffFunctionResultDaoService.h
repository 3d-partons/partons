#ifndef CONVOL_COEFF_FUNCTION_RESULT_DAO_SERVICE_H
#define CONVOL_COEFF_FUNCTION_RESULT_DAO_SERVICE_H

/**
 * @file ConvolCoeffFunctionResultDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 05, 2015
 * @version 1.0
 */

#include <string>

#include "../../common/service/ComputationDaoService.h"
#include "../../common/service/ResultInfoDaoService.h"
#include "../../ResultDaoService.h"

namespace PARTONS {

/**
 * @class ConvolCoeffFunctionResultDaoService
 *
 * @brief Compton form factor (CFF) result Data Access Object (DAO) service.
 */
class ConvolCoeffFunctionResultDaoService: public ResultDaoService {

public:

    /**
     * Default constructor.
     */
    ConvolCoeffFunctionResultDaoService(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~ConvolCoeffFunctionResultDaoService();

protected:

    int m_lastCCFKinematicProcessId; ///< Unique id of row containing CFF kinematics to be inserted via temporary file mechanism.
    int m_lastCCFResultId; ///< Unique id of row containing CFF result to be inserted via temporary file mechanism.
    int m_lastCCFResultProcessId; ///< Unique id of row containing complex values (result for given GPD type) to be inserted via temporary file mechanism.

    std::string m_ccfKinematicTableFile; ///< CFF kinematics to be inserted via temporary file mechanism.
    std::string m_ccfResultTableFile; ///< CFF results to be inserted via temporary file mechanism.
    std::string m_ccfResultComplexTableFile; ///< Complex values (results for GPD types) to be inserted via temporary file mechanism.

    /**
     * ComputationDaoService object to perform database queries.
     */
    ComputationDaoService m_computationDaoService;

    /**
     * ResultInfoDaoService object to perform database queries.
     */
    ResultInfoDaoService m_resultInfoDaoService;
};

} /* namespace PARTONS */

#endif /* CONVOL_COEFF_FUNCTION_RESULT_DAO_SERVICE_H */
