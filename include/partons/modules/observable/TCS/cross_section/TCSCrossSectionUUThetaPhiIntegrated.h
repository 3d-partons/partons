#ifndef TCSCROSSSECTIONTHETAPHIINTEGRATEDUU_H
#define TCSCROSSSECTIONTHETAPHIINTEGRATEDUU_H

/**
 * @file TCSCrossSectionUUThetaPhiIntegrated.h
 * @author Pawel Sznajder (IPNO)
 * @date November 25, 2016
 * @version 1.0
 */

#include <string>
#include <vector>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "TCSCrossSectionUUThetaIntegrated.h"

namespace PARTONS {

/**
 * @class TCSCrossSectionUUThetaPhiIntegrated
 * @brief Unpolarized cross-section integrated over \f$\theta\f$ in a given range (default: \f$|\theta - \pi/2| < \pi/4\f$) and \f$\phi\f$ in the full range.
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * \int_{0}^{2\pi}\mathrm{d}\phi\int_{\theta_{\mathrm{min}}}^{\theta_{\mathrm{max}}}\mathrm{d}\theta} \frac{\mathrm{d}^4\sigma_{UU}}{\mathrm{d}|t| \mathrm{d}Q^2 \mathrm{d}\phi \mathrm{d}\theta}\left(t, Q^2, \phi, \theta\right) \, .
 * \f$
 *
 * Unit: \f$\mathrm{nbarn}/\mathrm{GeV}^4\f$.
 */
class TCSCrossSectionUUThetaPhiIntegrated: public TCSCrossSectionUUThetaIntegrated {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    TCSCrossSectionUUThetaPhiIntegrated(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~TCSCrossSectionUUThetaPhiIntegrated();

    virtual TCSCrossSectionUUThetaPhiIntegrated* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSCrossSectionUUThetaPhiIntegrated(
            const TCSCrossSectionUUThetaPhiIntegrated &other);

    virtual PhysicalType<double> computeObservable(
            const TCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);

    /**
     * Functor to perform the integration.
     */
    NumA::FunctionType1D* m_pFunctionToIntegrateObservablePhi;

    /**
     * Function to be integrated.
     */
    virtual double functionToIntegrateObservablePhi(double x,
            std::vector<double> params);

    /**
     * Initialize functors.
     */
    void initFunctorsForIntegrations();

};

} /* namespace PARTONS */

#endif /* TCSCROSSSECTIONTHETAPHIINTEGRATEDUU_H */
