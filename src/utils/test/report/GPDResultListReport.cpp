#include "GPDResultListReport.h"

#include <sstream>

GPDResultListReport::GPDResultListReport() :
        ComparisonReport(), m_sameSize(false), m_lhsGPDResultListSize(0), m_rhsGPDResultListSize(
                0) {
}

GPDResultListReport::~GPDResultListReport() {
}

void GPDResultListReport::add(const GPDResultReport& gpdResultReport) {
    m_gpdResultReports.push_back(gpdResultReport);
}

const GPDResultReport& GPDResultListReport::get(unsigned int index) const {
    return m_gpdResultReports.at(index);
}

size_t GPDResultListReport::getSize() const {
    return m_gpdResultReports.size();
}

const GPDResultReport& GPDResultListReport::getUniqueReport() {
    return m_gpdResultReports.at(0);
}

std::string GPDResultListReport::toString() const {

    std::ostringstream os;

    if (m_sameSize) {
        if (!m_comparisonResult) {
            os << "List of GPD results indices where comparison failed:"
                    << std::endl;
            for (unsigned int j = 0; j < m_differentResultIndex.size(); j++) {
                os << m_differentResultIndex.at(j) << "    ";
            }
            os << std::endl;
        }
        for (unsigned int i = 0; i != m_gpdResultReports.size(); i++) {
            os << m_gpdResultReports.at(i).toString() << std::endl;
        }
    } else {
        os << "The compared lists do not have the same size." << std::endl;
        os << "Size of lhs list:" << m_lhsGPDResultListSize << " elements"
                << std::endl;
        os << "Size of rhs list:" << m_rhsGPDResultListSize << " elements"
                << std::endl;
    }

    return os.str();
}

bool GPDResultListReport::isSameSize() const {
    return m_sameSize;
}

void GPDResultListReport::setSameSize(bool sameSize) {
    m_sameSize = sameSize;
}

const std::vector<unsigned int>& GPDResultListReport::getDifferentResultIndex() const {
    return m_differentResultIndex;
}

void GPDResultListReport::setDifferentResultIndex(
        const std::vector<unsigned int>& differentResultIndex) {
    m_differentResultIndex = differentResultIndex;
}

unsigned int GPDResultListReport::getLhsGpdResultListSize() const {
    return m_lhsGPDResultListSize;
}

void GPDResultListReport::setLhsGpdResultListSize(
        unsigned int lhsGpdResultListSize) {
    m_lhsGPDResultListSize = lhsGpdResultListSize;
}

unsigned int GPDResultListReport::getRhsGpdResultListSize() const {
    return m_rhsGPDResultListSize;
}

void GPDResultListReport::setRhsGpdResultListSize(
        unsigned int rhsGpdResultListSize) {
    m_rhsGPDResultListSize = rhsGpdResultListSize;
}
