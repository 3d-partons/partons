/*
 * GK11NoQuarksModel.cpp
 *
 *  Created on: Feb 13, 2015
 *      Author: kuba
 */

#include "GK11NoQuarksModel.h"

#include <cmath>
#include <vector>

#include "../../beans/parton_distribution/GluonDistribution.h"
#include "../../beans/parton_distribution/PartonDistribution.h"
#include "../../beans/parton_distribution/QuarkDistribution.h"
#include "../../beans/QuarkFlavor.h"
#include "../../BaseObjectRegistry.h"

// Initialise [class]::classId with a unique name.
const unsigned int GK11NoQuarksModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GK11NoQuarksModel("GK11NoQuarksModel"));

GK11NoQuarksModel::GK11NoQuarksModel(const std::string& className) :
        GK11Model(className) {
}

GK11NoQuarksModel::~GK11NoQuarksModel() {
    // TODO Auto-generated destructor stub
}

GK11NoQuarksModel* GK11NoQuarksModel::clone() const {
    return new GK11NoQuarksModel(*this);
}

GK11NoQuarksModel::GK11NoQuarksModel(const GK11NoQuarksModel& other) :
        GK11Model(other) {
}

PartonDistribution GK11NoQuarksModel::computeH() {
    PartonDistribution partonDistribution;

    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    calculateHCoefs(); // Calculate the K's and the coefficients

// Gluons

    c1 = 2.23 + 0.362 * fL; // See table 1 p. 12
    c2 = 5.43 - 7.0 * fL; // See table 1 p. 12
    c3 = -34.0 + 22.5 * fL; // See table 1 p. 12
    c4 = 40.6 - 21.6 * fL; // See table 1 p. 12
    b0 = 2.58 + 0.25 * log(0.880354 / (0.880354 + m_MuF2)); // See eq. (39) p. 14

    GluonDistribution gluonDistribution(
            exp(b0 * m_t)
                    * (c1 * Hi1tab.at(0) + c2 * Hi1tab.at(1) + c3 * Hi1tab.at(2)
                            + c4 * Hi1tab.at(3))); // See eq. (27)

    /*    m_pLoggerManager->debug(getClassName(), __func__,
     Formatter() << "1st coef " << Hi1tab.at(0)
     << "2nd coef " << Hi1tab.at(1)
     << "3rd coef " << Hi1tab.at(2)
     << "4rdt coef " << Hi1tab.at(3)) ;*/

// quarks set to zero
    quarkDistribution_u.setQuarkDistribution(0.);
    quarkDistribution_d.setQuarkDistribution(0.);
    quarkDistribution_s.setQuarkDistribution(0.);

    // Set Hq(+)
    quarkDistribution_u.setQuarkDistributionPlus(0.);
    quarkDistribution_d.setQuarkDistributionPlus(0.);
    quarkDistribution_s.setQuarkDistributionPlus(0.);

    // Set Hq(-)
    quarkDistribution_u.setQuarkDistributionMinus(0.);
    quarkDistribution_d.setQuarkDistributionMinus(0.);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}

//TODO implement
PartonDistribution GK11NoQuarksModel::computeE() {
    return PartonDistribution();
}

//TODO implement
PartonDistribution GK11NoQuarksModel::computeHt() {
    return PartonDistribution();
}

//TODO implement
PartonDistribution GK11NoQuarksModel::computeEt() {
    return PartonDistribution();
}
