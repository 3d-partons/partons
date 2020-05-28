#ifndef TCS_CFF_FROM_DVCS_DIFFERENTIATION_H
#define TCS_CFF_FROM_DVCS_DIFFERENTIATION_H

/**
 * @file TCSCFFFromDVCSDifferentiation.h
 * @author Oskar Grocholski (UW/NCBJ)
 * @date September 08, 2014
 * @version 1.0
 */

#include "../../../beans/gpd/GPDType.h"

namespace PARTONS {
class DVCSConvolCoeffFunctionModule;
} /* namespace PARTONS */

namespace PARTONS {

struct TCSCFFFromDVCSDifferentiationParameters {

    DVCSConvolCoeffFunctionModule* m_pDVCSConvolCoeffFunctionModule;
    double m_xi, m_t, m_MuF2, m_MuR2;
    GPDType::Type m_currentGPDComputeType;
    bool m_isReal;
};

double TCSCFFFromDVCSDifferentiationFunction(double Q2, void* params);

} /* namespace PARTONS */

#endif /* TCS_CFF_FROM_DVCS_DIFFERENTIATION_H */
