#ifndef OBSERVABLE_RESULT_DAO_SERVICE_H
#define OBSERVABLE_RESULT_DAO_SERVICE_H

/**
 * @file ObservableResultDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 02, 2015
 * @version 1.0
 */

#include <string>

#include "../../common/service/ComputationDaoService.h"
#include "../../common/service/ResultInfoDaoService.h"
#include "../../ResultDaoService.h"

namespace PARTONS {

/**
 * @class ObservableResultDaoService
 *
 * @brief Observable result Data Access Object (DAO) service.
 */
class ObservableResultDaoService: public ResultDaoService {

public:

    /**
     * Default constructor.
     */
    ObservableResultDaoService(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~ObservableResultDaoService();

protected:

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

} /* namespace PARTONS */

#endif /* OBSERVABLE_RESULT_DAO_SERVICE_H */
