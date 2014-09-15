#include "GPDOutputData.h"

#include <sstream>
#include <utility>

GPDOutputData::GPDOutputData() {
}

GPDOutputData::~GPDOutputData() {
    for (std::map<GPDComputeType::Type, GPDResultData*>::iterator it =
            gpdResults.begin(); it != gpdResults.end(); ++it) {
        if ((it->second) != 0) {
            delete (it->second);
            (it->second) = 0;
        }
    }

    gpdResults.clear();
}

void GPDOutputData::addGPDResultData(GPDResultData* _pGPDResultData) {
    gpdResults.insert(
            std::pair<GPDComputeType::Type, GPDResultData*>(
                    _pGPDResultData->getGpdComputeType()->t_, _pGPDResultData));
}

GPDResultData* GPDOutputData::getGPDResultData(
        GPDComputeType::Type _gpdComputeType) {
    return gpdResults[_gpdComputeType];
}

//TODO tester cette méthode
std::vector<GPDComputeType> GPDOutputData::listGPDTypeComputed() {
    std::vector<GPDComputeType> list;

    if (gpdResults.size() != 0) {

        for (std::map<GPDComputeType::Type, GPDResultData*>::iterator it =
                gpdResults.begin(); it != gpdResults.end(); ++it) {
            list.push_back(it->first);
        }
    }

    return list;
}

std::string GPDOutputData::toString() {

    std::ostringstream os;

    os << std::endl << "#[GPDOutputData]" << std::endl;

    if (gpdResults.size() != 0) {
        for (std::map<GPDComputeType::Type, GPDResultData*>::iterator it =
                gpdResults.begin(); it != gpdResults.end(); ++it) {
            os << (it->second)->toString() << std::endl;
        }
    }

    os << std::endl;

    return os.str();
}

