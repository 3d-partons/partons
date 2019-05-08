#include "../../../include/partons/services/VizualisationService.h"

#include "../../../include/partons/BaseObjectRegistry.h"
#include "../../../include/partons/Partons.h"
#include "../../../include/partons/utils/plot2D/Plot2DList.h"

namespace PARTONS {

const unsigned int VizualisationService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new VizualisationService("VizualisationService"));

VizualisationService::VizualisationService(const std::string &className) :
        ServiceObject(className) {
}

VizualisationService::~VizualisationService() {
}

void VizualisationService::computeTask(Task &task) {
}

Plot2DList VizualisationService::getPlot2DFromSQLQuery(
        const std::string &sqlQuery) {

    Plot2DList plot2DList;
    return plot2DList;
}

} /* namespace PARTONS */
