#ifndef DVCSALUINTSIN2Phi_H
#define DVCSALUINTSIN2Phi_H

/**
 * @file DVCSAluIntSin2Phi.h
 * @author Luca COLANERI (IPNO)
 * @date July 19, 2016
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../../../MathIntegratorModule.h"
#include "DVCSAluInt.h"

namespace PARTONS {

/**
 * @class DVCSAluIntSin2Phi
 *
 * @brief 2th Fourier moment of longitudinally polarized beam asymmetry for difference over beam charges (interference part).
 *
 * Definition:<br>
 *
 * \f$
 * A_{LU, Int}^{\sin(2\phi)}(x_{B}, t, Q^2) =
 * \frac{1}{\pi}
 * \int_{0}^{2\pi} d\phi A_{LU, Int}(x_{B}, t, Q^2, \phi) \sin(2\phi)
 * \f$
 *
 * where \f$A_{LU, Int}(x_{B}, t, Q^2, \phi)\f$ is defined in DVCSAluInt.
 */
class DVCSAluIntSin2Phi: public DVCSAluInt, public MathIntegratorModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAluIntSin2Phi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAluIntSin2Phi();

    virtual DVCSAluIntSin2Phi* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computeFourierObservable();

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAluIntSin2Phi(const DVCSAluIntSin2Phi &other);

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

#endif /* DVCSALUINTSIN2Phi_H */
