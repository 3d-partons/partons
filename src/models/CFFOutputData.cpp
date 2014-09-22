#include "CFFOutputData.h"

#include "../utils/stringUtils/Formatter.h"

CFFOutputData::CFFOutputData(std::complex<double> cff, CFFInputData cffInputData) :
        m_cff(cff), m_cffInputData(cffInputData) {

}

std::string CFFOutputData::toString() {
    return Formatter() << "[CFFOutputData]" << "\n" << m_cff.real() << " "
            << m_cff.imag() << "\n" << m_cffInputData.toString();
}
