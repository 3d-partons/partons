#include "../../../../../../include/partons/modules/observable/DVCS/cross_section/DVCSCrossSectionUUBHSubProc.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/beans/process/DVCSSubProcessType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"

namespace PARTONS {

const unsigned int DVCSCrossSectionUUBHSubProc::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCrossSectionUUBHSubProc("DVCSCrossSectionUUBHSubProc"));

DVCSCrossSectionUUBHSubProc::DVCSCrossSectionUUBHSubProc(
        const std::string &className) :
        Observable(className) {
    m_channel = ChannelType::DVCS;
}

DVCSCrossSectionUUBHSubProc::DVCSCrossSectionUUBHSubProc(
        const DVCSCrossSectionUUBHSubProc& other) :
        Observable(other) {
}

DVCSCrossSectionUUBHSubProc::~DVCSCrossSectionUUBHSubProc() {
}

DVCSCrossSectionUUBHSubProc* DVCSCrossSectionUUBHSubProc::clone() const {
    return new DVCSCrossSectionUUBHSubProc(*this);
}

void DVCSCrossSectionUUBHSubProc::configure(
        const ElemUtils::Parameters &parameters) {
    Observable::configure(parameters);
}

double DVCSCrossSectionUUBHSubProc::computePhiObservable(double phi) {

    double result = 0.;

    //charge does not matter
    double A =
            static_cast<DVCSProcessModule*>(m_pProcessModule)->computeCrossSection(
                    1., -1, NumA::Vector3D(0., 0., 0.), phi,
                    DVCSSubProcessType::BH);

    double B =
            static_cast<DVCSProcessModule*>(m_pProcessModule)->computeCrossSection(
                    -1., -1, NumA::Vector3D(0., 0., 0.), phi,
                    DVCSSubProcessType::BH);

    result = (A + B) / 2.;

    //integrate over transversely polarized target dependence to obtain 4-fold differential cross-section
    result *= 2 * Constant::PI;

    //change to nb
    result *= Constant::CONV_GEVm2_TO_NBARN;

    return result;
}

} /* namespace PARTONS */
