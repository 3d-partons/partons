#include "../../../../../../include/partons/modules/observable/DVCS/cross_section/DVCSCrossSectionDifferenceLUMinus.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/process/ProcessModule.h"

const unsigned int DVCSCrossSectionDifferenceLUMinus::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCrossSectionDifferenceLUMinus(
                        "DVCSCrossSectionDifferenceLUMinus"));

DVCSCrossSectionDifferenceLUMinus::DVCSCrossSectionDifferenceLUMinus(
        const std::string &className) :
        Observable(className) {
    m_channel = ObservableChannel::DVCS;
}

DVCSCrossSectionDifferenceLUMinus::DVCSCrossSectionDifferenceLUMinus(
        const DVCSCrossSectionDifferenceLUMinus& other) :
        Observable(other) {
}

DVCSCrossSectionDifferenceLUMinus::~DVCSCrossSectionDifferenceLUMinus() {
}

DVCSCrossSectionDifferenceLUMinus* DVCSCrossSectionDifferenceLUMinus::clone() const {
    return new DVCSCrossSectionDifferenceLUMinus(*this);
}

void DVCSCrossSectionDifferenceLUMinus::configure(const ElemUtils::Parameters &parameters) {
	Observable::configure(parameters);
}

double DVCSCrossSectionDifferenceLUMinus::computePhiObservable(double phi) {

    double result = 0.;

    double A = m_pProcessModule->computeCrossSection(1., -1,
            NumA::Vector3D(0., 0., 0.), phi);

    double B = m_pProcessModule->computeCrossSection(-1., -1,
            NumA::Vector3D(0., 0., 0.), phi);

    result = 0.5 * (A - B);

    //integrate over transversely polarized target dependence to obtain 4-fold differential cross-section
    result *= 2 * Constant::PI;

    //change to nb
    result *= Constant::CONV_GEVm2_TO_NBARN;

    return result;
}
