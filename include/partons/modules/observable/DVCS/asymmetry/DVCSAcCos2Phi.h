#ifndef DVCSACCOS2PHI_H
#define DVCSACCOS2PHI_H

/**
 * @file DVCSAcCos2Phi.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date September 28, 2015
 * @version 1.0
 */

#include <string>
#include <vector>

#include "../../../MathIntegratorModule.h"
#include "DVCSAc.h"

namespace NumA {
class FunctionType1D;
} /* namespace NumA */

/**
 * @class DVCSAcCos2Phi
 *
 * @brief 2th Fourier moment of beam charge asymmetry.
 *
 * Definition:<br>
 *
 * \f$
 * A_{C}^{\cos(2\phi)}(x_{B}, t, Q^2) =
 * \frac{1}{\pi}
 * \int_{0}^{2\pi} d\phi A_{C}(x_{B}, t, Q^2, \phi) \cos(2\phi)
 * \f$
 *
 * where \f$A_{C}(x_{B}, t, Q^2, \phi)\f$ is defined in DVCSAc.
 */
class DVCSAcCos2Phi: public DVCSAc, public MathIntegratorModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAcCos2Phi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAcCos2Phi();

    virtual DVCSAcCos2Phi* clone() const;
    virtual double computeFourierObservable();

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAcCos2Phi(const DVCSAcCos2Phi &other);

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

#endif /* DVCSACCOS2PHI_H */
