#ifndef DVCSAUTSINPHIMPHISCOS0PHI_H
#define DVCSAUTSINPHIMPHISCOS0PHI_H

/**
 * @file DVCSAutSinPhiMPhisCos0Phi.h
 * @author Luca COLANERI (IPNO)
 * @date July 18, 2016
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../../../MathIntegratorModule.h"
#include "DVCSAutSinPhiMPhis.h"

namespace PARTONS {

/**
 * @class DVCSAutSinPhiMPhisCos0Phi
 *
 * @brief 0th Fourier moment of transverse target beam asymmetry for negative beam charge.
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{UT}^{\sin\left(\phi-\phi_{S}\right)\cos\left(0\phi\right)}\left(x_{B}, t, Q^2\right) =
 * \frac{1}{2\pi}
 * \int_{0}^{2\pi} \mathrm{d}\phi A_{UT}^{\sin\left(\phi-\phi_{S}\right)}\left(x_{B}, t, Q^2, \phi\right) \cos\left(0\phi\right) \, ,
 * \f$
 *
 * where \f$A_{UT}^{\sin\left(\phi-\phi_{S}\right)}\left(x_{B}, t, Q^2, \phi\right)\f$ is defined in DVCSAutSinPhiMPhis.
 */
class DVCSAutSinPhiMPhisCos0Phi: public DVCSAutSinPhiMPhis,
        public MathIntegratorModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAutSinPhiMPhisCos0Phi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAutSinPhiMPhisCos0Phi();

    virtual DVCSAutSinPhiMPhisCos0Phi* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computeFourierObservable();

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAutSinPhiMPhisCos0Phi(const DVCSAutSinPhiMPhisCos0Phi &other);

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

#endif /* DVCSAUTSINPHIMPHISCOS0PHI_H */
