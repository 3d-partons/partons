#include "../../../include/partons/services/ComparisonService.h"

#include "../../../include/partons/BaseObjectRegistry.h"
#include "../../../include/partons/Partons.h"

namespace PARTONS {


// Initialise [class]::classId with a unique name.
const unsigned int ComparisonService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new ComparisonService("ComparisonService"));

ComparisonService::ComparisonService(const std::string& className) :
        ServiceObject(className) {
}

ComparisonService::~ComparisonService() {
}

void ComparisonService::computeTask(Task& task) {
}

} /* namespace PARTONS */
