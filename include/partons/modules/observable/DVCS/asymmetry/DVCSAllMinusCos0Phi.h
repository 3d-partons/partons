#ifndef DVCSALLMINUSCOS0PHI_H
#define DVCSALLMINUSCOS0PHI_H

/**
 * @file DVCSAllMinusCos0Phi.h
 * @author Luca COLANERI (IPNO)
 * @date July 18, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../../../MathIntegratorModule.h"
#include "DVCSAllMinus.h"

/**
 * @class DVCSAllMinusCos0Phi
 *
 * @brief 0th Fourier moment of longitudinally polarized beam and target asymmetry for negative beam charge.
 *
 * Definition:<br>
 *
 * \f$
 * A_{LL}^{\cos(0\phi)}(x_{B}, t, Q^2) =
 * \frac{1}{2\pi}
 * \int_{0}^{2\pi} d\phi A_{LL}(x_{B}, t, Q^2, \phi) \cos(0\phi)
 * \f$
 *
 * where \f$A_{LL}(x_{B}, t, Q^2, \phi)\f$ is defined in DVCSAllMinus.
 */
class DVCSAllMinusCos0Phi: public DVCSAllMinus, public MathIntegratorModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAllMinusCos0Phi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAllMinusCos0Phi();

    virtual DVCSAllMinusCos0Phi* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computeFourierObservable();

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAllMinusCos0Phi(const DVCSAllMinusCos0Phi &other);

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

#endif /* DVCSALLMINUSCOS0PHI_H */
