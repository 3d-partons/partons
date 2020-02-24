#include "../../../../../include/partons/modules/convol_coeff_function/TCS/TCSCFFFromDVCSDifferentiation.h"

#include <complex>

#include "../../../../../include/partons/beans/convol_coeff_function/ConvolCoeffFunctionResult.h"
#include "../../../../../include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../../../../../include/partons/beans/List.h"
#include "../../../../../include/partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"

namespace PARTONS {

double TCSCFFFromDVCSDifferentiationFunction(double Q2, void* params) {

    TCSCFFFromDVCSDifferentiationParameters* tcsParams =
            static_cast<TCSCFFFromDVCSDifferentiationParameters*>(params);

    List<GPDType> gpdTypeList;
    gpdTypeList.add(GPDType(tcsParams->m_currentGPDComputeType));

    std::complex<double> result =
            ((tcsParams->m_pDVCSConvolCoeffFunctionModule)->compute(
                    DVCSConvolCoeffFunctionKinematic(tcsParams->m_xi,
                            tcsParams->m_t, Q2, tcsParams->m_MuF2,
                            tcsParams->m_MuR2), gpdTypeList)).getResult(
                    tcsParams->m_currentGPDComputeType);

    return (tcsParams->m_isReal) ? (result.real()) : (result.imag());
}

} /* namespace PARTONS */
