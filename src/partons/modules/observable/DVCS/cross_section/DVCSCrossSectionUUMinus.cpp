#include "../../../../../../include/partons/modules/observable/DVCS/cross_section/DVCSCrossSectionUUMinus.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/DVCS/DVCSObservableResult.h"
#include "../../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

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

PhysicalType<double> DVCSCrossSectionUUMinus::computePhiDVCSObservable(
        const DVCSObservableKinematic& kinematic) {

    //TODO operation at PysicalType

    //result
    double result = 0.;

    //evaluate
    DVCSObservableResult A = m_pProcessModule->compute(1., -1,
            NumA::Vector3D(0., 0., 0.), kinematic);

    DVCSObservableResult B = m_pProcessModule->compute(-1., -1,
            NumA::Vector3D(0., 0., 0.), kinematic);

    //combine
    result = (A.getValue().getValue() + B.getValue().getValue()) / 2.;

    //integrate over transversely polarized target dependence to obtain 4-fold differential cross-section
    result *= 2 * Constant::PI;

    //change to nb
    result *= Constant::CONV_GEVm2_TO_NBARN;

    return PhysicalType<double>(result, PhysicalUnit::NB);
}

} /* namespace PARTONS */
