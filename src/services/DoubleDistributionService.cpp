#include "DoubleDistributionService.h"

#include "../beans/double_distribution/DoubleDistributionKinematic.h"
#include "../beans/double_distribution/DoubleDistributionResult.h"
#include "../beans/double_distribution/DoubleDistributionType.h"
#include "../BaseObjectRegistry.h"
#include "../modules/DoubleDistributionModule.h"

// Initialise [class]::classId with a unique name and selfregister this module into the global registry.
const unsigned int DoubleDistributionService::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DoubleDistributionService("DoubleDistributionService"));

DoubleDistributionService::DoubleDistributionService(
        const std::string &className) :
        ServiceObject(className) {
}

DoubleDistributionService::~DoubleDistributionService() {
}

//TODO implement all function
void DoubleDistributionService::computeTask(Task &task) {
}

DoubleDistributionResult DoubleDistributionService::compute(
        const DoubleDistributionKinematic& kinematic,
        DoubleDistributionModule* pDoubleDistributionModule) {
    return pDoubleDistributionModule->compute(kinematic.getBeta(),
            kinematic.getAlpha(), kinematic.getT(), kinematic.getMuF2(),
            kinematic.getMuR2(), kinematic.getDoubleDistributionType());
}
