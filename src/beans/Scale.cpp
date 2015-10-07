#include "Scale.h"

Scale::Scale() :
        BaseObject("Scale"), m_MuF2(0.), m_MuR2(0.) {
}

Scale::~Scale() {
    // TODO Auto-generated destructor stub
}

Scale::Scale(double MuF2, double MuR2) :
        BaseObject("Scale"), m_MuF2(MuF2), m_MuR2(MuR2) {
}

double Scale::getMuF2() const {
    return m_MuF2;
}

void Scale::setMuF2(double muF2) {
    m_MuF2 = muF2;
}

double Scale::getMuR2() const {
    return m_MuR2;
}

void Scale::setMuR2(double muR2) {
    m_MuR2 = muR2;
}
