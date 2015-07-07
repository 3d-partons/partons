#include "GPDResultListReport.h"

#include <sstream>

GPDResultListReport::GPDResultListReport() :
        ComparisonReport(), m_sameSize(false) {
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

    for (unsigned int i = 0; i != m_gpdResultReports.size(); i++) {
        os << m_gpdResultReports.at(i).toString() << std::endl;
    }

    return os.str();
}

bool GPDResultListReport::isSameSize() const {
    return m_sameSize;
}

void GPDResultListReport::setSameSize(bool sameSize) {
    m_sameSize = sameSize;
}
