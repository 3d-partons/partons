#include "../../../../../../include/partons/modules/observable/DVCS/cross_section/DVCSCrossSectionUUMinus.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/process/ProcessModule.h"

namespace PARTONS {


const unsigned int DVCSCrossSectionUUMinus::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCrossSectionUUMinus("DVCSCrossSectionUUMinus"));

DVCSCrossSectionUUMinus::DVCSCrossSectionUUMinus(const std::string &className) :
        Observable(className) {
    m_channel = ChannelType::DVCS;
}

DVCSCrossSectionUUMinus::DVCSCrossSectionUUMinus(
        const DVCSCrossSectionUUMinus& other) :
        Observable(other) {
}

DVCSCrossSectionUUMinus::~DVCSCrossSectionUUMinus() {
}

DVCSCrossSectionUUMinus* DVCSCrossSectionUUMinus::clone() const {
    return new DVCSCrossSectionUUMinus(*this);
}

void DVCSCrossSectionUUMinus::configure(const ElemUtils::Parameters &parameters) {
	Observable::configure(parameters);
}

double DVCSCrossSectionUUMinus::computePhiObservable(double phi) {

    double result = 0.;

    double A = m_pProcessModule->computeCrossSection(1., -1,
            NumA::Vector3D(0., 0., 0.), phi);

    double B = m_pProcessModule->computeCrossSection(-1., -1,
            NumA::Vector3D(0., 0., 0.), phi);

    result = (A + B) / 2.;

    //integrate over transversely polarized target dependence to obtain 4-fold differential cross-section
    result *= 2 * Constant::PI;

    //change to nb
    result *= Constant::CONV_GEVm2_TO_NBARN;

    return result;
}

} /* namespace PARTONS */
