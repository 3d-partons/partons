#include "../../../../../../include/partons/modules/observable/DVCS/other/DVCSCFFxiReH.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../../../../include/partons/modules/xi_converter/DVCS/DVCSXiConverterModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSCFFxiReH::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCFFxiReH("DVCSCFFxiReH"));

DVCSCFFxiReH::DVCSCFFxiReH(const std::string &className) :
        DVCSObservable(className) {
}

DVCSCFFxiReH::DVCSCFFxiReH(
        const DVCSCFFxiReH& other) :
        DVCSObservable(other) {
}

DVCSCFFxiReH::~DVCSCFFxiReH() {
}

DVCSCFFxiReH* DVCSCFFxiReH::clone() const {
    return new DVCSCFFxiReH(*this);
}

PhysicalType<double> DVCSCFFxiReH::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //evaluate
    double xi = m_pProcessModule->getXiConverterModule()->compute(kinematic).getValue();
    double cff = m_pProcessModule->getConvolCoeffFunctionValue(GPDType::H).real();

    //change to nb
    return PhysicalType<double>(xi * cff, PhysicalUnit::NONE);
}

} /* namespace PARTONS */
