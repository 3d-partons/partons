#include <cmath>
#include <vector>

#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/gpd/GPDGK11NoQuarks.h"

// Initialise [class]::classId with a unique name.
const unsigned int GPDGK11NoQuarks::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GPDGK11NoQuarks("GPDGK11NoQuarks"));

GPDGK11NoQuarks::GPDGK11NoQuarks(const std::string& className) :
        GPDGK11(className) {
}

GPDGK11NoQuarks::~GPDGK11NoQuarks() {
}

GPDGK11NoQuarks* GPDGK11NoQuarks::clone() const {
    return new GPDGK11NoQuarks(*this);
}

GPDGK11NoQuarks::GPDGK11NoQuarks(const GPDGK11NoQuarks& other) :
        GPDGK11(other) {
}

PartonDistribution GPDGK11NoQuarks::computeH() {
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

    /*    debug(__func__,
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
PartonDistribution GPDGK11NoQuarks::computeE() {
    return PartonDistribution();
}

//TODO implement
PartonDistribution GPDGK11NoQuarks::computeHt() {
    return PartonDistribution();
}

//TODO implement
PartonDistribution GPDGK11NoQuarks::computeEt() {
    return PartonDistribution();
}
