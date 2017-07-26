#ifndef DVCSAULMINUSSIN3PHI
#define DVCSAULMINUSSIN3PHI

/**
 * @file DVCSAulMinusSin3Phi.h
 * @author Luca COLANERI (IPNO)
 * @date July 18, 2016
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../../../MathIntegratorModule.h"
#include "DVCSAulMinus.h"

namespace PARTONS {

/**
 * @class DVCSAulMinusSin3Phi
 *
 * @brief 3th Fourier moment of longitudinally polarized target asymmetry for negative beam charge.
 *
 * Definition:<br>
 *
 * \f$
 * A_{UL}^{\sin(3\phi)}(x_{B}, t, Q^2) =
 * \frac{1}{\pi}
 * \int_{0}^{2\pi} d\phi A_{UL}(x_{B}, t, Q^2, \phi) \sin(3\phi)
 * \f$
 *
 * where \f$A_{UL}(x_{B}, t, Q^2, \phi)\f$ is defined in DVCSAulMinus.
 */
class DVCSAulMinusSin3Phi: public DVCSAulMinus, public MathIntegratorModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAulMinusSin3Phi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAulMinusSin3Phi();

    virtual DVCSAulMinusSin3Phi* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computeFourierObservable();

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAulMinusSin3Phi(const DVCSAulMinusSin3Phi &other);

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

#endif /* DVCSAULMINUSSIN3PHI */
