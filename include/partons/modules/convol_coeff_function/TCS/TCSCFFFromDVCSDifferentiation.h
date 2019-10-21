#ifndef TCS_CFF_FROM_DVCS_DIFFERENTIATION_H
#define TCS_CFF_FROM_DVCS_DIFFERENTIATION_H

/**
 * @file TCSCFFFromDVCSDifferentiation.h
 * @author Oskar Grocholski (UW/NCBJ)
 * @date September 08, 2014
 * @version 1.0
 */

#include <complex>

#include "../../../beans/convol_coeff_function/ConvolCoeffFunctionResult.h"
#include "../../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/List.h"
#include "../DVCS/DVCSConvolCoeffFunctionModule.h"

namespace PARTONS {

struct TCSCFFFromDVCSDifferentiationParameters {

    DVCSConvolCoeffFunctionModule* m_pDVCSConvolCoeffFunctionModule;
    double m_xi, m_t, m_MuF2, m_MuR2;
    GPDType::Type m_currentGPDComputeType;
    bool m_isReal;
};

double TCSCFFFromDVCSDifferentiationFunction(double Q2, void* voidParams) {

    TCSCFFFromDVCSDifferentiationParameters* params =
            static_cast<TCSCFFFromDVCSDifferentiationParameters*>(voidParams);

    List<GPDType> gpdTypeList;
    gpdTypeList.add(GPDType(params->m_currentGPDComputeType));

    std::complex<double> result =
            ((params->m_pDVCSConvolCoeffFunctionModule)->compute(
                    DVCSConvolCoeffFunctionKinematic(params->m_xi,
                            params->m_t, Q2, params->m_MuF2,
                            params->m_MuR2), gpdTypeList)).getResult(
                    params->m_currentGPDComputeType);

    return (params->m_isReal) ? (result.real()) : (result.imag());
}

} /* namespace PARTONS */

#endif /* TCS_CFF_FROM_DVCS_DIFFERENTIATION_H */
