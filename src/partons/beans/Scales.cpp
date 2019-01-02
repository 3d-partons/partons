#include "../../../include/partons/beans/Scales.h"

#include <ElementaryUtils/string_utils/Formatter.h>

namespace PARTONS {


Scales::Scales() :
        BaseObject("Scale"), m_MuF2(0.), m_MuR2(0.) {
}

Scales::~Scales() {
    // TODO Auto-generated destructor stub
}

Scales::Scales(double MuF2, double MuR2) :
        BaseObject("Scale"), m_MuF2(MuF2), m_MuR2(MuR2) {
}

double Scales::getMuF2() const {
    return m_MuF2;
}

void Scales::setMuF2(double muF2) {
    m_MuF2 = muF2;
}

double Scales::getMuR2() const {
    return m_MuR2;
}

void Scales::setMuR2(double muR2) {
    m_MuR2 = muR2;
}

std::string Scales::toString() const {
    return ElemUtils::Formatter() << "m_MuF2 = " << m_MuF2 << " (GeV2) m_MuR2 = "
            << m_MuR2 << " (GeV2)";
}

} /* namespace PARTONS */
