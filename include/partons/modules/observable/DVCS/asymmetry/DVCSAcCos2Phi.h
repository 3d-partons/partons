#ifndef DVCSACCOS2PHI_H
#define DVCSACCOS2PHI_H

/**
 * @file DVCSAcCos2Phi.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date September 28, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../../../MathIntegratorModule.h"
#include "DVCSAc.h"

namespace NumA {
class FunctionType1D;
} /* namespace NumA */

namespace PARTONS {

/**
 * @class DVCSAcCos2Phi
 *
 * @brief 2th Fourier moment of beam charge asymmetry.
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{C}^{\cos\left(2\phi\right)}\left(x_{B}, t, Q^2\right) =
 * \frac{1}{\pi}
 * \int_{0}^{2\pi} \mathrm{d}\phi A_{C}\left(x_{B}, t, Q^2, \phi\right) \cos\left(2\phi\right) \, ,
 * \f$
 *
 * where \f$A_{C}\left(x_{B}, t, Q^2, \phi\right)\f$ is defined in DVCSAc.
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
    virtual void configure(const ElemUtils::Parameters &parameters);
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

} /* namespace PARTONS */

#endif /* DVCSACCOS2PHI_H */
