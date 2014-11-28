#include "GPDOutputData.h"

#include <sstream>
#include <utility>

GPDOutputData::GPDOutputData() {
}

GPDOutputData::~GPDOutputData() {
    m_gpdResults.clear();
}

void GPDOutputData::addGPDResultData(GPDResultData gpdResultData) {
    m_gpdResults.insert(
            std::pair<GPDComputeType::Type, GPDResultData>(
                    gpdResultData.getGpdComputeType()->getType(),
                    gpdResultData));
}

GPDResultData* GPDOutputData::getGPDResultData(
        GPDComputeType::Type _gpdComputeType) {

    m_it = m_gpdResults.find(_gpdComputeType);
    return (m_it != m_gpdResults.end()) ? &(m_it->second) : 0;

    //return &(gpdResults[_gpdComputeType]);
}

//TODO tester cette méthode
std::vector<GPDComputeType> GPDOutputData::listGPDTypeComputed() {
    std::vector<GPDComputeType> list;

    if (m_gpdResults.size() != 0) {

        for (std::map<GPDComputeType::Type, GPDResultData>::iterator it =
                m_gpdResults.begin(); it != m_gpdResults.end(); ++it) {
            list.push_back(it->first);
        }
    }

    return list;
}

std::string GPDOutputData::toString() {

    std::ostringstream os;

    os << std::endl << "#[GPDOutputData]" << std::endl;

    if (m_gpdResults.size() != 0) {
        for (std::map<GPDComputeType::Type, GPDResultData>::iterator it =
                m_gpdResults.begin(); it != m_gpdResults.end(); ++it) {
            os << (it->second).toString() << std::endl;
        }
    }

    os << std::endl;

    return os.str();
}

