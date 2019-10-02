#ifndef TCSACUTHETAINTEGRATED_H
#define TCSACUTHETAINTEGRATED_H

/**
 * @file TCSAcuThetaIntegrated.h
 * @author Pawel Sznajder (IPNO)
 * @date November 25, 2016
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <utility>
#include <vector>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../../../MathIntegratorModule.h"
#include "TCSAcu.h"

namespace PARTONS {

/**
 * @class TCSAcuThetaIntegrated
 * @brief ACU asymmetry integrated over \f$\theta\f$ in a given range (default: \f$|\theta - \pi/2| < \pi/4\f$).
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * \int_{\theta_{\mathrm{min}}}^{\theta_{\mathrm{max}}} A_{CU}{\mathrm{d}|t| \mathrm{d}Q^2 \mathrm{d}\phi \mathrm{d}\theta}\left(t, Q^2, \phi, \theta\right)\, .
 * \f$
 *
 * Unit: none.
 */
class TCSAcuThetaIntegrated: public TCSAcu,
        public MathIntegratorModule {

public:

    static const std::string PARAMETER_NAME_THETA_LIMIT; ///< Name of parameter to set \f$\theta_{\mathrm{limit}}\f$, which is the limit on \f$\theta\f$ angle integration, defined as \f$|\theta - \pi/2| < \theta_{\mathrm{limit}}\f$. In degrees.

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    TCSAcuThetaIntegrated(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~TCSAcuThetaIntegrated();

    virtual TCSAcuThetaIntegrated* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);

    /**
     * Get range of theta angle used in the integration.
     */
    const std::pair<double, double>& getThetaRange() const;

    /**
     * Set range of theta angle used in the integration.
     */
    void setThetaRange(const std::pair<double, double>& thetaRange);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSAcuThetaIntegrated(
            const TCSAcuThetaIntegrated &other);

    virtual PhysicalType<double> computeObservable(
            const TCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);

    /**
     * Functor to perform the integration.
     */
    NumA::FunctionType1D* m_pFunctionToIntegrateObservableTheta;

    /**
     * Function to be integrated.
     */
    virtual double functionToIntegrateObservableTheta(double x,
            std::vector<double> params);

    /**
     * Initialize functors.
     */
    virtual void initFunctorsForIntegrations();

    std::pair<double, double> m_thetaRange; ///< Range of theta angle used in the integration.
};

} /* namespace PARTONS */

#endif /* TCSACUTHETAINTEGRATED_H */
