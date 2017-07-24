#ifndef DVCSALUINTSIN1Phi_H
#define DVCSALUINTSIN1Phi_H

/**
 * @file DVCSAluIntSin1Phi.h
 * @author Luca COLANERI (IPNO)
 * @date July 19, 2016
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../../../MathIntegratorModule.h"
#include "DVCSAluInt.h"

/**
 * @class DVCSAluIntSin1Phi
 *
 * @brief 1th Fourier moment of longitudinally polarized beam asymmetry for difference over beam charges (interference part).
 *
 * Definition:<br>
 *
 * \f$
 * A_{LU, Int}^{\sin(1\phi)}(x_{B}, t, Q^2) =
 * \frac{1}{\pi}
 * \int_{0}^{2\pi} d\phi A_{LU, Int}(x_{B}, t, Q^2, \phi) \sin(1\phi)
 * \f$
 *
 * where \f$A_{LU, Int}(x_{B}, t, Q^2, \phi)\f$ is defined in DVCSAluInt.
 */
class DVCSAluIntSin1Phi: public DVCSAluInt, public MathIntegratorModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAluIntSin1Phi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAluIntSin1Phi();

    virtual DVCSAluIntSin1Phi* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computeFourierObservable();

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAluIntSin1Phi(const DVCSAluIntSin1Phi &other);

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

#endif /* DVCSALUINTSIN1Phi_H */
