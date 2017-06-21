#ifndef DVCSALLMINUSCOS2PHI_H
#define DVCSALLMINUSCOS2PHI_H

/**
 * @file DVCSAllMinusCos2Phi.h
 * @author Luca COLANERI (IPNO)
 * @date July 18, 2015
 * @version 1.0
 */

#include <string>
#include <vector>

#include "../../../MathIntegratorModule.h"
#include "DVCSAllMinus.h"

/**
 * @class DVCSAllMinusCos2Phi
 *
 * @brief 2th Fourier moment of longitudinally polarized beam and target asymmetry for negative beam charge.
 *
 * Definition:<br>
 *
 * \f$
 * A_{LL}^{\cos(2\phi)}(x_{B}, t, Q^2) =
 * \frac{1}{\pi}
 * \int_{0}^{2\pi} d\phi A_{LL}(x_{B}, t, Q^2, \phi) \cos(2\phi)
 * \f$
 *
 * where \f$A_{LL}(x_{B}, t, Q^2, \phi)\f$ is defined in DVCSAllMinus.
 */
class DVCSAllMinusCos2Phi: public DVCSAllMinus, public MathIntegratorModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAllMinusCos2Phi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAllMinusCos2Phi();

    virtual DVCSAllMinusCos2Phi* clone() const;
    virtual double computeFourierObservable();

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAllMinusCos2Phi(const DVCSAllMinusCos2Phi &other);

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

#endif /* DVCSALLMINUSCOS2PHI_H */
