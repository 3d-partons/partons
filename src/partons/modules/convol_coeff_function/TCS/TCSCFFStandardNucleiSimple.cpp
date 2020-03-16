/*
 * TCSCFFStandardNucleiSimple.cpp
 *
 *  Created on: Mar 11, 2020
 *      Author: partons
 */

#include "../../../../../include/partons/modules/convol_coeff_function/TCS/TCSCFFStandardNucleiSimple.h"

#include "../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/gpd/GPDModule.h"

namespace PARTONS {
class PartonDistribution;
} /* namespace PARTONS */

namespace PARTONS {

const unsigned int TCSCFFStandardNucleiSimple::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSCFFStandardNucleiSimple("TCSCFFStandardNucleiSimple"));

TCSCFFStandardNucleiSimple::TCSCFFStandardNucleiSimple(
        const std::string &className) :
        TCSCFFStandard(className), Nuclei(className) {
}

TCSCFFStandardNucleiSimple::TCSCFFStandardNucleiSimple(
        const TCSCFFStandardNucleiSimple& other) :
        TCSCFFStandard(other), Nuclei(other) {
}

TCSCFFStandardNucleiSimple::~TCSCFFStandardNucleiSimple() {
}

TCSCFFStandardNucleiSimple* TCSCFFStandardNucleiSimple::clone() const {
    return new TCSCFFStandardNucleiSimple(*this);
}

void TCSCFFStandardNucleiSimple::configure(
        const ElemUtils::Parameters &parameters) {

    TCSCFFStandard::configure(parameters);
    Nuclei::configure(parameters);
}

void TCSCFFStandardNucleiSimple::computeDiagonalGPD() {

    PartonDistribution partonDistribution = m_pGPDModule->compute(
            GPDKinematic(m_xi, m_xi, m_t, m_MuF2, m_MuR2),
            m_currentGPDComputeType);

    m_quarkDiagonal = computeSquareChargeAveragedGPD(partonDistribution);
    m_gluonDiagonal = m_A * 2.
            * partonDistribution.getGluonDistribution().getGluonDistribution();
}

double TCSCFFStandardNucleiSimple::computeSquareChargeAveragedGPD(
        const PartonDistribution& partonDistribution) {

    double resultP = 0.;

    resultP +=
            (partonDistribution.getQuarkDistribution(QuarkFlavor::UP).getQuarkDistributionPlus())
                    * Constant::U2_ELEC_CHARGE;

    resultP +=
            (partonDistribution.getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistributionPlus())
                    * Constant::D2_ELEC_CHARGE;
    resultP +=
            (partonDistribution.getQuarkDistribution(QuarkFlavor::STRANGE).getQuarkDistributionPlus())
                    * Constant::S2_ELEC_CHARGE;

    double resultN = 0.;

    resultN +=
            (partonDistribution.getQuarkDistribution(QuarkFlavor::UP).getQuarkDistributionPlus())
                    * Constant::D2_ELEC_CHARGE;

    resultN +=
            (partonDistribution.getQuarkDistribution(QuarkFlavor::DOWN).getQuarkDistributionPlus())
                    * Constant::U2_ELEC_CHARGE;
    resultN +=
            (partonDistribution.getQuarkDistribution(QuarkFlavor::STRANGE).getQuarkDistributionPlus())
                    * Constant::S2_ELEC_CHARGE;

    return m_Z * resultP + (m_A - m_Z) * resultN;
}

} /* namespace PARTONS */
