#ifndef DVMP_CFF_GK06_INTEGRATION_PARAMETERS_H
#define DVMP_CFF_GK06_INTEGRATION_PARAMETERS_H

/**
 * @file DVMPCFFGK06.h
 * @author Kemal Tezgin
 * @date October 21, 2019
 * @version 1.0
 */

#include <stddef.h>

#include "../../../beans/gpd/GPDType.h"

namespace PARTONS {
class DVMPCFFGK06;
} /* namespace PARTONS */

namespace PARTONS {

struct DVMPCFFGK06IntegrationParameters {

    DVMPCFFGK06* m_pDVMPCFFGK06; ///< Pointer to DVMPCFFGK06.
    GPDType::Type m_gpdType; ///< GPD type.
    size_t m_twist; ///< Twist.
    bool m_isReal; ///< If true, return real part.
};

} /* namespace PARTONS */

#endif /* DVMP_CFF_GK06_INTEGRATION_PARAMETERS_H */
