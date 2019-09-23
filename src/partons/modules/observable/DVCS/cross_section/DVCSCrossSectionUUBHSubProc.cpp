#include "../../../../../../include/partons/modules/observable/DVCS/cross_section/DVCSCrossSectionUUBHSubProc.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../../include/partons/beans/process/VCSSubProcessType.h"
#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSCrossSectionUUBHSubProc::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCrossSectionUUBHSubProc("DVCSCrossSectionUUBHSubProc"));

DVCSCrossSectionUUBHSubProc::DVCSCrossSectionUUBHSubProc(
        const std::string &className) :
        DVCSObservable(className) {
}

DVCSCrossSectionUUBHSubProc::DVCSCrossSectionUUBHSubProc(
        const DVCSCrossSectionUUBHSubProc& other) :
        DVCSObservable(other) {
}

DVCSCrossSectionUUBHSubProc::~DVCSCrossSectionUUBHSubProc() {
}

DVCSCrossSectionUUBHSubProc* DVCSCrossSectionUUBHSubProc::clone() const {
    return new DVCSCrossSectionUUBHSubProc(*this);
}

PhysicalType<double> DVCSCrossSectionUUBHSubProc::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //evaluate
    DVCSObservableResult A = m_pProcessModule->compute(1., -1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType,
            VCSSubProcessType::BH);

    DVCSObservableResult B = m_pProcessModule->compute(-1., -1,
            NumA::Vector3D(0., 0., 0.), kinematic, gpdType,
            VCSSubProcessType::BH);

    //combine
    PhysicalType<double> result = (A.getValue() + B.getValue()) / 2.;

    //integrate over transversely polarized target dependence to obtain 4-fold differential cross-section
    result *= 2. * Constant::PI;

    //change to nb
    return result.makeSameUnitAs(PhysicalUnit::NB);
}

} /* namespace PARTONS */
