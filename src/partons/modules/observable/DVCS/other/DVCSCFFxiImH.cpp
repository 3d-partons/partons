#include "../../../../../../include/partons/modules/observable/DVCS/other/DVCSCFFxiImH.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../../../../include/partons/modules/xi_converter/DVCS/DVCSXiConverterModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSCFFxiImH::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCFFxiImH("DVCSCFFxiImH"));

DVCSCFFxiImH::DVCSCFFxiImH(const std::string &className) :
        DVCSObservable(className) {
}

DVCSCFFxiImH::DVCSCFFxiImH(
        const DVCSCFFxiImH& other) :
        DVCSObservable(other) {
}

DVCSCFFxiImH::~DVCSCFFxiImH() {
}

DVCSCFFxiImH* DVCSCFFxiImH::clone() const {
    return new DVCSCFFxiImH(*this);
}

PhysicalType<double> DVCSCFFxiImH::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //evaluate
    double xi = m_pProcessModule->getXiConverterModule()->compute(kinematic).getValue();
    double cff = m_pProcessModule->getConvolCoeffFunctionValue(GPDType::H).imag();

    //change to nb
    return PhysicalType<double>(xi * cff, PhysicalUnit::NONE);
}

} /* namespace PARTONS */
