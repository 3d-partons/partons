/*
 * GPDData.cpp
 *
 *  Created on: 24 juil. 2014
 *      Author: bryan
 */

#include "GPDInputData.h"

#include <sstream>

GPDInputData::GPDInputData(double _x, double _xi, double _t, double _MuF,
        double _MuR) :
        x(_x), xi(_xi), t(_t), MuF(_MuF), MuR(_MuR) {
}

std::string GPDInputData::toString() {
    std::ostringstream os;

    os << "#[GPDInputData]" << std::endl;
    os << "x = " << x << std::endl;
    os << "xi = " << xi << std::endl;
    os << "t = " << t << " Gev2" << std::endl;
    os << "MuF = " << MuF << " Gev2" << std::endl;
    os << "MuR = " << MuR << " Gev2" << std::endl;
    os << std::endl;

    return os.str();
}

double GPDInputData::getMuF() const {
    return MuF;
}

double GPDInputData::getMuR() const {
    return MuR;
}

double GPDInputData::getT() const {
    return t;
}

double GPDInputData::getX() const {
    return x;
}

double GPDInputData::getXi() const {
    return xi;
}
