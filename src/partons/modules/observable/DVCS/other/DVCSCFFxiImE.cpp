#include "../../../../../../include/partons/modules/observable/DVCS/other/DVCSCFFxiImE.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../../../../include/partons/modules/xi_converter/DVCS/DVCSXiConverterModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSCFFxiImE::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCFFxiImE("DVCSCFFxiImE"));

DVCSCFFxiImE::DVCSCFFxiImE(const std::string &className) :
        DVCSObservable(className) {
}

DVCSCFFxiImE::DVCSCFFxiImE(
        const DVCSCFFxiImE& other) :
        DVCSObservable(other) {
}

DVCSCFFxiImE::~DVCSCFFxiImE() {
}

DVCSCFFxiImE* DVCSCFFxiImE::clone() const {
    return new DVCSCFFxiImE(*this);
}

PhysicalType<double> DVCSCFFxiImE::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //evaluate
    double xi = m_pProcessModule->getXiConverterModule()->compute(kinematic).getValue();
    double cff = m_pProcessModule->getConvolCoeffFunctionValue(GPDType::E).imag();

    //change to nb
    return PhysicalType<double>(xi * cff, PhysicalUnit::NONE);
}

} /* namespace PARTONS */
