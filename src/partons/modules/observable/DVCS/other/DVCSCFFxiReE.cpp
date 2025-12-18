#include "../../../../../../include/partons/modules/observable/DVCS/other/DVCSCFFxiReE.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../../../../include/partons/modules/xi_converter/DVCS/DVCSXiConverterModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSCFFxiReE::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCFFxiReE("DVCSCFFxiReE"));

DVCSCFFxiReE::DVCSCFFxiReE(const std::string &className) :
        DVCSObservable(className) {
}

DVCSCFFxiReE::DVCSCFFxiReE(
        const DVCSCFFxiReE& other) :
        DVCSObservable(other) {
}

DVCSCFFxiReE::~DVCSCFFxiReE() {
}

DVCSCFFxiReE* DVCSCFFxiReE::clone() const {
    return new DVCSCFFxiReE(*this);
}

PhysicalType<double> DVCSCFFxiReE::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //evaluate
    double xi = m_pProcessModule->getXiConverterModule()->compute(kinematic).getValue();
    double cff = m_pProcessModule->getConvolCoeffFunctionValue(GPDType::E).real();

    //change to nb
    return PhysicalType<double>(xi * cff, PhysicalUnit::NONE);
}

} /* namespace PARTONS */
