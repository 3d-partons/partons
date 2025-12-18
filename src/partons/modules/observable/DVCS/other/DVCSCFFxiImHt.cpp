#include "../../../../../../include/partons/modules/observable/DVCS/other/DVCSCFFxiImHt.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../../../../include/partons/modules/xi_converter/DVCS/DVCSXiConverterModule.h"
#include "../../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const unsigned int DVCSCFFxiImHt::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVCSCFFxiImHt("DVCSCFFxiImHt"));

DVCSCFFxiImHt::DVCSCFFxiImHt(const std::string &className) :
        DVCSObservable(className) {
}

DVCSCFFxiImHt::DVCSCFFxiImHt(
        const DVCSCFFxiImHt& other) :
        DVCSObservable(other) {
}

DVCSCFFxiImHt::~DVCSCFFxiImHt() {
}

DVCSCFFxiImHt* DVCSCFFxiImHt::clone() const {
    return new DVCSCFFxiImHt(*this);
}

PhysicalType<double> DVCSCFFxiImHt::computeObservable(
        const DVCSObservableKinematic& kinematic,
        const List<GPDType>& gpdType) {

    //evaluate
    double xi = m_pProcessModule->getXiConverterModule()->compute(kinematic).getValue();
    double cff = m_pProcessModule->getConvolCoeffFunctionValue(GPDType::Ht).imag();

    //change to nb
    return PhysicalType<double>(xi * cff, PhysicalUnit::NONE);
}

} /* namespace PARTONS */
