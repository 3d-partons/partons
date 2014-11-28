#include "CFFOutputData.h"

#include <stdexcept>
#include <utility>

#include "../../utils/stringUtils/Formatter.h"
//#include "CFFInputData.h"

CFFOutputData::CFFOutputData(CFFInputData cffInputData)
        : m_cffInputData(cffInputData) {
}

CFFOutputData::CFFOutputData() {
}

CFFOutputData::~CFFOutputData() {
}

std::string CFFOutputData::toString() {
    Formatter formatter;
    formatter << "[CFFOutputData]" << "\n";

    for (m_it = m_listCFFbyGPDComputeType.begin();
            m_it != m_listCFFbyGPDComputeType.end(); m_it++) {
        formatter << "CFF_" << GPDComputeType(m_it->first).toString() << " = "
                << (m_it->second).real() << ", " << (m_it->second).imag()
                << "\n";
    }

        formatter << m_cffInputData.toString();

    return formatter.str();
}

void CFFOutputData::add(GPDComputeType::Type gpdComputeType,
        std::complex<double> cff) {
    m_listCFFbyGPDComputeType.insert(std::make_pair(gpdComputeType, cff));
}

std::complex<double> CFFOutputData::getCFF(
        GPDComputeType::Type gpdComputeType) {
    m_it = m_listCFFbyGPDComputeType.find(gpdComputeType);
    if (m_it != m_listCFFbyGPDComputeType.end()) {
        return m_it->second;
    }

    throw std::runtime_error(
            Formatter()
                    << "[CFFOutputData::getCFF] cannot find CFF for GPDComputeType = "
                    << GPDComputeType(gpdComputeType).toString());
}

// ##### GETTERS & SETTERS #####

const CFFInputData& CFFOutputData::getCffInputData() const {
    return m_cffInputData;
}

void CFFOutputData::setCffInputData(const CFFInputData& cffInputData) {
    m_cffInputData = cffInputData;
}
