#include "../../../../include/partons/modules/observable/CrossSectionBeamMinusUU.h"

#include <NumA/linear_algebra/vector/Vector3D.h>
#include <cmath>

#include "../../../../include/partons/beans/observable/ObservableChannel.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/modules/ProcessModule.h"

const unsigned int CrossSectionBeamMinusUU::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new CrossSectionBeamMinusUU("CrossSectionBeamMinusUU"));

CrossSectionBeamMinusUU::CrossSectionBeamMinusUU(const std::string &className) :
        Observable(className) {
    m_channel = ObservableChannel::DVCS;
}

CrossSectionBeamMinusUU::CrossSectionBeamMinusUU(
        const CrossSectionBeamMinusUU& other) :
        Observable(other) {
}

CrossSectionBeamMinusUU::~CrossSectionBeamMinusUU() {
}

CrossSectionBeamMinusUU* CrossSectionBeamMinusUU::clone() const {
    return new CrossSectionBeamMinusUU(*this);
}

double CrossSectionBeamMinusUU::compute(double phi) {

    double result = 0.;

    double A = m_pProcessModule->computeCrossSection(1., -1,
            NumA::Vector3D(0., 0., +1.), phi);

    double B = m_pProcessModule->computeCrossSection(-1., -1,
            NumA::Vector3D(0., 0., +1.), phi);

    double C = m_pProcessModule->computeCrossSection(1., -1,
            NumA::Vector3D(0., 0., -1.), phi);

    double D = m_pProcessModule->computeCrossSection(-1., -1,
            NumA::Vector3D(0., 0., -1.), phi);

    result = (A + B + C + D) / 4.;

    result *= 2 * M_PI; //integrate over transversely polarized target dependence to obtain 4-fold differential cross-section
    result *= 0.3894 * 1.E6; //change to nb

    return result;
}
