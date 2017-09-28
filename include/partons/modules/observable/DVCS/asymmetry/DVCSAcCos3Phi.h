#ifndef DVCSACCOS3PHI_H
#define DVCSACCOS3PHI_H

/**
 * @file DVCSAcCos3Phi.h
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
 * @class DVCSAcCos3Phi
 *
 * @brief 3th Fourier moment of beam charge asymmetry.
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{C}^{\cos\left(3\phi\right)}\left(x_{B}, t, Q^2\right) =
 * \frac{1}{\pi}
 * \int_{0}^{2\pi} \mathrm{d}\phi A_{C}\left(x_{B}, t, Q^2, \phi\right) \cos\left(3\phi\right) \, ,
 * \f$
 *
 * where \f$A_{C}\left(x_{B}, t, Q^2, \phi\right)\f$ is defined in DVCSAc.
 */
class DVCSAcCos3Phi: public DVCSAc, public MathIntegratorModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAcCos3Phi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAcCos3Phi();


    virtual DVCSAcCos3Phi* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computeFourierObservable();

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAcCos3Phi(const DVCSAcCos3Phi &other);

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

#endif /* DVCSACCOS3PHI_H */
