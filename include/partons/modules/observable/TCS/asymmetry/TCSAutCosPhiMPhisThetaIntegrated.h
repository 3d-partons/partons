#ifndef TCSAUTCOSPHIMPHISTHETAINTEGRATED_H
#define TCSAUTCOSPHIMPHISTHETAINTEGRATED_H

/**
 * @file TCSAutCosPhiMPhisThetaIntegrated.h
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
#include "../TCSObservable.h"

namespace PARTONS {

/**
 * @class TCSAutCosPhiMPhisThetaIntegrated
 * @brief AutCosPhiMPhis asymmetry with cross-sections integrated over \f$\theta\f$ in a given range (default: \f$|\theta - \pi/2| < \pi/4\f$).
 *
 * Unit: none.
 */
class TCSAutCosPhiMPhisThetaIntegrated: public TCSObservable,
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
    TCSAutCosPhiMPhisThetaIntegrated(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~TCSAutCosPhiMPhisThetaIntegrated();

    virtual TCSAutCosPhiMPhisThetaIntegrated* clone() const;
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
    TCSAutCosPhiMPhisThetaIntegrated(
            const TCSAutCosPhiMPhisThetaIntegrated &other);

    virtual PhysicalType<double> computeObservable(
            const TCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);

    /**
     * Functor to perform the integration (polarization + target +).
     */
    NumA::FunctionType1D* m_pFunctionToIntegrateObservableTheta1;

    /**
     * Functor to perform the integration (polarization - target +).
     */
    NumA::FunctionType1D* m_pFunctionToIntegrateObservableTheta2;

    /**
     * Functor to perform the integration (polarization + target -).
     */
    NumA::FunctionType1D* m_pFunctionToIntegrateObservableTheta3;

    /**
     * Functor to perform the integration (polarization - target -).
     */
    NumA::FunctionType1D* m_pFunctionToIntegrateObservableTheta4;

    /**
     * Function to be integrated (polarization + target +).
     */
    virtual double functionToIntegrateObservableTheta1(double x,
            std::vector<double> params);

    /**
     * Function to be integrated (polarization - target +).
     */
    virtual double functionToIntegrateObservableTheta2(double x,
            std::vector<double> params);

    /**
     * Function to be integrated (polarization + target -).
     */
    virtual double functionToIntegrateObservableTheta3(double x,
            std::vector<double> params);

    /**
     * Function to be integrated (polarization - target -).
     */
    virtual double functionToIntegrateObservableTheta4(double x,
            std::vector<double> params);

    /**
     * Initialize functors.
     */
    virtual void initFunctorsForIntegrations();

    std::pair<double, double> m_thetaRange; ///< Range of theta angle used in the integration.
};

} /* namespace PARTONS */

#endif /* TCSAUTCOSPHIMPHISTHETAINTEGRATED_H */
