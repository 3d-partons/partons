#include "../../../../include/partons/modules/observable/CrossSectionDifferenceBeamMinusLU.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/modules/ProcessModule.h"

const unsigned int CrossSectionDifferenceBeamMinusLU::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new CrossSectionDifferenceBeamMinusLU(
                        "CrossSectionDifferenceBeamMinusLU"));

CrossSectionDifferenceBeamMinusLU::CrossSectionDifferenceBeamMinusLU(
        const std::string &className) :
        Observable(className) {
    m_channel = ObservableChannel::DVCS;
}

CrossSectionDifferenceBeamMinusLU::CrossSectionDifferenceBeamMinusLU(
        const CrossSectionDifferenceBeamMinusLU& other) :
        Observable(other) {
}

CrossSectionDifferenceBeamMinusLU::~CrossSectionDifferenceBeamMinusLU() {
}

CrossSectionDifferenceBeamMinusLU* CrossSectionDifferenceBeamMinusLU::clone() const {
    return new CrossSectionDifferenceBeamMinusLU(*this);
}

double CrossSectionDifferenceBeamMinusLU::computePhiObservable(double phi) {

    double result = 0.;

    double A = m_pProcessModule->computeCrossSection(1., -1,
            NumA::Vector3D(0., 0., 0.), phi);

    double B = m_pProcessModule->computeCrossSection(-1., -1,
            NumA::Vector3D(0., 0., 0.), phi);

    result = 0.5*(A - B);

    result *= 2 * Constant::PI; //integrate over transversely polarized target dependence to obtain 4-fold differential cross-section
    result *= 0.3894 * 1.E6; //change to nb

    return result;
}
