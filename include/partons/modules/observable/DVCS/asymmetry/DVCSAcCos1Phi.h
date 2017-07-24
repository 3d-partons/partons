#ifndef DVCSACCOS1PHI_H
#define DVCSACCOS1PHI_H

/**
 * @file DVCSAcCos1Phi.h
 * @author Luca COLANERI (IPNO)
 * @date July 18, 2016
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../../../MathIntegratorModule.h"
#include "DVCSAc.h"

namespace PARTONS {

/**
 * @class DVCSAcCos1Phi
 *
 * @brief 1th Fourier moment of beam charge asymmetry.
 *
 * Definition:<br>
 *
 * \f$
 * A_{C}^{\cos(1\phi)}(x_{B}, t, Q^2) =
 * \frac{1}{\pi}
 * \int_{0}^{2\pi} d\phi A_{C}(x_{B}, t, Q^2, \phi) \cos(1\phi)
 * \f$
 *
 * where \f$A_{C}(x_{B}, t, Q^2, \phi)\f$ is defined in DVCSAc.
 */
class DVCSAcCos1Phi: public DVCSAc, public MathIntegratorModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAcCos1Phi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAcCos1Phi();

    virtual DVCSAcCos1Phi* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computeFourierObservable();

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAcCos1Phi(const DVCSAcCos1Phi &other);

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

#endif /* DVCSACCOS1PHI_H */
