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
 * @brief Evaluate cross-section integrated over \f$\phi\f$ with parameters set via automatization mechanism.
 *
 * This module can be used to evaluate a given cross-section integrated over \f$/phi\f$ with beam charge, beam helicity and target polarization set via the automatization mechanism.
 *
 * Definition:<br>
 *
 * \f$
 * d^4\sigma_{t_{h}}^{b_{h} b_{c}}(x_{B}, t, Q^2, \phi_{S}) =
 * \int_{0}^{2\pi} d\phi d^5\sigma_{t_{h}}^{b_{h} b_{c}}(x_{B}, t, Q^2, \phi, \phi_{S})
 * \f$
 *
 * where:
 * \f$\sigma_{t_{h}}^{b_{h} b_{c}}\f$ is single photon production cross-section (DVCS, BH and Int) for target helicity denoted by \f$t_{h}\f$, beam helicity denoted by \f$b_{h}\f$ and beam charge denoted by \f$b_{c}\f$.
 *
 * Units:<br>
 *
 *\f$1/\mathrm{GeV}^6\f$
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
