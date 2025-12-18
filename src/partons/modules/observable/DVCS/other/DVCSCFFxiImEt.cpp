#include "../../../../../../include/partons/modules/observable/DVCS/other/DVCSCFFxiImEt.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../../../../include/partons/modules/xi_converter/DVCS/DVCSXiConverterModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSCFFxiImEt::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCFFxiImEt("DVCSCFFxiImEt"));

DVCSCFFxiImEt::DVCSCFFxiImEt(const std::string &className) :
        DVCSObservable(className) {
}

DVCSCFFxiImEt::DVCSCFFxiImEt(
        const DVCSCFFxiImEt& other) :
        DVCSObservable(other) {
}

DVCSCFFxiImEt::~DVCSCFFxiImEt() {
}

DVCSCFFxiImEt* DVCSCFFxiImEt::clone() const {
    return new DVCSCFFxiImEt(*this);
}

PhysicalType<double> DVCSCFFxiImEt::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //evaluate
    double xi = m_pProcessModule->getXiConverterModule()->compute(kinematic).getValue();
    double cff = m_pProcessModule->getConvolCoeffFunctionValue(GPDType::Et).imag();

    //change to nb
    return PhysicalType<double>(xi * cff, PhysicalUnit::NONE);
}

} /* namespace PARTONS */
