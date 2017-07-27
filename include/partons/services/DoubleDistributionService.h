#ifndef DOUBLE_DISTRIBUTION_SERVICE_H
#define DOUBLE_DISTRIBUTION_SERVICE_H

/**
 * @file DoubleDistributionService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 30 July 2015
 * @version 1.0
 */

#include <string>

#include "../ServiceObject.h"

namespace PARTONS {

class DoubleDistributionKinematic;
class DoubleDistributionModule;
class DoubleDistributionResult;

/**
 * @class DoubleDistributionService
 *
 * @brief
 */
class DoubleDistributionService: public ServiceObject {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    DoubleDistributionService(const std::string &className);

    /**
     * Default destructor
     */
    virtual ~DoubleDistributionService();

    virtual void computeTask(Task &task);

    DoubleDistributionResult compute(
            const DoubleDistributionKinematic &kinematic,
            DoubleDistributionModule* pDoubleDistributionModule);
};

} /* namespace PARTONS */

#endif /* DOUBLE_DISTRIBUTION_SERVICE_H */
