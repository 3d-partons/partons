#include "../../../../../../include/partons/modules/observable/DVCS/cross_section/DVCSCrossSectionUUMinus.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../../../../include/partons/modules/process/ProcessModule.h"

namespace PARTONS {

const unsigned int DVCSCrossSectionUUMinus::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCrossSectionUUMinus("DVCSCrossSectionUUMinus"));

DVCSCrossSectionUUMinus::DVCSCrossSectionUUMinus(const std::string &className) :
        DVCSObservable(className, ObservableType::PHI) {
}

DVCSCrossSectionUUMinus::DVCSCrossSectionUUMinus(
        const DVCSCrossSectionUUMinus& other) :
        DVCSObservable(other) {
}

DVCSCrossSectionUUMinus::~DVCSCrossSectionUUMinus() {
}

DVCSCrossSectionUUMinus* DVCSCrossSectionUUMinus::clone() const {
    return new DVCSCrossSectionUUMinus(*this);
}

double DVCSCrossSectionUUMinus::computePhiDVCSObservable(
        const DVCSObservableKinematic& kinematic) {

    //result
    double result = 0.;

    //evaluate
    double A = m_pProcessModule->compute(1., -1, NumA::Vector3D(0., 0., 0.),
            kinematic);

    double B = m_pProcessModule->compute(-1., -1, NumA::Vector3D(0., 0., 0.),
            kinematic);

    result = (A + B) / 2.;

    //integrate over transversely polarized target dependence to obtain 4-fold differential cross-section
    result *= 2 * Constant::PI;

    //change to nb
    result *= Constant::CONV_GEVm2_TO_NBARN;

    return result;
}

} /* namespace PARTONS */
