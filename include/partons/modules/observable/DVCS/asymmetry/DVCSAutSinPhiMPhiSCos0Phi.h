#ifndef DVCSAUTSINPHIMPHISCOS0PHI_H
#define DVCSAUTSINPHIMPHISCOS0PHI_H

/**
 * @file DVCSAutSinPhiMPhiSCos0Phi.h
 * @author Luca COLANERI (IPNO)
 * @date July 18, 2016
 * @version 1.0
 */

#include <string>
#include <vector>

#include "../../../MathIntegratorModule.h"
#include "DVCSAutSinPhiMPhiS.h"

/**
 * @class DVCSAutSinPhiMPhiSCos0Phi
 *
 * @brief 0th Fourier moment of transverse target beam asymmetry for negative beam charge.
 *
 * Definition:<br>
 *
 * \f$
 * A_{UT}^{\sin(\phi-\phi_{S})\cos(0\phi)}(x_{B}, t, Q^2) =
 * \frac{1}{2\pi}
 * \int_{0}^{2\pi} d\phi A_{UT}^{\sin(\phi-\phi_{S})}(x_{B}, t, Q^2, \phi) \cos(0\phi)
 * \f$
 *
 * where \f$A_{UT}^{\sin(\phi-\phi_{S})}(x_{B}, t, Q^2, \phi)\f$ is defined in DVCSAutSinPhiMPhiS.
 */
class DVCSAutSinPhiMPhiSCos0Phi: public DVCSAutSinPhiMPhiS,
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
    DVCSAutSinPhiMPhiSCos0Phi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAutSinPhiMPhiSCos0Phi();

    virtual DVCSAutSinPhiMPhiSCos0Phi* clone() const;
    virtual double computeFourierObservable();

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAutSinPhiMPhiSCos0Phi(const DVCSAutSinPhiMPhiSCos0Phi &other);

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

#endif /* DVCSAUTSINPHIMPHISCOS0PHI_H */
