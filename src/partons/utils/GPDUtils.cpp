#include "../../../include/partons/utils/GPDUtils.h"

#include <stddef.h>
#include <vector>

#include "../../../include/partons/database/gpd/service/GPDResultDaoService.h"

List<GPDKinematic> GPDUtils::getKinematicListByIntervals(
        const NumA::Interval<double> &xInterval,
        const NumA::Interval<double> &xiInterval,
        const NumA::Interval<double> &tInterval,
        const NumA::Interval<double> &MuFInterval,
        const NumA::Interval<double> &MuRInterval) {

    std::vector<double> xSteps = xInterval.computeSteps();
    std::vector<double> xiSteps = xiInterval.computeSteps();
    std::vector<double> tSteps = tInterval.computeSteps();
    std::vector<double> MuFSteps = MuFInterval.computeSteps();
    std::vector<double> MuRSteps = MuRInterval.computeSteps();

    List<GPDKinematic> kinematicList;

    for (size_t MuR = 0; MuR != MuRSteps.size(); MuR++) {
        for (size_t MuF = 0; MuF != MuFSteps.size(); MuF++) {
            for (size_t t = 0; t != tSteps.size(); t++) {
                for (size_t xi = 0; xi != xiSteps.size(); xi++) {
                    for (size_t x = 0; x != xSteps.size(); x++) {
                        kinematicList.add(GPDKinematic(x, xi, t, MuF, MuR));
                    }
                }
            }
        }
    }
    return kinematicList;
}

List<GPDKinematic> GPDUtils::getKinematicListFromFile(
        const std::string &filePath) {
    List<GPDKinematic> kinematicList;

    //TODO implement

    return kinematicList;
}

List<GPDKinematic> GPDUtils::getKinematicListFromResultList(
        const List<GPDResult>& gpdResultList) {
    List<GPDKinematic> kinematicList;

    for (size_t i = 0; i != gpdResultList.size(); i++) {
        kinematicList.add(gpdResultList[i].getKinematic());
    }

    return kinematicList;
}

List<GPDResult> GPDUtils::getResultListFromDatabase(int computationId) {
    GPDResultDaoService service;
    return service.getGPDResultListByComputationId(computationId);
}
