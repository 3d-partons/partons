#include "KinematicVariables.h"

#include <sstream>
#include <stdexcept>

KinematicVariables::KinematicVariables() :
        xB(0.), t(0.), Q2(0.), xi(0.) {
    updateXi();
}

KinematicVariables::KinematicVariables(double _xB, double _t, double _Q2) :
        xB(_xB), t(_t), Q2(_Q2), xi(0.) {
    updateXi();
}

void KinematicVariables::updateXi() {
    double denom = (2. - xB);
    if (denom != 0) {
        xi = xB / denom;
    } else {
        std::ostringstream os;
        os << "[KinematicVariables] updateXi() : divided by ZERO with xB = "
                << xB << std::endl;
        throw std::invalid_argument(os.str());
    }
}

double KinematicVariables::getQ2() const {
    return Q2;
}

void KinematicVariables::setQ2(double _Q2) {
    Q2 = _Q2;
}

double KinematicVariables::getT() const {
    return t;
}

void KinematicVariables::setT(double _t) {
    t = _t;
}

double KinematicVariables::getXB() const {
    return xB;
}

void KinematicVariables::setXB(double _xB) {
    xB = _xB;
}

double KinematicVariables::getXi() const {
    return xi;
}
