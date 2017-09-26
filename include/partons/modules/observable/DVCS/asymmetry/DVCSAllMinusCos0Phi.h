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

namespace PARTONS {

/**
 * @class DVCSAllMinusCos0Phi
 *
 * @brief 0th Fourier moment of longitudinally polarized beam and target asymmetry for negative beam charge.
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{LL}^{\cos\left(0\phi\right)}\left(x_{B}, t, Q^2\right) =
 * \frac{1}{2\pi}
 * \int_{0}^{2\pi} \mathrm{d}\phi A_{LL}\left(x_{B}, t, Q^2, \phi\right) \cos\left(0\phi\right) \, ,
 * \f$
 *
 * where \f$A_{LL}\left(x_{B}, t, Q^2, \phi\right)\f$ is defined in DVCSAllMinus.
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

} /* namespace PARTONS */

#endif /* DVCSALLMINUSCOS0PHI_H */
