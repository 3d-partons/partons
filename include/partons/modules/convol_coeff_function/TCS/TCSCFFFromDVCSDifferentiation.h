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
#include "../ConvolCoeffFunctionModule.h"
#include "../DVCS/DVCSConvolCoeffFunctionModule.h"

namespace PARTONS {

struct TCSCFFFromDVCSDifferentiationParameters {

    DVCSConvolCoeffFunctionModule* m_pDVCSConvolCoeffFunctionModule;
    double m_xi, m_t, m_MuF2, m_MuR2;
    GPDType::Type m_currentGPDComputeType;
};

double TCSCFFFromDVCSDifferentiationFunction(double Q2, void* params) {

    TCSCFFFromDVCSDifferentiationParameters* tcsParams =
            static_cast<TCSCFFFromDVCSDifferentiationParameters*>(params);

    List<GPDType> gpdTypeList;
    gpdTypeList.add(GPDType(tcsParams->m_currentGPDComputeType));

    return tcsParams->m_pDVCSConvolCoeffFunctionModule->compute(
            DVCSConvolCoeffFunctionKinematic(tcsParams->m_xi, tcsParams->m_t,
                    Q2, tcsParams->m_MuF2, tcsParams->m_MuR2), gpdTypeList).getResult(
            tcsParams->m_currentGPDComputeType).imag();
}

} /* namespace PARTONS */

#endif /* TCS_CFF_FROM_DVCS_DIFFERENTIATION_H */
