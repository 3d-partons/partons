#include "../../../../../../include/partons/modules/observable/DVCS/other/DVCSCFFxiReHt.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../../../../include/partons/modules/xi_converter/DVCS/DVCSXiConverterModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSCFFxiReHt::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCFFxiReHt("DVCSCFFxiReHt"));

DVCSCFFxiReHt::DVCSCFFxiReHt(const std::string &className) :
        DVCSObservable(className) {
}

DVCSCFFxiReHt::DVCSCFFxiReHt(
        const DVCSCFFxiReHt& other) :
        DVCSObservable(other) {
}

DVCSCFFxiReHt::~DVCSCFFxiReHt() {
}

DVCSCFFxiReHt* DVCSCFFxiReHt::clone() const {
    return new DVCSCFFxiReHt(*this);
}

PhysicalType<double> DVCSCFFxiReHt::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //evaluate
    double xi = m_pProcessModule->getXiConverterModule()->compute(kinematic).getValue();
    double cff = m_pProcessModule->getConvolCoeffFunctionValue(GPDType::Ht).real();

    //change to nb
    return PhysicalType<double>(xi * cff, PhysicalUnit::NONE);
}

} /* namespace PARTONS */
