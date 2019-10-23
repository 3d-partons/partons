#include "../../../../include/partons/modules/gpd/GPDGK19.h"

#include <utility>

#include "../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../include/partons/BaseObjectRegistry.h"

namespace PARTONS {

const unsigned int GPDGK19::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GPDGK19("GPDGK19"));

GPDGK19::GPDGK19(const std::string &className) :
        GPDGK16(className) {

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::H, &GPDModule::computeH));
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::E, &GPDModule::computeE));
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::Ht, &GPDModule::computeHt));
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::Et, &GPDModule::computeEt));
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::HTrans, &GPDModule::computeHTrans));
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::ETrans, &GPDModule::computeETrans));
}

GPDGK19::GPDGK19(const GPDGK19& other) :
        GPDGK16(other) {
}

GPDGK19* GPDGK19::clone() const {
    return new GPDGK19(*this);
}

GPDGK19::~GPDGK19() {
}

void GPDGK19::initModule() {
    GPDGK16::initModule();
}

void GPDGK19::isModuleWellConfigured() {
    GPDGK16::isModuleWellConfigured();

}

PartonDistribution GPDGK19::computeH() {
    return GPDGK16::computeH();
}

PartonDistribution GPDGK19::computeE() {
    return GPDGK16::computeE();
}

PartonDistribution GPDGK19::computeHt() {
    return GPDGK16::computeHt();
}

PartonDistribution GPDGK19::computeEt() {
    return GPDGK16::computeEt();
}

PartonDistribution GPDGK19::computeHTrans() {

    //availible internal variables
    m_x, m_xi, m_t, m_MuF2, m_MuR2, m_currentGPDComputeType;
    m_MuF2_ref; // this is set in constructor of GPDGK16: m_MuF2_ref = 4.

    //variables
    double uVal, uSea, dVal, dSea, sSea, g;
    double uValMx, dValMx;

    //calculate values at (x, xi, t)
    uVal = 1.;
    dVal = 0.;

    uSea = 0.;
    dSea = 0.;
    sSea = 0.;

    g = 0.;

    //calculate values at (-x, xi, t)
    uValMx = 0.;
    dValMx = 0.;

    //store
    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    quarkDistribution_u.setQuarkDistribution(uVal + uSea);
    quarkDistribution_d.setQuarkDistribution(dVal + dSea);
    quarkDistribution_s.setQuarkDistribution(sSea);

    quarkDistribution_u.setQuarkDistributionPlus(uVal - uValMx + 2 * uSea);
    quarkDistribution_d.setQuarkDistributionPlus(dVal - dValMx + 2 * dSea);
    quarkDistribution_s.setQuarkDistributionPlus(2 * sSea);

    quarkDistribution_u.setQuarkDistributionMinus(uVal + uValMx);
    quarkDistribution_d.setQuarkDistributionMinus(dVal + dValMx);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    GluonDistribution gluonDistribution(g);

    PartonDistribution partonDistribution;

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    //return
    return partonDistribution;
}

PartonDistribution GPDGK19::computeETrans() {

    //availible internal variables
    m_x, m_xi, m_t, m_MuF2, m_MuR2, m_currentGPDComputeType;
    m_MuF2_ref; // this is set in constructor of GPDGK16: m_MuF2_ref = 4.

    //variables
    double uVal, uSea, dVal, dSea, sSea, g;
    double uValMx, dValMx;

    //calculate values at (x, xi, t)
    uVal = 2.;
    dVal = 0.;

    uSea = 0.;
    dSea = 0.;
    sSea = 0.;

    g = 0.;

    //calculate values at (-x, xi, t)
    uValMx = 0.;
    dValMx = 0.;

    //store
    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    quarkDistribution_u.setQuarkDistribution(uVal + uSea);
    quarkDistribution_d.setQuarkDistribution(dVal + dSea);
    quarkDistribution_s.setQuarkDistribution(sSea);

    quarkDistribution_u.setQuarkDistributionPlus(uVal - uValMx + 2 * uSea);
    quarkDistribution_d.setQuarkDistributionPlus(dVal - dValMx + 2 * dSea);
    quarkDistribution_s.setQuarkDistributionPlus(2 * sSea);

    quarkDistribution_u.setQuarkDistributionMinus(uVal + uValMx);
    quarkDistribution_d.setQuarkDistributionMinus(dVal + dValMx);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    GluonDistribution gluonDistribution(g);

    PartonDistribution partonDistribution;

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    //return
    return partonDistribution;
}

} /* namespace PARTONS */
