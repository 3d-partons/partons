#include "CFFOutputData.h"

#include "../utils/stringUtils/Formatter.h"

CFFOutputData::CFFOutputData(CFFInputData cffInputData) :
        m_cff(0., 0.), m_cffInputData(cffInputData) {

}

CFFOutputData::CFFOutputData(std::complex<double> cff,
        CFFInputData cffInputData) :
        m_cff(cff), m_cffInputData(cffInputData) {

}

std::string CFFOutputData::toString() {
    return Formatter() << "[CFFOutputData]" << "\n" << m_cff.real() << " "
            << m_cff.imag() << "\n" << m_cffInputData.toString();
}

std::complex<double> CFFOutputData::getCff() const {
    return m_cff;
}

void CFFOutputData::setCff(std::complex<double> cff) {
    m_cff = cff;
}

const CFFInputData& CFFOutputData::getCffInputData() const {
    return m_cffInputData;
}

void CFFOutputData::setCffInputData(const CFFInputData& cffInputData) {
    m_cffInputData = cffInputData;
}
