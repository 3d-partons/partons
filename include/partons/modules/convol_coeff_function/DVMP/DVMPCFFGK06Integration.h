#ifndef DVMP_CFF_GK06_INTEGRATION_H
#define DVMP_CFF_GK06_INTEGRATION_H

/**
 * @file DVMPCFFGK06.h
 * @author Kemal Tezgin
 * @date October 21, 2019
 * @version 1.0
 */

#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/QuarkFlavor.h"
#include "DVMPCFFGK06.h"

namespace PARTONS {

struct DVMPCFFGK06IntegrationParameters {

    const DVMPCFFGK06* m_pDVMPCFFGK06;
    GPDType::Type m_gpdType;
    QuarkFlavor::Type m_quarkType;
    bool m_isReal;
};

double DVMPCFFGK06IntegrationFunctionQuark(double* x, size_t n, void* voidParams) {

    DVMPCFFGK06IntegrationParameters* params =
            static_cast<DVMPCFFGK06IntegrationParameters*>(voidParams);
    return (params->m_pDVMPCFFGK06)->quarkUnintegratedAmplitude(x[0], x[1], x[2],
            params->m_gpdType, params->m_quarkType);
}

double DVMPCFFGK06IntegrationFunctionGluon(double* x, size_t n, void* voidParams) {

    DVMPCFFGK06IntegrationParameters* params =
            static_cast<DVMPCFFGK06IntegrationParameters*>(voidParams);
    return params->m_pDVMPCFFGK06->gluonUnintegratedAmplitude(x[0], x[1], x[2],
            params->m_gpdType);
}

} /* namespace PARTONS */

#endif /* DVMP_CFF_GK06_INTEGRATION_H */
