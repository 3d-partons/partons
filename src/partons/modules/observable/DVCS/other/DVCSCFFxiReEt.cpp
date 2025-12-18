#include "../../../../../../include/partons/modules/observable/DVCS/other/DVCSCFFxiReEt.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../../../../include/partons/modules/xi_converter/DVCS/DVCSXiConverterModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSCFFxiReEt::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCFFxiReEt("DVCSCFFxiReEt"));

DVCSCFFxiReEt::DVCSCFFxiReEt(const std::string &className) :
        DVCSObservable(className) {
}

DVCSCFFxiReEt::DVCSCFFxiReEt(
        const DVCSCFFxiReEt& other) :
        DVCSObservable(other) {
}

DVCSCFFxiReEt::~DVCSCFFxiReEt() {
}

DVCSCFFxiReEt* DVCSCFFxiReEt::clone() const {
    return new DVCSCFFxiReEt(*this);
}

PhysicalType<double> DVCSCFFxiReEt::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //evaluate
    double xi = m_pProcessModule->getXiConverterModule()->compute(kinematic).getValue();
    double cff = m_pProcessModule->getConvolCoeffFunctionValue(GPDType::Et).real();

    //change to nb
    return PhysicalType<double>(xi * cff, PhysicalUnit::NONE);
}

} /* namespace PARTONS */
