#include "../../../../../include/partons/modules/convol_coeff_function/TCS/TCSCFFFromDVCSOnlyNLOPart.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <gsl/gsl_deriv.h>
#include <gsl/gsl_math.h>

#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/List.h"
#include "../../../../../include/partons/beans/PerturbativeQCDOrderType.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../../../../../include/partons/modules/convol_coeff_function/TCS/TCSCFFFromDVCSDifferentiation.h"

namespace PARTONS {

const unsigned int TCSCFFFromDVCSOnlyNLOPart::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new TCSCFFFromDVCSOnlyNLOPart("TCSCFFFromDVCSOnlyNLOPart"));

TCSCFFFromDVCSOnlyNLOPart::TCSCFFFromDVCSOnlyNLOPart(
        const std::string &className) :
        TCSCFFFromDVCS(className) {
}

TCSCFFFromDVCSOnlyNLOPart::TCSCFFFromDVCSOnlyNLOPart(
        const TCSCFFFromDVCSOnlyNLOPart &other) :
        TCSCFFFromDVCS(other) {
}

TCSCFFFromDVCSOnlyNLOPart::~TCSCFFFromDVCSOnlyNLOPart() {
}

TCSCFFFromDVCSOnlyNLOPart* TCSCFFFromDVCSOnlyNLOPart::clone() const {
    return new TCSCFFFromDVCSOnlyNLOPart(*this);
}

std::complex<double> TCSCFFFromDVCSOnlyNLOPart::computeUnpolarized() {

    if (m_qcdOrderType != PerturbativeQCDOrderType::NLO) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "QCD order: "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString()
                        << " not implemented");
    }

    /*
     * Calculate TCS Compton Form Factors using results obtained for DVCS
     * For unpolarized CFFs we have:
     *      In LO     CFF_TCS = CFF_DVCS*
     *      In NLO    CFF_TCS = CFF_DVCS* - i PI Q2 d/dQ2 CFF_DVCS*
     */

    const double step = 1.E-8; // differentiation step

    //set module and pQCD order
    getDVCSConvolCoeffFunctionModule()->setQCDOrderType(m_qcdOrderType);

    //currect GPD type
    List<GPDType> gpdTypeList;
    gpdTypeList.add(GPDType(m_currentGPDComputeType));

    //result
    std::complex<double> TCSCFF;

    gsl_function gslFunction;
    TCSCFFFromDVCSDifferentiationParameters params;

    gslFunction.function = &TCSCFFFromDVCSDifferentiationFunction;
    gslFunction.params = static_cast<void*>(&params);

    double resultReal, abserrReal;
    double resultImag, abserrImag;

    params.m_pDVCSConvolCoeffFunctionModule =
            getDVCSConvolCoeffFunctionModule();
    params.m_xi = m_xi;
    params.m_t = m_t;
    params.m_MuF2 = m_MuF2;
    params.m_MuR2 = m_MuR2;
    params.m_currentGPDComputeType = m_currentGPDComputeType;

    params.m_isReal = true;
    gsl_deriv_central(&gslFunction, m_Q2Prim, step, &resultReal, &abserrReal);

    params.m_isReal = false;
    gsl_deriv_central(&gslFunction, m_Q2Prim, step, &resultImag, &abserrImag);

    TCSCFF = std::complex<double>(0., 0.);
    TCSCFF -= Constant::PI * m_Q2Prim * std::complex<double>(0., 1.)
            * std::conj(std::complex<double>(resultReal, resultImag));

    return TCSCFF;
}

} /* namespace PARTONS */
