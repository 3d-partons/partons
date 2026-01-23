#include "../../../../../../include/partons/modules/observable/DVCS/other/DVCSCFFxiImEt.h"

#include <NumA/linear_algebra/vector/Vector3D.h>

#include "../../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../../include/partons/beans/Scales.h"
#include "../../../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../../../../../../include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../../../../../../include/partons/modules/xi_converter/DVCS/DVCSXiConverterModule.h"
#include "../../../../../../include/partons/modules/scales/DVCS/DVCSScalesModule.h"
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
        List<GPDType> gpdList;
        gpdList.add(GPDType::Et);

        PhysicalType<double> xi = m_pProcessModule->getXiConverterModule()->compute(kinematic);
        Scales scales = m_pProcessModule->getScaleModule()->compute(kinematic);

        std::complex<double> cff =
            m_pProcessModule->getConvolCoeffFunctionModule()->compute(DVCSConvolCoeffFunctionKinematic(xi, kinematic.getT(), kinematic.getQ2(), scales.getMuF2(), scales.getMuR2()), gpdList).getResult(gpdList[0]);

        //return
        return PhysicalType<double>(xi.getValue() * cff.imag(), PhysicalUnit::NONE);
}

} /* namespace PARTONS */
