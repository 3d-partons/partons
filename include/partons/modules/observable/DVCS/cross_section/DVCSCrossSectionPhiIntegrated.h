#ifndef DVCSCROSSSECTIONPHIINTEGRATED_H
#define DVCSCROSSSECTIONPHIINTEGRATED_H

/**
 * @file DVCSCrossSectionPhiIntegrated.h
 * @author Luca COLANERI (IPNO)
 * @date July 18, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../../../MathIntegratorModule.h"
#include "DVCSCrossSection.h"

namespace PARTONS {

/**
 * @class DVCSCrossSectionPhiIntegrated
 *
 * @brief Evaluate *differential* cross-section integrated over \f$\phi\f$ with parameters set via automation mechanism.
 *
 * This module can be used to evaluate a given cross-section integrated over \f$\phi\f$ with beam charge, beam helicity and target polarization set via the automation mechanism.
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * \frac{\mathrm{d}^4\sigma_{t_{h}}^{b_{h} b_{c}}}
 * {\mathrm{d}x_{B} \mathrm{d}|t| \mathrm{d}Q^2 \mathrm{d}\phi_{S}}\left(x_{B}, t, Q^2, \phi_{S}\right) =
 * \int_{0}^{2\pi} \mathrm{d}\phi \frac{\mathrm{d}^5\sigma_{t_{h}}^{b_{h} b_{c}}}
 * {\mathrm{d}x_{B} \mathrm{d}|t| \mathrm{d}Q^2 \mathrm{d}\phi \mathrm{d}\phi_{S}}\left(x_{B}, t, Q^2, \phi, \phi_{S}\right) \, ,
 * \f$
 *
 * where
 * \f$\sigma_{t_{h}}^{b_{h} b_{c}}\f$ is a single photon production cross-section (DVCS, BH and Interference) for target helicity denoted by \f$t_{h}\f$, beam helicity denoted by \f$b_{h}\f$ and beam charge denoted by \f$b_{c}\f$.
 *
 * Unit: \f$\mathrm{GeV}^{-6}\f$.
 */
class DVCSCrossSectionPhiIntegrated: public DVCSCrossSection, public MathIntegratorModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSCrossSectionPhiIntegrated(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSCrossSectionPhiIntegrated();

    virtual DVCSCrossSectionPhiIntegrated* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computeFourierObservable();

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSCrossSectionPhiIntegrated(const DVCSCrossSectionPhiIntegrated &other);

    /**
     * Functor to perform the integration.
     */
    NumA::FunctionType1D* m_pFunctionToIntegrateObservable;

    /**
     * Function to be integrated.
     */
    virtual double functionToIntegrateObservable(double x,
            std::vector<double> params);

    /**
     * Initialize functors.
     */
    void initFunctorsForIntegrations();
};

} /* namespace PARTONS */


#endif /* DVCSCROSSSECTIONPHIINTEGRATED_H */
